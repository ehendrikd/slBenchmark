#ifndef BINARY_IMPLEMENTATION_H
#define BINARY_IMPLEMENTATION_H

#include "slBenchmark.h"

using namespace cv;

enum backgroundType {White,Black};

class BinaryImplementation : public slImplementation {
	public:
		BinaryImplementation(int);
		virtual ~BinaryImplementation() {};
		void preExperimentRun();
		void postExperimentRun();
	        virtual double getPatternWidth();
		bool hasMoreIterations();
		virtual Mat generatePattern();
		virtual void iterationProcess();
		//virtual void postIterationsProcess();
		//Getters and Setters
		virtual double getBinaryCode(int, int);
		int getNumberPatterns();

                // The next function determine whether a colour can be
                // considered to be white or black. This is determined by 
                // the contrast between the first and the second captures:
                // if the contrast is above a certain threshold (Black_Threshold),
                // then the colour is considered black. If the contrast is below a 
                // certain threshold (White_Threshold) it is considered white. 
                // If it is between the two threshold it is uncertain.
		int guessColour(int);

		void generateBackground(Mat &pattern, Scalar &colour);

		virtual double solveCorrespondence(int, int);

	protected:
		unsigned int currentNumberColumns;
		unsigned int numberColumns;
		// The patterns for are bicolour, typically
		// black and white. In reality, they can be
		// shades of gray to avoid reflection of light
		// This is specified by the following two attributes.
		unsigned short Black_Value;
		unsigned short White_Value;
		// The thresholds for black and white values
		short Black_Threshold;
		short White_Threshold;
		// An array containing the codes for each column
		int *binaryCode;
};

#endif //BINARY_IMPLEMENTATION_H
