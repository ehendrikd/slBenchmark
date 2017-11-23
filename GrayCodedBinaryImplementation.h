#ifndef GRAY_CODED_BINARY_IMPLEMENTATION_H
#define GRAY_CODED_BINARY_IMPLEMENTATION_H

#include "BinaryImplementation.h"

class GrayCodedBinaryImplementation : public BinaryImplementation {
	public:
		GrayCodedBinaryImplementation(int);
		virtual ~GrayCodedBinaryImplementation() {};
		virtual Mat generatePattern();
		//virtual void iterationProcess();
		//virtual void postIterationsProcess();
		virtual double getBinaryCode(int, int);
		int convertGrayCodeToInteger(int, int);
};

#endif //GRAY_CODED_BINARY_IMPLEMENTATION_H
