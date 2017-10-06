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
        //return experiment->getIterationIndex() < experiment->getInfrastructure()->getCameraResolution().width;
        return experiment->getIterationIndex() < numberColumns;
}

Mat SingleLineImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	double columnWidth = (double)experiment->getInfrastructure()->getCameraResolution().width / (double)numberColumns;
	int columnOffset = (int)((double)experiment->getIterationIndex() * columnWidth);
	//int columnOffset = (int)((double)(experiment->getIterationIndex() * 256) * columnWidth);

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	Mat pattern(cameraHeight, cameraWidth, CV_8UC3, Scalar(SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL, SINGLE_LINE_BLACK_VAL));
	Scalar colour(SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL, SINGLE_LINE_WHITE_VAL);

	rectangle(pattern, Point(columnOffset, 0), Point((columnOffset + ((int)columnWidth - 1)), cameraHeight), colour, FILLED);

	return pattern;
}

/*
void SingleLineImplementation::iterationProcess() {
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	Mat mat = experiment->getLastCapture();

	ofstream out;
	out.open("curves.csv", ios_base::app);

	out << experiment->getIterationIndex();

	for (int lineX = 0; lineX < croppedArea.width; lineX++) {
		Vec3b pixelBGR = mat.at<Vec3b>(0, lineX);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		if (colourTotal > 0) {
			out << "," << colourTotal;
		}
		
	}

	out << endl;
	out.close();
}

void SingleLineImplementation::postIterationsProcess() {
	ofstream temp("temp_1.txt");
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	double cameraWidth = (double)experiment->getInfrastructure()->getCameraResolution().width;

	for (int y = 0; y < croppedArea.height; y++) {
		for (int iteration = 0; iteration < numberColumns; iteration++) {
			double currentOriginalColumn = originalColumn[(y * numberColumns) + iteration];

			if (currentOriginalColumn != -1) {
				double displacement = 
					//((double)x / (double)croppedArea.width) - ((double)currentOriginalColumn / (double)experiment->getInfrastructure()->getCameraResolution().width);
					//((currentOriginalColumn + (double)croppedArea.x) / cameraWidth) - ((double)iteration / (double)numberColumns);
					((currentOriginalColumn / croppedArea.width) - ((double)iteration / (double)numberColumns)) - currentOriginalColumn;
					//((double)x / (double)croppedArea.width) - ((double)currentOriginalColumn / ((double)croppedArea.width / 2.0));
					//(double)(x - currentOriginalColumn);
				
				if (y == 0) {
					temp << "it: " << iteration << " coc: " << currentOriginalColumn << " disp: " << displacement << endl;
				}
			
				if (!isnan(displacement)) {
					slDepthExperimentResult result(currentOriginalColumn, y, displacement * SINGLE_LINE_Z_SCALE);
					experiment->storeResult(&result);
				}

			}
		}
	}
	temp.close();
}
*/
double SingleLineImplementation::solveCorrespondence(int x, int y) {
	//return x;
	Rect croppedArea = experiment->getInfrastructure()->getCroppedArea();
	Mat lineMat = experiment->getCaptureAt(x);
/*
	static double lineTotal = 0.0;
	static double aTotal = 0.0;
*/
	double lineTotal = 0.0;
	double aTotal = 0.0;

/*
	if (x == 0) {
		lineTotal = 0.0;
		aTotal = 0.0;
	}
*/
	for (int lineX = 0; lineX < croppedArea.width; lineX++) {
		Vec3b pixelBGR = lineMat.at<Vec3b>(y, lineX);

		double colourTotal = (double)(pixelBGR[0] + pixelBGR[1] + pixelBGR[2]);

		lineTotal += colourTotal;
		aTotal += ((double)lineX * colourTotal);
/*
		if (y == 0 && colourTotal > 0) {
			out << "," << colourTotal;
		}
*/		
	}

/*
	if (x == 40 && y == 0) {
		out << "aTotal: " << aTotal << " lineTotal: " << lineTotal << " aTotal / lineTotal: " << (aTotal / lineTotal) << endl;
	}
*/
	return aTotal / lineTotal;
}
