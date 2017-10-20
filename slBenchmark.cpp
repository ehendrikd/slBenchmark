/*
 * File: slBenchmark.cpp
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
#include "slBenchmark.h"

//Cross platform mkdir
#ifdef _WIN32
#include <direct.h>
#endif

int makeDir(const char* name) {
#ifdef _WIN32
	return mkdir(name);
#else
	return mkdir(name, S_IRWXU);
#endif
}

/*
 * slImplementation
 */ 

//Create a structured light implementation
slImplementation::slImplementation(string newIdentifier): identifier(newIdentifier), experiment(NULL) {
}

//Set the identifier
void slImplementation::setIdentifier(string newIdentifier) {
	identifier = newIdentifier;
}

double slImplementation::getPatternWidth() {
	return experiment->getInfrastructure()->getCameraResolution().width;
}

double slImplementation::getCaptureWidth() {
	return experiment->getInfrastructure()->getCroppedArea().width;
}

double slImplementation::getDisplacement(double x_pattern, double x_image) {
    // Proper calculation of displacement depends on the following parameters:
    // * depth of view of the camera and of the projector
    // * Resolution of the camera and the projector
    // Optionally, we need also the following parameter:
    // * distance between the camera and the project.
    // Setting this will give an accurate depth. Otherwise, proportions 
    // should be correct, but not to scale.
    double xc = x_image/getCaptureWidth();
    double xp = x_pattern/getPatternWidth();
    double piOn180 = M_PI/180;
    slInfrastructure *infrastructure = experiment->getInfrastructure();
    double gammac = infrastructure->getCameraHorizontalFOV() * piOn180; // depth of camera view in radians.
    double gammap = infrastructure->getProjectorHorizontalFOV() * piOn180; // depths of projector view in radians.
    double tgc = tan(gammac/2), tgp = tan(gammap/2);
    double Delta = 1; // Distance between camera and projector

    return Delta / 2 / (tgc*xc - tgp*xp);
}

//Get the identifier
string slImplementation::getIdentifier() {
	return identifier;
}

//Check if there are any more pattern generation and capture iterations
bool slImplementation::hasMoreIterations() {
	return experiment->getIterationIndex() == 0;
}

//Process after the interations
void slImplementation::postIterationsProcess() {
	processCaptures();
	iterateCorrespondences();
}

//Iterate through the captures to solve the correseponce problem
void slImplementation::iterateCorrespondences() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
	double zScale = experiment->getInfrastructure()->getScale();

	for (int y = 0; y < cameraResolution.height; y++) {
		for (int x = 0; x < cameraResolution.width; x++) {
			double xSolved = solveCorrespondence(x,y);

			if (!isnan(xSolved) && xSolved != -1) {				
				double displacement = getDisplacement(xSolved, x);
				slDepthExperimentResult result(x, y, displacement * zScale);
				experiment->storeResult(&result);
			}
		}
	}
}

/*
 * slInfrastructure
 */ 

//Create an infrastructure instance with a name, camera resolution and cropped area
slInfrastructure::slInfrastructure(string newName, Size newCameraResolution, Rect newCroppedArea): name(newName), cameraResolution(newCameraResolution), cameraHorizontalFOV(0), projectorHorizontalFOV(0), croppedArea(newCroppedArea), zscale(1), experiment(NULL) {
}

//The name of this infrastructure
string slInfrastructure::getName() {
	return name;
}

//Set the camera resolution
void slInfrastructure::setCameraResolution(Size newCameraResolution) {
	cameraResolution = newCameraResolution;
}

//Set the cropped area
void slInfrastructure::setCroppedArea(Rect newCroppedArea) {
	croppedArea = newCroppedArea;
}

//Get the camera resolution
Size slInfrastructure::getCameraResolution() {
	return cameraResolution;
}

//Get the camera horizontal FOV angle (degrees)
double slInfrastructure::getCameraHorizontalFOV() {
	return cameraHorizontalFOV;
}

//Set the camera horizontal FOV angle (degrees)
void slInfrastructure::setCameraHorizontalFOV(double newCameraHorizontalFOV) {
	cameraHorizontalFOV = newCameraHorizontalFOV;
}

//Get the projector horizontal FOV angle (degrees)
double slInfrastructure::getProjectorHorizontalFOV() {
	return projectorHorizontalFOV;
}

//Set the projector horizontal FOV angle (degrees)
void slInfrastructure::setProjectorHorizontalFOV(double newProjectorHorizontalFOV) {
	projectorHorizontalFOV = newProjectorHorizontalFOV;
}

//Get the cropped area
Rect slInfrastructure::getCroppedArea() {
	return croppedArea;
}

//Set the scale
void slInfrastructure::setScale(double s) {
	zscale = s;
}

//Get the scale
double slInfrastructure::getScale() {
	return zscale;
}

/*
 * slBlenderVirtualInfrastructure
 */ 

//Create a blender virtual infrastructure instance
slBlenderVirtualInfrastructure::slBlenderVirtualInfrastructure() : slInfrastructure(string("slBlenderVirtualInfrastructure")) {
	setScale(280);
	setCameraHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_FOV);
	setProjectorHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_FOV);
}

//Project the structured light implementation pattern and capture it
Mat slBlenderVirtualInfrastructure::projectAndCapture(Mat patternMat) {
	DB("-> slBlenderVirtualInfrastructure::projectAndCapture()")
			
	stringstream patternFilename, captureFilename, outputFilename, blenderCommandLine;

	patternFilename << "." << OS_SEP << "blender_tmp_pattern.png";
	captureFilename << "." << OS_SEP << "blender_tmp_capture.png";
	outputFilename << experiment->getPath() << OS_SEP << "slVirtualScene_" << experiment->getIterationIndex() << ".blend";

	imwrite(patternFilename.str().c_str(), patternMat);

	blenderCommandLine 
		<< "blender -b -P slBlenderVirtualInfrastructure.py -- " 
			<< patternFilename.str() << " " 
			<< captureFilename.str() << " " 
			<< outputFilename.str() << " "
			<< (int)cameraResolution.width << " " 
			<< (int)cameraResolution.height << " "
			<< getCameraHorizontalFOV() << " "
			<< getProjectorHorizontalFOV();
			
	DB("blenderCommandLine: " << blenderCommandLine.str())

	int exeResult = system(blenderCommandLine.str().c_str());
	DB("exeResult: " << exeResult)

	if (exeResult != 0) {
		FATAL("Could not launch blender. Please ensure the blender executable can be found in the current path.")
	}

	Mat captureMat = imread(captureFilename.str().c_str());

	remove(patternFilename.str().c_str());
	remove(captureFilename.str().c_str());
	
	DB("<- slBlenderVirtualInfrastructure::projectAndCapture()")

	return captureMat;
}

/*
 * slPhysicalInfrastructure
 */ 

//Create a physical infrastructure instance
slPhysicalInfrastructure::slPhysicalInfrastructure(Size newProjectorResolution, Size newCameraResolution, int newCameraIndex, int newWaitTime): 
	slInfrastructure(string("slPhysicalInfrastructure"), newCameraResolution), 
	projectorResolution(newProjectorResolution),
	cameraIndex(newCameraIndex),
	waitTime(newWaitTime) {
}

//Project the structured light implementation pattern and capture it
Mat slPhysicalInfrastructure::projectAndCapture(Mat patternMat) {
	DB("-> slPhysicalInfrastructure::projectAndCapture()")

	VideoCapture videoCapture(cameraIndex);

	if (!videoCapture.isOpened()) {
		FATAL("Could not open video capture device #" << cameraIndex)
	}

	Mat captureMat;

	namedWindow("main", CV_WINDOW_NORMAL);
	setWindowProperty("main", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	if (projectorResolution.height != cameraResolution.height || projectorResolution.width != cameraResolution.width) {
		Mat projectorPatternMat(projectorResolution.height, projectorResolution.width, CV_8UC3);
		resize(patternMat, projectorPatternMat, projectorPatternMat.size());
		imshow("main", projectorPatternMat);
	} else {
		imshow("main", patternMat);
	}

	waitKey(waitTime);

	videoCapture >> captureMat;

	waitKey(waitTime);
	
	DB("<- slPhysicalInfrastructure::projectAndCapture()")

	return captureMat;
}

/*
 * slExperiment
 */ 

//Set default session path
string slExperiment::sessionPath = string("");

//Get the current session path
string slExperiment::getSessionPath() {
	if (sessionPath.empty()) {
		stringstream pathStream;

		pathStream << "." << OS_SEP << clock() << OS_SEP;
		sessionPath =  pathStream.str();

		makeDir(sessionPath.c_str());
	}
	
	return sessionPath;
}

//Create an experiment
slExperiment::slExperiment(slInfrastructure *newlInfrastructure, slImplementation *newImplementation) : infrastructure(newlInfrastructure), implementation(newImplementation) {
	path = string("");
	captures = new vector<Mat>();
}

//Clean up
slExperiment::~slExperiment() {
	delete captures;
}

//Get the current experiment path
string slExperiment::getPath() {
	if (path.empty()) {
		stringstream pathStream;

		pathStream << getSessionPath() << getIdentifier() << clock() << OS_SEP;
		path =  pathStream.str();

		makeDir(path.c_str());
	}
	
	return path;
}

//Run this experiment
void slExperiment::run() {
	DB("-> slExperiment::run() infrastructure: " << infrastructure->getName() << " implementation: " << implementation->getIdentifier())

	//Set the current experiments of the infrastructre and implementation to this experiment
	infrastructure->experiment = this;
	implementation->experiment = this;

	//Inform the implementation the experiment is about to run
	implementation->preExperimentRun();

	//String paths for the current implementation
	stringstream patternsPathStream, capturesPathStream, patternFileStream, captureFileStream;

	patternsPathStream << getPath() << "patterns";
	capturesPathStream << getPath() << "captures";

	makeDir(patternsPathStream.str().c_str());
	makeDir(capturesPathStream.str().c_str());

	//Zero the iteration index
	iterationIndex = 0;

	//Run before all iterations begin
	runPreIterations();
		
	//Loop until the structured light implementation's pattern generation and capture iterations are completed
	while (implementation->hasMoreIterations()) {
		//Run before this iteration begins
		runPreIteration();
		
		DB("About to start iteration #" << iterationIndex << "...")

		patternFileStream.str("");
		captureFileStream.str("");
			
		//Generate the implementation's pattern
		DB("About to implementation->generatePattern()...")




		//Run before a pattern is generated
		runPrePatternGeneration();

		Mat patternMat = implementation->generatePattern();

		//Run after a pattern is generated
		runPostPatternGeneration();




		DB("implementation->generatePattern() complete.")

		//Create current pattern file path
		patternFileStream << patternsPathStream.str() << OS_SEP << "pattern_" << iterationIndex << ".png";

		//Save the pattern to the implementation's patterns
		imwrite(patternFileStream.str(), patternMat);

		//Capture the implementation's pattern using the current infrastructure
		DB("About to infrastructure->projectAndCapture()...")




		//Run before pattern is projected and captured
		runPreProjectAndCapture();

		Mat captureMat = infrastructure->projectAndCapture(patternMat);

		//Run after pattern is projected and captured
		runPostProjectAndCapture();




		DB("infrastructure->projectAndCapture() complete.")

		//Store the capture for processing
		storeCapture(captureMat(infrastructure->getCroppedArea()));
		//storeCapture(captureMat);
		DB("after")

		//Create current capture file path
		captureFileStream << capturesPathStream.str() << OS_SEP << "capture_" << iterationIndex << ".png";

		//Save the capture to the implementation's captures
		imwrite(captureFileStream.str(), captureMat);

		//Allow the implementation to process during this iteration
		DB("About to implementation->iterationProcess()...")




		//Run before the implementation processes this iteration
		runPreIterationProcess();

		implementation->iterationProcess();

		//Run after the implementation processes this iteration
		runPostIterationProcess();




		DB("implementation->iterationProcess() complete.")

		DB("Iteration #" << iterationIndex << " complete.")



		//Run after this iteration has completed
		runPostIteration();

		//Increment the iteration index
		iterationIndex++;

	}

	//Run after all iterations have completed
	runPostIterations();



		
	//Allow the implementation to post process after the iterations
	DB("About to implementation->postIterationsProcess()...")



	//Run before the implementation processes after all the iterations
	runPreImplementationPostIterationsProcess();

	implementation->postIterationsProcess();

	//Run after the implementation processes after all the iterations
	runPostImplementationPostIterationsProcess();




	DB("implementation->postIterationsProcess() complete.")

	//Inform the implementation the experiment has completed running
	implementation->postExperimentRun();

	//Unset the current experiments of the infrastructre and implementation
	infrastructure->experiment = NULL;
	implementation->experiment = NULL;

	DB("<- slExperiment::end()")
}

//Get the current infrastructure
slInfrastructure *slExperiment::getInfrastructure() {
	return infrastructure;
}

//Get the current pattern generation and capture iteration index
int slExperiment::getIterationIndex() {
	return iterationIndex;
}

//Store the capture
void slExperiment::storeCapture(Mat captureMat) {
	captures->push_back(captureMat);
}

//Get the capture at an index
Mat slExperiment::getCaptureAt(int index) {
	return captures->at(index);
}

//Get the last capture
Mat slExperiment::getLastCapture() {
	return captures->back();
}

//Get the number of captures
int slExperiment::getNumberCaptures() {
	return captures->size();
}

//Get a meaningful identifier of this experiment
string slExperiment::getIdentifier() {
	stringstream identifierStream;

	identifierStream <<  infrastructure->getName() << implementation->getIdentifier();
//	identifierStream << "Experiment infrastructure: " << infrastructure->getName() << " implementation: " << implementation->getIdentifier();

	return identifierStream.str();
}

/*
 * slDepthExperiment
 */ 

//Create a depth experiment
slDepthExperiment::slDepthExperiment(slInfrastructure *newlInfrastructure, slImplementation *newImplementation) : slExperiment(newlInfrastructure, newImplementation), depthData(NULL) {
	int arraySize = infrastructure->getCameraResolution().width * infrastructure->getCameraResolution().height;

	depthDataValued = new bool[arraySize];
	depthData = new double[arraySize];

	for (int index = 0; index < arraySize; index++) {
		depthDataValued[index] = false;
		depthData[index] = 0.0;
	}
}

//Clean up
slDepthExperiment::~slDepthExperiment() {
	if (depthData != NULL) {
		delete[] depthDataValued;
		delete[] depthData;
	}
}

//Store a result of this experiment
void slDepthExperiment::storeResult(slExperimentResult *experimentResult) {
	slDepthExperimentResult *depthExperimentResult = (slDepthExperimentResult *)experimentResult;

	int arrayOffset = (depthExperimentResult->y * infrastructure->getCameraResolution().width) + depthExperimentResult->x;
	
	depthDataValued[arrayOffset] = true;
	depthData[arrayOffset] = depthExperimentResult->z;
}

//Run after the implementation processes after all the iterations
void slDepthExperiment::runPostImplementationPostIterationsProcess() {
	stringstream pointCloudFileStream;

	pointCloudFileStream << getPath() << "point_cloud_final.xyz";

	ofstream outputFileStream(pointCloudFileStream.str().c_str());

	for (int y = 0; y < infrastructure->getCameraResolution().height; y++) {
		for (int x = 0; x < infrastructure->getCameraResolution().width; x++) {
			int arrayOffset = (y * infrastructure->getCameraResolution().width) + x;

			if (depthDataValued[arrayOffset]) {
				outputFileStream << x << " " << y << " " << depthData[arrayOffset] << endl;
			}
		}	
	}

	outputFileStream.close();
}

/*
 * slDepthExperimentResult
 */ 

//Create a depth experiment result
slDepthExperimentResult::slDepthExperimentResult(int newX, int newY, double newZ) : x(newX), y(newY), z(newZ) {
}

/*
 * slSpeedExperiment
 */

//Create a speed experiment
slSpeedExperiment::slSpeedExperiment(slInfrastructure *newlInfrastructure, slImplementation *newImplementation) : 
	slExperiment(newlInfrastructure, newImplementation),
	previousClock(0), 
	totalClock(0) {
}

//Run before a pattern is generated
void slSpeedExperiment::runPrePatternGeneration() {
	previousClock = clock();
}

//Run after a pattern is generated
void slSpeedExperiment::runPostPatternGeneration() {
	totalClock += clock() - previousClock;
}

//Run before pattern is projected and captured
void slSpeedExperiment::runPreProjectAndCapture() {
	previousClock = clock();
}

//Run after pattern is projected and captured
void slSpeedExperiment::runPostProjectAndCapture() {
	totalClock += clock() - previousClock;
}

//Run before the implementation processes this iteration
void slSpeedExperiment::runPreIterationProcess() {
	previousClock = clock();
}

//Run after the implementation processes this iteration
void slSpeedExperiment::runPostIterationProcess() {
	totalClock += clock() - previousClock;
}

//Run before the implementation processes after all the iterations
void slSpeedExperiment::runPrePostIterationsProcess() {
	previousClock = clock();
}

//Run after the implementation processes after all the iterations
void slSpeedExperiment::runPostPostIterationsProcess() {
	totalClock += clock() - previousClock;
}

//Get the total clock value
clock_t slSpeedExperiment::getTotalClock() {
	return totalClock;
}

/*
 * slSpeedDepthExperiment
 */ 

//Create a speed and depth experiment
slSpeedDepthExperiment::slSpeedDepthExperiment(slInfrastructure *newlInfrastructure, slImplementation *newImplementation) : 
	slExperiment(newlInfrastructure, newImplementation), 
	slSpeedExperiment(newlInfrastructure, newImplementation), 
	slDepthExperiment(newlInfrastructure, newImplementation) {
}

/*
 * slResultRenderer
 */ 

//void slResult

/*
 * slBenchmark
 */ 

//Create a structured light benchmark
slBenchmark::slBenchmark() {
	experiments = new vector<slExperiment *>();
}

//Clean up
slBenchmark::~slBenchmark() {
	delete experiments;
}

//Add an experiment to this benchmark
void slBenchmark::addExperiment(slExperiment *newExperiment) {
	experiments->push_back(newExperiment);
}

/*
 * slSpeedBenchmark
 */ 

//Compare the experiments of this benchmark
void slSpeedBenchmark::compareExperiments() {
	int index = 0;
	for (vector<slExperiment *>::iterator experiment = experiments->begin(); experiment != experiments->end(); ++experiment) {
		slSpeedExperiment *speedExperiment = dynamic_cast<slSpeedExperiment *>(*experiment);
		DB(speedExperiment->getIdentifier() << " totalClock: " << speedExperiment->getTotalClock() << " (" << ((double)speedExperiment->getTotalClock() / (double)CLOCKS_PER_SEC) << " seconds)")
		index++;
	}
}

