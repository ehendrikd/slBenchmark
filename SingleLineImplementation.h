#ifndef SINGLE_LINE_IMPLEMENTATION_H
#define SINGLE_LINE_IMPLEMENTATION_H

#include "slBenchmark.h"

#define SINGLE_LINE_BLACK_VAL 0
#define SINGLE_LINE_WHITE_VAL 255

//#define SINGLE_LINE_WHITE_THRESHOLD 50
#define SINGLE_LINE_WHITE_THRESHOLD 250

//#define SINGLE_LINE_Z_SCALE 1000
//#define SINGLE_LINE_Z_SCALE 9000
//#define SINGLE_LINE_Z_SCALE 9544.935210908354
#define SINGLE_LINE_Z_SCALE 19089.870421816708
//#define SINGLE_LINE_Z_SCALE 10000

using namespace cv;

class SingleLineImplementation : public slImplementation {
	public:
		SingleLineImplementation(int);
		virtual ~SingleLineImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
		bool hasMoreIterations();
		virtual double getPatternWidth();
		virtual Mat generatePattern();
		//virtual void iterationProcess();
		//virtual void postIterationsProcess();
		virtual double solveCorrespondence(int, int);

	protected:
		int numberColumns;
		double *originalColumn;
		int interlines;
};

#endif //SINGLE_LINE_IMPLEMENTATION_H
