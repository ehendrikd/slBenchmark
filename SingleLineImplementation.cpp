#include "SingleLineImplementation.h"

SingleLineImplementation::SingleLineImplementation(int newNumberColumns): slImplementation(string("SingleLineImplementation")), numberColumns(newNumberColumns) {
}

void SingleLineImplementation::preExperimentRun() {
	int arraySize = experiment->getInfrastructure()->getCroppedArea().height * numberColumns;

	originalColumn = new double[arraySize];

	for (int index = 0; index < arraySize; index++) {
		originalColumn[index] = -1.0;
	}
	
}

void SingleLineImplementation::postExperimentRun() {
	delete[] originalColumn;
}

bool SingleLineImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < numberColumns;
}

Mat SingleLineImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	double columnWidth = (double)experiment->getInfrastructure()->getCameraResolution().width / (double)numberColumns;
	int columnOffset = (int)((double)experiment->getIterationIndex() * columnWidth);

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	Mat pattern(cameraHeight, cameraWidth, CV_8UC3, Scalar(SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL));
	Scalar colour(SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL);

	rectangle(pattern, Point(columnOffset, 0), Point((columnOffset + ((int)columnWidth - 1)), cameraHeight), colour, FILLED);

	return pattern;
}

double SingleLineImplementation::solveCorrespondence(int x, int y) {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	Mat lineMat = experiment->getCaptureAt(x);

	double lineTotal = 0.0;
	double aTotal = 0.0;

	for (int lineX = 0; lineX < croppedArea.width; lineX++) {
		Vec3b pixelBGR = lineMat.at<Vec3b>(y, lineX);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		lineTotal += colourTotal;
		aTotal += ((double)lineX * colourTotal);
	}

	return aTotal / lineTotal;
}
