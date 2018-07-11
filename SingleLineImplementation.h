#ifndef SINGLE_LINE_IMPLEMENTATION_H
#define SINGLE_LINE_IMPLEMENTATION_H

#include "slBenchmark.h"

#define SINGLE_LINE_BLACK_VAL 0
#define SINGLE_LINE_WHITE_VAL 255

//#define SINGLE_LINE_BLACK_THRESHOLD 384
#define SINGLE_LINE_BLACK_THRESHOLD 200
#define SINGLE_LINE_PIXEL_THRESHOLD 0.1

using namespace cv;

class SingleLineImplementation : public slImplementation {
	public:
		SingleLineImplementation(int);
		virtual ~SingleLineImplementation() {};
		bool hasMoreIterations();
		virtual double getPatternWidth();
		virtual Mat generatePattern();
		virtual void processCapture(Mat);
		virtual void postIterationsProcess() {};
		//virtual double solveCorrespondence(int, int);

	protected:
		int numberColumns;
		double *originalColumn;
};

#endif //SINGLE_LINE_IMPLEMENTATION_H
