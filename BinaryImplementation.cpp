#include "BinaryImplementation.h"

BinaryImplementation::BinaryImplementation(): slImplementation(string("BinaryImplementation")) {
        //numberPatterns = 6;
        numberPatterns = 8;
        Black_Value = 0;
	//White_Value = 195;
        White_Value = 255;
        Black_Threshold = -50;
        White_Threshold = 50;
}

void BinaryImplementation::preExperimentRun() {
	numberColumns = 1;

	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int arraySize = cameraResolution.width * cameraResolution.height;

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

unsigned int BinaryImplementation::getNumberPatterns() {
    return this->numberPatterns;
}

unsigned int BinaryImplementation::getNumberColumns() {
    return this->numberColumns;
}

double BinaryImplementation::getBinaryCode(int xProjector, int y) {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
//	return this->binaryCode[(y * cameraResolution.width) + xProjector];

	for (int x = 0; x < cameraResolution.width; x++) {
		int binaryXProjector = binaryCode[(y * cameraResolution.width) + x];

		if (binaryXProjector == xProjector) {
			return (double)x;
		}
	}

	return -1;
}

void BinaryImplementation::postExperimentRun() {
	delete[] binaryCode;
}

bool BinaryImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < getNumberPatterns() * 2;
}

int BinaryImplementation::guessColour(int colourDifference) {
        if (colourDifference < Black_Threshold) {
        return 1;
        } else if (colourDifference > White_Threshold) {
        return 0; }

        return -1;
}

void BinaryImplementation::generateBackground(Mat &pattern, Scalar &colour) {
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();

	int iterationIndex = experiment->getIterationIndex();

	int projectorWidth = (int)projectorResolution.width;
	int projectorHeight = (int)projectorResolution.height;

	pattern.create(projectorHeight, projectorWidth, CV_8UC3);

	if (iterationIndex % 2 == 0) {
		numberColumns *= 2;
		pattern.setTo(Scalar(White_Value, White_Value, White_Value));
		colour = Scalar(Black_Value, Black_Value, Black_Value);
	} else {
		pattern.setTo(Scalar(Black_Value, Black_Value, Black_Value));
		colour = Scalar(White_Value, White_Value, White_Value);
	}
}

Mat BinaryImplementation::generatePattern() {
	Mat pattern;
	Scalar colour;

	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();
	int projectorWidth = (int)projectorResolution.width;
	int projectorHeight = (int)projectorResolution.height;

	generateBackground(pattern,colour);

	int width = projectorWidth / numberColumns;

	for (int w = width; w < projectorWidth; w += (2 * width)) {
		rectangle(pattern, Point(w, 0), Point((w + width) - 1, projectorHeight), colour, FILLED);
	}

	return pattern;
}

void BinaryImplementation::iterationProcess() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	if (experiment->getIterationIndex() % 2 != 0) {
		Mat positiveMat = experiment->getCaptureAt(experiment->getNumberCaptures() - 2);
		Mat negativeMat = experiment->getLastCapture();

		for (int y = 0; y < cameraResolution.height; y++) {
			for (int x = 0; x < cameraResolution.width; x++) {
				Vec3b positivePixelBGR = positiveMat.at<Vec3b>(y, x);
				Vec3b negativePixelBGR = negativeMat.at<Vec3b>(y, x);

				int positiveColourTotal = (int)positivePixelBGR[0] + (int)positivePixelBGR[1] + (int)positivePixelBGR[2];
				int negativeColourTotal = (int)negativePixelBGR[0] + (int)negativePixelBGR[1] + (int)negativePixelBGR[2];

				int colourDifference = guessColour(positiveColourTotal - negativeColourTotal);

				int arrayOffset = (y * cameraResolution.width) + x;

				binaryCode[arrayOffset] <<= 1;
				if(colourDifference == -1) binaryCode[arrayOffset] = -1;
				else binaryCode[arrayOffset] += colourDifference;
			}
		}	
	}
}
/*
void BinaryImplementation::processCaptures() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int lookupArraySize = cameraResolution.height * numColumns;
	int arraySize = cameraResolution.width * cameraResolution.height;
	
	projectorColumnLookup = new int[lookupArraySize];

	for (int index = 0; index < lookupArraySize; index++) {
		projectorColumnLookup[index] = -1;
	}

	for (int index = 0; index < arraySize; index++) {
		
	}
}
*/
double BinaryImplementation::solveCorrespondence(int xProjector, int y) {
	static double lastBinaryCode = -1;

	if (xProjector == 0) {
		lastBinaryCode = -1;
	}

	double currentBinaryCode = getBinaryCode(xProjector, y);

	if (currentBinaryCode != -1 && currentBinaryCode != lastBinaryCode) {
		lastBinaryCode = currentBinaryCode;

		return currentBinaryCode;
	}

	return -1;
}	
