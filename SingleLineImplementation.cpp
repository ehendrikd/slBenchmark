#include "SingleLineImplementation.h"

SingleLineImplementation::SingleLineImplementation(int newNumberColumns): slImplementation(string("SingleLineImplementation")), numberColumns(newNumberColumns) {
	interlines = 1;
}

void SingleLineImplementation::preExperimentRun() {
	// We reconstruct a virtual pattern that is numberColumns wide (our maximal reconstructible x-accuracy) and cameraResolution.height high (the maximum reconstructible y-accuracy).
	int arraySize = experiment->getInfrastructure()->getCameraResolution().height * numberColumns;

	originalColumn = new double[arraySize];

	for (int index = 0; index < arraySize; index++) {
		originalColumn[index] = -1.0;
	}
	
}

void SingleLineImplementation::postExperimentRun() {
	delete[] originalColumn;
}

bool SingleLineImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < numberColumns / interlines+1;
}

Mat SingleLineImplementation::generatePattern() {
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();

	double columnWidth = (double)projectorResolution.width / (double)numberColumns;
	int columnOffset = (int)((double)experiment->getIterationIndex() * interlines * columnWidth);

	int projectorWidth = (int)projectorResolution.width;
	int projectorHeight = (int)projectorResolution.height;

	Mat pattern(projectorHeight, projectorWidth, CV_8UC3, Scalar(SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL));
	Scalar colour(SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL);

	rectangle(pattern, Point(columnOffset, 0), Point((columnOffset + ((int)columnWidth - 1)), projectorHeight), colour, FILLED);

	return pattern;
}

double SingleLineImplementation::solveCorrespondence(int x, int y) {
	if (x%interlines != 0) return -1;
	Mat lineMat = experiment->getCaptureAt(x/interlines);

	double lineTotal = 0.0;
	double aTotal = 0.0;

	for (int column = 0; column < numberColumns; column++) {
		Vec3b pixelBGR = lineMat.at<Vec3b>(y, column);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		lineTotal += colourTotal;
		aTotal += ((double)column * colourTotal);
	}

	return aTotal / lineTotal;
}

