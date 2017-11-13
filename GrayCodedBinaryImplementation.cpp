#include "GrayCodedBinaryImplementation.h"

GrayCodedBinaryImplementation::GrayCodedBinaryImplementation() {
	setIdentifier(string("GrayCodedBinaryImplementation"));
}

Mat GrayCodedBinaryImplementation::generatePattern() {
	Mat pattern;
	Scalar colour;
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();
	int projectorWidth = (int)projectorResolution.width;
	int projectorHeight = (int)projectorResolution.height;

	generateBackground(pattern,colour);

	int columnWidth = projectorWidth / numberColumns;
	int doubleColumnWidth = columnWidth * 2;
	int xPos = 0;

	for (int columnIndex = 0; columnIndex < numberColumns; columnIndex++) {
		if (columnIndex % 4 == 1) {
			rectangle(pattern, Point(xPos, 0), Point((xPos + doubleColumnWidth) - 1, projectorHeight), colour, FILLED);
		}
		xPos += columnWidth;
	}

	return pattern;
}

// Getters and Setters
double GrayCodedBinaryImplementation::getBinaryCode(int xProjector, int y) {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
/*	
	int binCode = this->binaryCode[(y * cameraResolution.width) + xProjector];
	if(binCode == -1) return -1;
	return convertGrayCodeToInteger(binCode, numberColumns, getNumberPatterns());
*/
	int columnWidth = cameraResolution.width / numberColumns;


	for (int x = 0; x < cameraResolution.width; x++) {
		int binaryXProjector = convertGrayCodeToInteger(binaryCode[(y * cameraResolution.width) + x], numberColumns, getNumberPatterns()) * columnWidth;

		if (binaryXProjector == xProjector) {
			return (double)x;
		}
	}

	return -1;

}


/* FOR ADAPTIVE!!!!
void GrayCodedBinaryImplementation::iterationProcess() {
	if (benchmark->getIterationIndex() % 2 != 0) {

		Mat positiveMat = getCaptureAt(getNumberCaptures() - 2);
		Mat negativeMat = getLastCapture();

		for (int y = 0; y < benchmark->getCameraHeight(); y++) {
			int previousColumnEdgeIndex = -1;

			for (int x = 0; x < benchmark->getCameraWidth(); x++) {
				Vec3b positivePixelBGR = positiveMat.at<Vec3b>(y, x);
				Vec3b negativePixelBGR = negativeMat.at<Vec3b>(y, x);

				int positiveColourTotal = (int)positivePixelBGR[0] + (int)positivePixelBGR[1] + (int)positivePixelBGR[2];
				int negativeColourTotal = (int)negativePixelBGR[0] + (int)negativePixelBGR[1] + (int)negativePixelBGR[2];

				int colourDifference = positiveColourTotal - negativeColourTotal;

				int arrayOffset = (y * benchmark->getCameraWidth()) + x;

				binaryCode[arrayOffset] <<= 1;

				if (colourDifference < BLACK_THRESHOLD) {
					binaryCode[arrayOffset] += 1;
				} else if (colourDifference > WHITE_THRESHOLD) {
					binaryCode[arrayOffset] += 0;
                                } else {
					binaryCode[arrayOffset] = -1;
                                }			

				if (binaryCode[arrayOffset] != -1) {	
					int columnEdgeIndex = convertBinaryToInteger(binaryCode[arrayOffset], numberColumns, NUM_PATTERNS);

					if (columnEdgeIndex == previousColumnEdgeIndex + 1) {
					}

					previousColumnEgdeIndex = columnEdgeIndex;
				}

			}			
		}	
	}
}
*/

int GrayCodedBinaryImplementation::convertGrayCodeToInteger(int grayCodeToConvert, int numberColumns, int numberPatterns) {
	int result = grayCodeToConvert;

	for (int iteration = numberColumns; iteration > 0; iteration /= 2) {
		if (iteration < numberPatterns) {
			result = result ^ (result >> iteration);
		}
	}

	return result;
}
