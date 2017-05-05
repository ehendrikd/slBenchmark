#include "BinaryImplementation.h"

BinaryImplementation::BinaryImplementation(): slImplementation(string("BinaryImplementation")) {
        numberPatterns = 6;
        Black_Value = 0;
        White_Value = 195;
        Black_Threshold = -50;
        White_Threshold = 50;
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

unsigned int BinaryImplementation::getNumberPatterns() {
    return this->numberPatterns;
}

unsigned int BinaryImplementation::getNumberColumns() {
    return this->numberColumns;
}

int BinaryImplementation::getBinaryCode(int x, int y) {
    Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
    return this->binaryCode[(y * croppedArea.width) + x];
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
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int iterationIndex = experiment->getIterationIndex();

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	pattern.create(cameraHeight, cameraWidth, CV_8UC3);
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

	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	generateBackground(pattern,colour);

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

				int colourDifference = guessColour(positiveColourTotal - negativeColourTotal);

				int arrayOffset = (y * croppedArea.width) + x;

				binaryCode[arrayOffset] <<= 1;
				if(colourDifference == -1) binaryCode[arrayOffset] = -1;
				else binaryCode[arrayOffset] += colourDifference;
			}
		}	
	}
}

void BinaryImplementation::postIterationsProcess() {
        Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	for (int y = 0; y < croppedArea.height; y++) {
		int lastBinaryCode = -1;

		for (int x = 0; x < croppedArea.width; x++) {
			int currentBinaryCode = getBinaryCode(x,y);

			if (currentBinaryCode != -1 && currentBinaryCode != lastBinaryCode) {
				
				double displacement = getDisplacement((double)currentBinaryCode,x);
				slDepthExperimentResult result(x, y, displacement * this->getScale());
				experiment->storeResult(&result);

				lastBinaryCode = currentBinaryCode;
			}
		}
	}
}
