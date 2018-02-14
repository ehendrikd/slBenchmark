#ifndef SINGLE_LINE_IMPLEMENTATION_H
#define SINGLE_LINE_IMPLEMENTATION_H

#include "slBenchmark.h"

#define SINGLE_LINE_BLACK_VAL 0
#define SINGLE_LINE_WHITE_VAL 255

//#define SINGLE_LINE_BLACK_THRESHOLD 384
#define SINGLE_LINE_BLACK_THRESHOLD 200

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
