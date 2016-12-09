#include "GroundTruthImplementation.h"

GroundTruthImplementation::GroundTruthImplementation(): slImplementation(string("GroundTruthImplementation")) {
}

void GroundTruthImplementation::preExperimentRun() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	int arraySize = croppedArea.width * croppedArea.height;

	originalColumn = new int[arraySize];

	for (int index = 0; index < arraySize; index++) {
		originalColumn[index] = -1;
	}
}

void GroundTruthImplementation::postExperimentRun() {
	delete[] originalColumn;
}

bool GroundTruthImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < experiment->getInfrastructure()->getCameraResolution().width;
//     return experiment->getIterationIndex() < 1;   
}

Mat GroundTruthImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int iterationIndex = experiment->getIterationIndex();

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	Mat pattern(cameraHeight, cameraWidth, CV_8UC3, Scalar(GROUND_TRUTH_BLACK_VAL, GROUND_TRUTH_BLACK_VAL, GROUND_TRUTH_BLACK_VAL));
	Scalar colour(GROUND_TRUTH_WHITE_VAL, GROUND_TRUTH_WHITE_VAL, GROUND_TRUTH_WHITE_VAL);

	rectangle(pattern, Point(iterationIndex, 0), Point((iterationIndex + 1), cameraHeight), colour, FILLED);

	return pattern;
}

void GroundTruthImplementation::iterationProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	Mat mat = experiment->getLastCapture();

	int iterationIndex = experiment->getIterationIndex();

	for (int y = 0; y < croppedArea.height; y++) {
		for (int x = 0; x < croppedArea.width; x++) {
			Vec3b pixelBGR = mat.at<Vec3b>(y, x);

			int colourTotal = (int)pixelBGR[0] + (int)pixelBGR[1] + (int)pixelBGR[2];

			int arrayOffset = (y * croppedArea.width) + x;

			if (colourTotal > GROUND_TRUTH_WHITE_THRESHOLD) {
				originalColumn[arrayOffset] = iterationIndex;
			}			
		}
	}	
}

void GroundTruthImplementation::postIterationsProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();

	for (int y = 0; y < croppedArea.height; y++) {
		//int lastBinaryCode = -1;

		for (int x = 0; x < croppedArea.width; x++) {
			int currentOriginalColumn = originalColumn[(y * croppedArea.width) + x];

			//if (currentBinaryCode != -1 && currentBinaryCode != lastBinaryCode) {
			if (currentOriginalColumn != -1) {
				
				double displacement = 
					((double)x / (double)croppedArea.width) - ((double)currentOriginalColumn / (double)experiment->getInfrastructure()->getCameraResolution().width);

				slDepthExperimentResult result(x, y, displacement * GROUND_TRUTH_Z_SCALE);
				experiment->storeResult(&result);

				//lastBinaryCode = currentBinaryCode;
			}
		}
	}
}
