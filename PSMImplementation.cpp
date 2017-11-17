#include "PSMImplementation.h"

PSMImplementation::PSMImplementation(): slImplementation(string("PSMImplementation")),numberColumns(32) {
}

PSMImplementation::PSMImplementation(unsigned int nCol): slImplementation(string("PSMImplementation")),numberColumns(nCol) {
}
void PSMImplementation::preExperimentRun() {
	pixelsToProcess = new priority_queue<WrappedPixel, vector<WrappedPixel>, CompareWrappedPixel>();

	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int arraySize = cameraResolution.width * cameraResolution.height;

	phase = new float[arraySize];
	dist = new float[arraySize];
	mask = new int[arraySize];
	ready = new int[arraySize];
	names = new int[arraySize];
}

void PSMImplementation::postExperimentRun() {
	delete pixelsToProcess;

	delete[] phase;
	delete[] dist;
	delete[] mask;
	delete[] ready;
	delete[] names;
}

bool PSMImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < 3;
}

unsigned int PSMImplementation::getNumberColumns() {
    return this->numberColumns;
}

// For Phase shift methods we use the number of columns
// to determine the "size" of the pattern...
double PSMImplementation::getPatternWidth() {
    return (double) getNumberColumns();
}

Mat PSMImplementation::generatePattern() {
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();

	int iterationIndex = experiment->getIterationIndex();

	int screenWidth = (int)projectorResolution.width;
	int screenHeight = (int)projectorResolution.height;

	int columnWidth = screenWidth / getNumberColumns();

	float offset = -1.6;
	
	Mat pattern(screenHeight, screenWidth, CV_8UC3);

	for (int column = 0; column < getNumberColumns(); column++) {
		int columnX = (column * columnWidth);

		for (int x = columnX; x < (columnX + columnWidth); x++) {
			float theta = ((PSM_TWO_PI / columnWidth) * x) + offset;

			double phaseIntensity;

			switch (iterationIndex) {
				case 0:
					phaseIntensity = ((cos(theta - PSM_TWO_PI_ON_3) + 1.0) / 2.0) * 255.0;
					break;
				case 1:
					phaseIntensity = ((cos(theta + PSM_TWO_PI_ON_3) + 1.0) / 2.0) * 255.0;
					break;
				case 2:
					phaseIntensity = ((cos(theta) + 1.0) / 2.0) * 255.0;
					break;
			}

			line(pattern, Point(x, 0), Point(x, screenHeight - 1), Scalar(phaseIntensity, phaseIntensity, phaseIntensity));
		}
	}

	return pattern;
}

void PSMImplementation::postIterationsProcess() {
	phaseWrap();
	phaseUnwrap();
	makeDepth();
}

/**
 * max(|a-b|,1-|a-b|)
 */
float PSMImplementation::diff(float a, float b) {
	float d = (a < b ? b - a : a - b);
	return (d < 0.5 ? d : 1 - d);
}

float PSMImplementation::min(float a, float b, float c) {
	return (a < b && a < c ? a : (b < c ? b : c));
}

float PSMImplementation::max(float a, float b, float c) {
	return (a > b && a > c ? a : (b > c ? b : c));
}

float PSMImplementation::averageBrightness(int r, int g, int b) {
	return (r + g + b) / (255.0f * 3.0f);
}


/**
 * The function phaseWrap computes the phase of the pattern at each point (x,y),
 * that is its place within the column.
 */
void PSMImplementation::phaseWrap() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	float sqrt3 = sqrt(3);

	Mat phase1Mat = experiment->getCaptureAt(0);
	Mat phase2Mat = experiment->getCaptureAt(1);
	Mat phase3Mat = experiment->getCaptureAt(2);
	
	for (int y = 0; y < cameraResolution.height; y++) {
		for (int x = 0; x < cameraResolution.width; x++) {
			/* Start by getting the intensity of the image at the point for each image*/
			Vec3b phase1PixelBGR = phase1Mat.at<Vec3b>(y, x);
			Vec3b phase2PixelBGR = phase2Mat.at<Vec3b>(y, x);
			Vec3b phase3PixelBGR = phase3Mat.at<Vec3b>(y, x);

			float phase1 = averageBrightness((int)phase1PixelBGR[2], (int)phase1PixelBGR[1], (int)phase1PixelBGR[0]);
			float phase2 = averageBrightness((int)phase2PixelBGR[2], (int)phase2PixelBGR[1], (int)phase2PixelBGR[0]);
			float phase3 = averageBrightness((int)phase3PixelBGR[2], (int)phase3PixelBGR[1], (int)phase3PixelBGR[0]);

			/* Maximum intensity minus minimum intensity */
			float phaseRange = max(phase1, phase2, phase3) - min(phase1, phase2, phase3);

			int arrayOffset = (y * cameraResolution.width) + x;

			if (phaseRange <= PSM_NOISE_THRESHOLD) {
				mask[arrayOffset] = 1;
				ready[arrayOffset] = 0;
			} else {
				mask[arrayOffset] = 0;
				ready[arrayOffset] = 1;
			}

			dist[arrayOffset] = phaseRange;

			phase[arrayOffset] = atan2(sqrt3 * (phase1 - phase3), 2.0f * phase2 - phase1 - phase3) / PSM_TWO_PI;
		}
	}

	for (int y = 1; y < cameraResolution.height - 1; y++) {
		for (int x = 1; x < cameraResolution.width - 1; x++) {
			int arrayOffset = (y * cameraResolution.width) + x;

			if (mask[arrayOffset] == 0) {
				dist[arrayOffset] = (
					diff(phase[arrayOffset], phase[arrayOffset - 1]) +
					diff(phase[arrayOffset], phase[arrayOffset + 1]) +
					diff(phase[arrayOffset], phase[((y - 1) * cameraResolution.width) + x]) +
					diff(phase[arrayOffset], phase[((y + 1) * cameraResolution.width) + x])
				) / dist[arrayOffset];
			}
		}
	}
}

/**
 * Find the location of the point in the projected pattern, based on the phase
 * and its position in relation to the other points (ie, try to identify the column
 * it lies in
 */
void PSMImplementation::phaseUnwrap() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int startX = cameraResolution.width / 2;
	int startY = cameraResolution.height / 2;

	struct WrappedPixel firstWrappedPixel;

	firstWrappedPixel.x = startX;
	firstWrappedPixel.y = startY;
	firstWrappedPixel.dist = 0;
	firstWrappedPixel.phase = phase[(startY * cameraResolution.width) + startX];

	pixelsToProcess->push(firstWrappedPixel);

	while (!pixelsToProcess->empty()) {
		struct WrappedPixel currentPixel = pixelsToProcess->top();
		pixelsToProcess->pop();

		int x = currentPixel.x;
		int y = currentPixel.y;

		int arrayOffset = (y * cameraResolution.width) + x;

		if (ready[arrayOffset] == 1) {
			phase[arrayOffset] = currentPixel.phase;
			ready[arrayOffset] = 0;

			if (y > 0) {
				phaseUnwrap(x, y - 1, currentPixel.dist, currentPixel.phase);
			}
			if (y < cameraResolution.height - 1) {
				phaseUnwrap(x, y + 1, currentPixel.dist, currentPixel.phase);
			}
			if (x > 0) {
				phaseUnwrap(x - 1, y, currentPixel.dist, currentPixel.phase);
			}
			if (x < cameraResolution.width - 1) {
				phaseUnwrap(x + 1, y, currentPixel.dist, currentPixel.phase);
			}
		}
	}
}

void PSMImplementation::phaseUnwrap(int x, int y, float unwrapDist, float unwrapPhase) {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int arrayOffset = (y * cameraResolution.width) + x;

	if (ready[arrayOffset] == 1) {
		float diff = phase[arrayOffset] - (unwrapPhase - (int)unwrapPhase);

		if (diff > 0.5f) {
			diff--;
		}
		if (diff < -0.5f) {
			diff++;
		}
		
		struct WrappedPixel nextWrappedPixel;

		nextWrappedPixel.x = x;
		nextWrappedPixel.y = y;
		nextWrappedPixel.dist = unwrapDist + dist[arrayOffset];
		nextWrappedPixel.phase = unwrapPhase + diff;

		pixelsToProcess->push(nextWrappedPixel);
	}
}

void PSMImplementation::makeDepth() {
	slInfrastructure *infrastructure = experiment->getInfrastructure();
	Size cameraResolution = infrastructure->getCameraResolution();

	for (int y = 0; y < cameraResolution.height; y += PSM_RENDER_DETAIL) {
		for (int x = 0; x < cameraResolution.width; x += PSM_RENDER_DETAIL) {
			int arrayOffset = (y * cameraResolution.width) + x;

			if (mask[arrayOffset] == 0) {
				double xPos = getNumberColumns()/2 - phase[arrayOffset];
				double displacement = experiment->getDisplacement(xPos,x);
				slDepthExperimentResult result(x, y, displacement);
				experiment->storeResult(&result);
			}
		}
	}
}
