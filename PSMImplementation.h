#ifndef PSM_IMPLEMENTATION_H
#define PSM_IMPLEMENTATION_H

#include "slBenchmark.h"
#include <queue>

#define PSM_NOISE_THRESHOLD 0.1
#define PSM_TWO_PI 6.2831853
#define PSM_TWO_PI_ON_3 PSM_TWO_PI / 3.0

#define PSM_RENDER_DETAIL 7

using namespace cv;

struct WrappedPixel {
	int x;
	int y;
	float dist;
	float phase;
};

struct CompareWrappedPixel {
	bool operator()(const WrappedPixel& lhs, const WrappedPixel& rhs) const {
		if (lhs.dist < rhs.dist) {
			return true;
		}

		return false;
	}
};

class PSMImplementation : public slImplementation {
	public:
		PSMImplementation();
		// Constructor to set the number of columns to something else that 32...
		PSMImplementation(unsigned int); 
		virtual ~PSMImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
		bool hasMoreIterations();
		virtual double getPatternWidth();
		virtual Mat generatePattern();
		virtual void postIterationsProcess();
		unsigned int getNumberColumns();

	private:
		float diff(float, float);
		float min(float, float, float);
		float max(float, float, float);
		float averageBrightness(int, int, int);

		void phaseWrap();
		void phaseUnwrap(int, int, float, float);
		void phaseUnwrap();
		void makeDepth();

		unsigned int numberColumns;

		priority_queue<WrappedPixel, vector<WrappedPixel>, CompareWrappedPixel> *pixelsToProcess;

		float *phase;
		float *dist;
		int *mask;
		int *ready;
		int *names;
};

#endif //PSM_IMPLEMENTATION_H
