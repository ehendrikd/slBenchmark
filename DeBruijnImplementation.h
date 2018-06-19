#ifndef DEBRUIJN_IMPLEMENTATION_H
#define DEBRUIJN_IMPLEMENTATION_H

#include "slBenchmark.h"

#define DEBRUIJN_K 5
#define DEBRUIJN_N 3

#define DEBRUIJN_ALPHA 0.2
#define DEBRUIJN_BETA 0.8
//#define DEBRUIJN_ALPHA -1.0
//#define DEBRUIJN_BETA 2.0

//#define DEBRUIJN_THRESHOLD 5000
#define DEBRUIJN_THRESHOLD 200

using namespace cv;

typedef struct {
	double score;
	int numberItems;
	int caseSigma;
} pairScore;

class DeBruijnImplementation : public slImplementation {
	public:
		DeBruijnImplementation(unsigned int);
		virtual ~DeBruijnImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
		virtual double getPatternWidth();
		bool hasMoreIterations();
		virtual Mat generatePattern();
		void calculateCroppedCapture();
		virtual void processCapture(Mat);
		virtual void postIterationsProcess();
		
		unsigned int getNumberColumns();
		unsigned int getNumberEdges();

	protected:
		void db(int, int, int, int, vector<int> &, vector<int> &);
		double clamp(double, double, double);
		double consistency(int, int);
		double score(Vec3s, Vec3s);
		double sigma(int, int, Vec3s *, Vec3s *, pairScore **);
		void populateCorrespondences(int, int, int[][2], pairScore **);
		Vec3s *transitions;
	private:
    		double numberEdges;
};

#endif //DEBRUIJN_IMPLEMENTATION_H
