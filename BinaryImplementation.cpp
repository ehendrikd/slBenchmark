#include "BinaryImplementation.h"

BinaryImplementation::BinaryImplementation(): slImplementation(string("BinaryImplementation")) {
}

void BinaryImplementation::preExperimentRun() {
	numberColumns = 1;

	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	int arraySize = croppedArea.width * croppedArea.height;

	binaryCode = new int[arraySize];

	for (int index = 0; index < arraySize; index++) {
		binaryCode[index] = 0;
	}
}

// For binary implementations, the "width" of the pattern
// is the number of columns.
double BinaryImplementation::getPatternWidth() {
	return (double) this->getNumberColumns();
}

unsigned int BinaryImplementation::getNumberColumns() {
    return this->numberColumns;
}

void BinaryImplementation::postExperimentRun() {
	delete[] binaryCode;
}

bool BinaryImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < BINARY_NUM_PATTERNS * 2;
}

Mat BinaryImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int iterationIndex = experiment->getIterationIndex();

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	Mat pattern(cameraHeight, cameraWidth, CV_8UC3, Scalar(BINARY_WHITE_VAL, BINARY_WHITE_VAL, BINARY_WHITE_VAL));
	Scalar colour(BINARY_BLACK_VAL, BINARY_BLACK_VAL, BINARY_BLACK_VAL);

	if (iterationIndex % 2 == 0) {
		numberColumns *= 2;
	} else {
		pattern.setTo(Scalar(BINARY_BLACK_VAL, BINARY_BLACK_VAL, BINARY_BLACK_VAL));
		colour = Scalar(BINARY_WHITE_VAL, BINARY_WHITE_VAL, BINARY_WHITE_VAL);
	}

	int width = cameraWidth / numberColumns;

	for (int w = width; w < cameraWidth; w += (2 * width)) {
		rectangle(pattern, Point(w, 0), Point((w + width) - 1, cameraHeight), colour, FILLED);
	}

	return pattern;
}

void BinaryImplementation::iterationProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	if (experiment->getIterationIndex() % 2 != 0) {
		Mat positiveMat = experiment->getCaptureAt(experiment->getNumberCaptures() - 2);
		Mat negativeMat = experiment->getLastCapture();

		for (int y = 0; y < croppedArea.height; y++) {
			for (int x = 0; x < croppedArea.width; x++) {
				Vec3b positivePixelBGR = positiveMat.at<Vec3b>(y, x);
				Vec3b negativePixelBGR = negativeMat.at<Vec3b>(y, x);

				int positiveColourTotal = (int)positivePixelBGR[0] + (int)positivePixelBGR[1] + (int)positivePixelBGR[2];
				int negativeColourTotal = (int)negativePixelBGR[0] + (int)negativePixelBGR[1] + (int)negativePixelBGR[2];

				int colourDifference = positiveColourTotal - negativeColourTotal;

				int arrayOffset = (y * croppedArea.width) + x;

				binaryCode[arrayOffset] <<= 1;

				if (colourDifference < BINARY_BLACK_THRESHOLD) {
					binaryCode[arrayOffset] += 1;
				} else if (colourDifference > BINARY_WHITE_THRESHOLD) {
					binaryCode[arrayOffset] += 0;
                                } else {
					binaryCode[arrayOffset] = -1;
                                }			
			}
		}	
	}
}

void BinaryImplementation::postIterationsProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	for (int y = 0; y < croppedArea.height; y++) {
		int lastBinaryCode = -1;

		for (int x = 0; x < croppedArea.width; x++) {
			int currentBinaryCode = binaryCode[(y * croppedArea.width) + x];

			if (currentBinaryCode != -1 && currentBinaryCode != lastBinaryCode) {
				
				double displacement = getDisplacement(currentBinaryCode,x);
				slDepthExperimentResult result(x, y, displacement * this->getScale());
				experiment->storeResult(&result);

				lastBinaryCode = currentBinaryCode;
			}
		}
	}
}
