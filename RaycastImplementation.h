#ifndef RAYCAST_IMPLEMENTATION_H
#define RAYCAST_IMPLEMENTATION_H

#include "slBenchmark.h"

using namespace cv;

class RaycastImplementation : public slImplementation {
	public:
		RaycastImplementation(int);
		virtual ~RaycastImplementation() {};
		virtual double getPatternWidth();
		virtual Mat generatePattern();
		virtual void postIterationsProcess();
		bool hasMoreIterations();
		virtual double solveCorrespondence(int, int) {return 0;}
	private:
		int width;
};

#endif //RAYCAST_IMPLEMENTATION_H
