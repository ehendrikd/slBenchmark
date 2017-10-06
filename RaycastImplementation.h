#ifndef RAYCAST_IMPLEMENTATION_H
#define RAYCAST_IMPLEMENTATION_H

#include "slBenchmark.h"

using namespace cv;

class RaycastImplementation : public slImplementation {
	public:
		RaycastImplementation();
		virtual ~RaycastImplementation() {};
		virtual Mat generatePattern();
		virtual void postIterationsProcess();
		bool hasMoreIterations();
		virtual double solveCorrespondence(int, int) {return 0;}
};

#endif //RAYCAST_IMPLEMENTATION_H
