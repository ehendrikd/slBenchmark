/*
 * File: fileInfrastructure.cpp
 * 
 * Copyright 2016 Evan Dekker
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description:
 *
 * This file implements classes slBenchmark and slImplementation. The
 * slBenchmark class conducts the structured light scanning tests
 * and stores various benchmarking information. The slImplementation
 * class allows a custom structured light implementation to be
 * tested.
 */ 
#include "slFileInfrastructure.h"

//Cross platform mkdir
#ifdef _WIN32
#include <direct.h>
#endif


/*
 * slFileInfrastructure
 */ 

//Create a file infrastructure instance
slFileInfrastructure::slFileInfrastructure(string dirname, double newCameraHorizontalFOV, double newCameraVerticalFOV, double newProjectorHorizontalFOV, double newProjectorVerticalFOV, double newCameraProjectorSeparation) : slInfrastructure(string("slFileInfrastructure")) {
	setCameraHorizontalFOV(newCameraHorizontalFOV);
	setCameraVerticalFOV(newCameraVerticalFOV);
	setProjectorHorizontalFOV(newProjectorHorizontalFOV);
	setProjectorVerticalFOV(newProjectorVerticalFOV);
	setCameraProjectorSeparation(newCameraProjectorSeparation);

	this->dirname << "." << OS_SEP << dirname; /* For now only take a relative path. Should be made more robust. */
	if(dirname[dirname.size()-1] != OS_SEP) /* Append / if necessary */
		this->dirname << OS_SEP; 
	DB("Directory name:" << this->dirname.str());
}

//Project the structured light implementation pattern and capture it
Mat slFileInfrastructure::projectAndCapture(Mat patternMat) {
	DB("-> slFileInfrastructure::projectAndCapture()")
			
	stringstream captureFilename;

	captureFilename << this->dirname.str() << "capture_" << experiment->getIterationIndex() << ".png" ;
	DB("reading file " << captureFilename.str().c_str());
	Mat captureMat = imread(captureFilename.str().c_str());
	DB("<- slFileInfrastructure::projectAndCapture()")

	return captureMat;
}
