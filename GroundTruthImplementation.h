#ifndef GROUND_TRUTH_IMPLEMENTATION_H
#define GROUND_TRUTH_IMPLEMENTATION_H

#include "slBenchmark.h"

#define GROUND_TRUTH_BLACK_VAL 0
#define GROUND_TRUTH_WHITE_VAL 255

#define GROUND_TRUTH_WHITE_THRESHOLD 50

using namespace cv;

class GroundTruthImplementation : public slImplementation {
	public:
		GroundTruthImplementation();
		virtual ~GroundTruthImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
		bool hasMoreIterations();
		virtual Mat generatePattern();
		virtual void iterationProcess();
		virtual void postIterationsProcess();

	protected:
		int *originalColumn;
};

#endif //GROUND_TRUTH_IMPLEMENTATION_H
