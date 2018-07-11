#include "SingleLineImplementation.h"

SingleLineImplementation::SingleLineImplementation(int newNumberColumns): slImplementation(string("SingleLineImplementation")), numberColumns(newNumberColumns) {
}

bool SingleLineImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < numberColumns;
}

double SingleLineImplementation::getPatternWidth() {
	return (double)numberColumns;
}

Mat SingleLineImplementation::generatePattern() {
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();

	double columnWidth = (double)projectorResolution.width / (double)numberColumns;
	int columnOffset = (int)((double)experiment->getIterationIndex() * columnWidth);

	int projectorWidth = (int)projectorResolution.width;
	int projectorHeight = (int)projectorResolution.height;

	Mat pattern(projectorHeight, projectorWidth, CV_8UC3, Scalar(SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL));
	Scalar colour(SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL);

	//rectangle(pattern, Point(columnOffset, 0), Point((columnOffset + ((int)columnWidth - 1)), projectorHeight), colour, FILLED);
	rectangle(pattern, Point(columnOffset, 0), Point(columnOffset, projectorHeight), colour, FILLED);

	return pattern;
}

void SingleLineImplementation::processCapture(Mat captureMat) {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();

	int xPattern = experiment->getIterationIndex();

	for (int y = 0; y < cameraResolution.height; y++) {
		int columnMax = 0;
//		int foundColumn = -1;
		int xColumn = -1;	
		double xCamera = -1.0;

		for (int column = 0; column < cameraResolution.width; column++) {
			Vec3b pixelBGR = captureMat.at<Vec3b>(y, column);

			int colourTotal = pixelBGR[0] + pixelBGR[1] + pixelBGR[2];

			if (colourTotal > columnMax) {
				columnMax = colourTotal;
				xColumn = column;
			}
/*	
			if (colourTotal >= SINGLE_LINE_BLACK_THRESHOLD && colourTotal > columnMax) {
				columnMax = colourTotal;

				double prevColourTotal = 0.0;
				double nextColourTotal = 0.0;

				if (column > 0) {
					Vec3b prevPixelBGR = captureMat.at<Vec3b>(y, column - 1);
					prevColourTotal = (double)(prevPixelBGR[0] + prevPixelBGR[1] + prevPixelBGR[2]);
				}

				if (column < (cameraResolution.width - 1)) {
					Vec3b nextPixelBGR = captureMat.at<Vec3b>(y, column + 1);
					nextColourTotal = (double)(nextPixelBGR[0] + nextPixelBGR[1] + nextPixelBGR[2]);
				}

				xCamera = (double)column;

				if (y == 360) {
					DB("xPattern: " << xPattern << " column: " << column << " p: " << prevColourTotal << " c: " << colourTotal << " n: " << nextColourTotal)
				}	

				break;
			}
*/

		}


		if (columnMax >= SINGLE_LINE_BLACK_THRESHOLD) {
			xCamera = (double)xColumn;
	
			int prevColourTotal = 0;
			int nextColourTotal = 0;

			if (xColumn > 0) {
				Vec3b prevPixelBGR = captureMat.at<Vec3b>(y, xColumn - 1);
				prevColourTotal = prevPixelBGR[0] + prevPixelBGR[1] + prevPixelBGR[2];
			}

			if (xColumn < (cameraResolution.width - 1)) {
				Vec3b nextPixelBGR = captureMat.at<Vec3b>(y, xColumn + 1);
				nextColourTotal = nextPixelBGR[0] + nextPixelBGR[1] + nextPixelBGR[2];
			}

			//xCamera = (double)xColumn;	

			if (prevColourTotal != 0 && nextColourTotal != 0) {
				int sidesTotal = prevColourTotal + nextColourTotal;
				xCamera -= 0.5 + ((double)nextColourTotal / (double)sidesTotal);
/*
				double offset = (double)nextColourTotal / (double)sidesTotal;

				if (fabs(0.5 - offset) <= SINGLE_LINE_PIXEL_THRESHOLD) {
					xCamera = (double)xColumn;						
				}
*/
			}
			
/*
			if (y == 360) {
				//DB("xPattern: " << xPattern << " xCamera: " << xCamera << " p: " << prevColourTotal << " c: " << columnMax << " n: " << nextColourTotal)
				DB("xPattern," << xPattern << "," << xCamera)
			}
*/
	
		}


		if (!isnan(xCamera) && xCamera != -1) {				
			double displacement = experiment->getDisplacement(xPattern, xCamera);
			int xProjector = (int)(experiment->getImplementation()->getPatternXOffsetFactor(xPattern) * projectorResolution.width);

			if (!isinf(displacement)) {
				slDepthExperimentResult result(xProjector, y, displacement);
				experiment->storeResult(&result);
			}
		}
	}
}
/*
double SingleLineImplementation::solveCorrespondence(int xProjector, int y) {
	Mat lineMat = experiment->getCaptureAt(xProjector);
	int cameraWidth = experiment->getInfrastructure()->getCameraResolution().width;

	double columnMax = 0.0;
	int foundColumn = -1;

	for (int column = 0; column < cameraWidth; column++) {
		Vec3b pixelBGR = lineMat.at<Vec3b>(y, column);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		if (colourTotal >= SINGLE_LINE_BLACK_THRESHOLD && colourTotal > columnMax) {
			columnMax = colourTotal;
			foundColumn = column;
		}

	}

	return (double)foundColumn;
}
*/
/*
double SingleLineImplementation::solveCorrespondence(int xProjector, int y) {
	Mat lineMat = experiment->getCaptureAt(xProjector);
	int cameraWidth = experiment->getInfrastructure()->getCameraResolution().width;

	double lineTotal = 0.0;
	double aTotal = 0.0;

	for (int column = 0; column < cameraWidth; column++) {
		Vec3b pixelBGR = lineMat.at<Vec3b>(y, column);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		if (colourTotal < SINGLE_LINE_BLACK_THRESHOLD) {
			colourTotal = 0;
		}

		lineTotal += colourTotal;
		aTotal += ((double)column * colourTotal);
	}
	
	return aTotal / lineTotal;
}
*/
