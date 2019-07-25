/*
 * File: slBenchmark.h
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
 * This file defines classes slBenchmark and slImplementation. The
 * slBenchmark class conducts the structured light scanning tests
 * and stores various benchmarking information. The slImplementation
 * class allows a custom structured light implementation to be
 * tested.
 */ 
#ifndef SLBENCHMARK_H
#define SLBENCHMARK_H

#include <iostream>
#include <iterator>
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>

//Physical camera/projector calibration filename/XML names
#define INTRINSIC_NAME				"intrinsic"
#define DISTORTION_NAME				"distortion"

//Default camera resolution
#define DEFAULT_CAMERA_PROJECTOR_WIDTH		1920
#define DEFAULT_CAMERA_PROJECTOR_HEIGHT		1080

//Default camera/projector FOV
#define DEFAULT_CAMERA_HORIZONTAL_FOV		49.134
#define DEFAULT_CAMERA_VERTICAL_FOV		28.841
#define DEFAULT_PROJECTOR_HORIZONTAL_FOV	20.0
#define DEFAULT_PROJECTOR_VERTICAL_FOV		20.0

//Default camera projector separation
#define DEFAULT_CAMERA_PROJECTOR_SEPARATION	1

//Default camera values
#define DEFAULT_CAMERA_PIPE			""
#define DEFAULT_CAMERA_INDEX			0

//Default projection and capture wait (pause) time in milliseconds
#define DEFAULT_WAIT_TIME			1000

using namespace std;
using namespace cv;

//Debug
//#ifdef DEBUG_BUILD
#define DB(x) cerr << x << endl;
//#else
//#define DB(x)
//#endif

//Fatal error
#define FATAL(x) cerr << "FATAL: " << x << endl; exit(-1);

//Cross platform OS separator
#ifdef _WIN32
#define OS_SEP '\\'
#else
#define OS_SEP '/'
#endif

//Forward declaration
class slExperiment;

//Abstract class that defines a structured light implementation
class slImplementation {
	public:
		//Create a structured light implementation instance with an identifier
		slImplementation(string);

		//Clean up
		virtual ~slImplementation() {};

		//Initialise prior to an experiment running
		virtual void preExperimentRun() {};

		//Initialise after an experiment has run
		virtual void postExperimentRun() {};

		//Get the width of the pattern
		virtual double getPatternWidth() = 0;

		//Get the pattern x offset factor, can account for uneven column widths
		virtual double getPatternXOffsetFactor(double);

		//Get the identifier
		string getIdentifier();

		//Check if there are any more pattern generation and capture iterations
		virtual bool hasMoreIterations();

		//Generate the structure light pattern
		virtual Mat generatePattern() = 0;

		//Process a capture
		virtual void processCapture(Mat) {};

		//Process after the interations
		virtual void postIterationsProcess();
		
		//Solve the correspondence problem
		virtual double solveCorrespondence(int, int) {return 0;} 

		//A reference to the current experiment
		slExperiment *experiment;

	protected:
		//Set the identifier
		void setIdentifier(string);

	private:
		//Initailise this structured light implementation instance
		void initialise();

		//A string used to identify this implementation
		string identifier;
};

//A device for capturing images
class slCameraDevice {
	public:
		//Create a camera device
		slCameraDevice(
			int newResolutionWidth = DEFAULT_CAMERA_PROJECTOR_WIDTH,
			int newResolutionHeight = DEFAULT_CAMERA_PROJECTOR_HEIGHT,
			double newHorizontalFOV = DEFAULT_CAMERA_HORIZONTAL_FOV,
			double newVerticalFOV = DEFAULT_CAMERA_VERTICAL_FOV,
			string newCameraPipe = string(DEFAULT_CAMERA_PIPE),
			int newCameraIndex = DEFAULT_CAMERA_INDEX
		): 
			resolution(Size(newResolutionWidth, newResolutionHeight)),
			horizontalFOV(newHorizontalFOV), 
			verticalFOV(newVerticalFOV), 
			cameraPipe(newCameraPipe),
			cameraIndex(newCameraIndex)
		{};

		//The resolution
		Size resolution;
		
		//The horizontal FOV (degrees)
		double horizontalFOV;
		
		//The vertical FOV (degrees)
		double verticalFOV;

		//The gstreamer pipe to capture with
		string cameraPipe;

		//The camera index to capture with
		int cameraIndex;
};

//A device for projecting images
class slProjectorDevice {
	public:
		//Create a projection device
		slProjectorDevice(
			int newResolutionWidth = DEFAULT_CAMERA_PROJECTOR_WIDTH,
			int newResolutionHeight = DEFAULT_CAMERA_PROJECTOR_HEIGHT,
			double newHorizontalFOV = DEFAULT_PROJECTOR_HORIZONTAL_FOV,
			double newVerticalFOV = DEFAULT_PROJECTOR_VERTICAL_FOV
		):
			resolution(Size(newResolutionWidth, newResolutionHeight)),
			horizontalFOV(newHorizontalFOV), 
			verticalFOV(newVerticalFOV) 
		{};

		//The resolution
		Size resolution;
		
		//The horizontal FOV (degrees)
		double horizontalFOV;
		
		//The vertical FOV (degrees)
		double verticalFOV;
};

//An infrastructure setup represents a particular capture device, projection device and separation between them
class slInfrastructureSetup {
	public:
		//Create an infrastructure setup
		slInfrastructureSetup(
			slCameraDevice newCameraDevice = slCameraDevice(),
			slProjectorDevice newProjectorDevice = slProjectorDevice(),
			double newCameraProjectorSeparation = DEFAULT_CAMERA_PROJECTOR_SEPARATION
		):
			cameraDevice(newCameraDevice),
			projectorDevice(newProjectorDevice),
			cameraProjectorSeparation(newCameraProjectorSeparation)
		{};

		//The camera device
		slCameraDevice cameraDevice;

		//The projector device
		slProjectorDevice projectorDevice;

		//The distance between the camera and projector
		double cameraProjectorSeparation;
};

//Abstract infrastruture (projector and camera) class used for the benchmarking
class slInfrastructure {
	public:
		//Create an infrastructure instance with a benchmark and a name
		slInfrastructure(
			string newName,
			slInfrastructureSetup newInfrastructureSetup = slInfrastructureSetup(),
			bool newCalibrationRequired = true
		): 
			name(newName), 
			infrastructureSetup(newInfrastructureSetup), 
			calibrationRequired(newCalibrationRequired),
			experiment(NULL)
		{};

		//Clean up
		virtual ~slInfrastructure() {};

		//Initialise the infrastucture
		virtual void init();

		//Project the structured light implementation pattern and capture it
		virtual Mat projectAndCapture(Mat) = 0;

		//Return the name of this infrastructure 
		string getName();

		//Get the camera resolution
		Size getCameraResolution();

		//Get the projector resolution
		Size getProjectorResolution();

		//Get the horizontal camera FOV angle (degrees)
		double getCameraHorizontalFOV();

		//Get the vertical camera FOV angle (degrees)
		double getCameraVerticalFOV();

		//Get the horizontal projector FOV angle (degrees)
		double getProjectorHorizontalFOV();

		//Get the vertical projector FOV angle (degrees)
		double getProjectorVerticalFOV();

		//Get the distance between the camera and the projector
		double getCameraProjectorSeparation();
	
		//Does this infrastructure require calibration
		bool isCalibrationRequired() {return calibrationRequired;};

		//A reference to the current experiment
		slExperiment *experiment;
		
		//The intrinsic calibration matrix
		Mat intrinsicMat;

		//The distortion calibration matrix
		Mat distortionMat;

	protected:
		//The infrastructure setup
		slInfrastructureSetup infrastructureSetup;

		//Does this infrastructure require calibration
		bool calibrationRequired;

	private:
		//Generate a unique identifier for this infrastructure and setup (for saving/reading calibration)
		unsigned int getUniqueID();

		//The name of this infrastructure
		string name;
};

//Virtual infrastructure using open source 3D modelling software Blender (http://www.blender.org)
//NOTE: Ensure the blender executable can be found via the environment PATH variable to use
class slBlenderVirtualInfrastructure : public slInfrastructure {
	public:
		//Create a blender virtual infrastruture instance
		slBlenderVirtualInfrastructure(
			slInfrastructureSetup newInfrastructureSetup = slInfrastructureSetup()
		): 
			slInfrastructure(
				string("slBlenderVirtualInfrastructure"), 
				slInfrastructureSetup(
					newInfrastructureSetup.cameraDevice,
					//Limitation of blender spot spight projector, can only be square, horizontal sizes used
					slProjectorDevice(
						(int)newInfrastructureSetup.projectorDevice.resolution.width,
						(int)newInfrastructureSetup.projectorDevice.resolution.width,
						newInfrastructureSetup.projectorDevice.horizontalFOV,
						newInfrastructureSetup.projectorDevice.horizontalFOV
					),
					newInfrastructureSetup.cameraProjectorSeparation
				)
			),
			saveBlenderFile(false),
			virtualSceneJSONFilename(string("slVirtualScene.json"))
		{};

		//Check if saving blender file
		bool saveBlenderFile;

		//The JSON filename that describes the objects in the virtual scene
		string virtualSceneJSONFilename;

		//Initialise the infrastucture
		void init();

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);
};

//Physical infrastructure using opencv projection and video capture
class slPhysicalInfrastructure : public slInfrastructure {
	public:
		//Create a physical infrastruture instance
		slPhysicalInfrastructure(slInfrastructureSetup newInfrastructureSetup, int newWaitTime = DEFAULT_WAIT_TIME); 

		//Clean up
		virtual ~slPhysicalInfrastructure();

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);

	private:
		//The wait (pause) time in milliseconds between each projection and capture
		int waitTime;

		//The video capture object
		VideoCapture videoCapture;
};

//A simple infrastructure class that reads capture files stored in the system, for example as a result of a previous experiment
class slFileInfrastructure : public slInfrastructure {
	public:
		//Create a file infrastruture instance
		slFileInfrastructure(slInfrastructureSetup newInfrastructureSetup): 
			slInfrastructure(string("slFileInfrastructure"), newInfrastructureSetup, false)
       		{};

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);
};

//Abstract class that defines a result from a particular experiment
class slExperimentResult {
	public:
		//Create an experiment result
		slExperimentResult() {};

		//Clean up
		virtual ~slExperimentResult() {};
};

//Abstract class that defines an experiment that tests a given implementation on a given infrastructure
class slExperiment {
	public:
		//Get the current session path
		static string getSessionPath();

		//Create an experiment
		slExperiment(slInfrastructure *, slImplementation *);

		//Clean up
		virtual ~slExperiment();

		//Get the current experiment path
		string getPath();

		//Run this experiment
		void run();

		//Run before all iterations begin
		virtual void runPreIterations() {};

		//Run before this iteration begins
		virtual void runPreIteration() {};

		//Run before a pattern is generated
		virtual void runPrePatternGeneration() {};

		//Run after a pattern is generated
		virtual void runPostPatternGeneration() {};

		//Run before pattern is projected and captured
		virtual void runPreProjectAndCapture() {};

		//Run after pattern is projected and captured
		virtual void runPostProjectAndCapture() {};

		//Run before the implementation processes the capture
		virtual void runPreProcessCapture() {};

		//Run after the implementation processes the capture
		virtual void runPostProcessCapture() {};

		//Run after this iteration has completed
		virtual void runPostIteration() {};

		//Run after all iterations have completed
		virtual void runPostIterations() {};

		//Run before the implementation processes after all the iterations
		virtual void runPreImplementationPostIterationsProcess() {};

		//Run after the implementation processes after all the iterations
		virtual void runPostImplementationPostIterationsProcess() {};

		//Store a result of this experiment
		virtual void storeResult(slExperimentResult *) {};

		//Get the current infrastructure
		slInfrastructure *getInfrastructure();

		//Get the current implementation
		slImplementation *getImplementation();

		//Get the current pattern generation and capture iteration index
		int getIterationIndex();

		//Store the capture
		void storeCapture(Mat);

		//Get the capture at an index
		Mat getCaptureAt(int);

		//Get the last capture
		Mat getLastCapture();

		//Get the number of captures
		int getNumberCaptures();

		//Compute the depth from a pair of x coordinates from the projection pattern and the image
		virtual double getDisplacement(double,double);
		virtual double getDisplacement(double,double,bool);

		//Get a meaningful identifier of this experiment
		string getIdentifier();

	protected:
		//The infrastructure used for this experiment
		slInfrastructure *infrastructure;

		//The implementation being tested in this experiement
		slImplementation *implementation;

	private:
		//The current session path
		static string sessionPath;

		//The current experiment path
		string path;

		//The current pattern generation and capture iteration index
		int iterationIndex;

		//The stored captures
		vector<Mat> *captures;
};

//Class that defines a kind of experiment that records depth
class slDepthExperiment : public virtual slExperiment {
	public:
		//Create a depth experiment
		slDepthExperiment(slInfrastructure *, slImplementation *);

		//Clean up
		virtual ~slDepthExperiment();

		//Store a result of this experiment
		virtual void storeResult(slExperimentResult *);

		//Get the number of depth data values
		//int getNumDepthDataValues();

		//Check if depth data value has been set
		//bool isDepthDataValued(int);
		bool isDepthDataValued(int, int);

		//Get depth data value
		//double getDepthData(int);
		double getDepthData(int, int);
		
	private:
		//Number of depth data values
		//int numDepthDataValues;

		//Check if the depth data value has been set
		//bool *depthDataValued;
		map<int, map<int, bool> > depthDataValued;

		//The depth data
		//double *depthData;
		map<int, map<int, double> > depthData;
};

//Class that defines a depth experiment result with x, y and z coordinates
class slDepthExperimentResult : public slExperimentResult {
	public:
		//Create a depth experiment result
		slDepthExperimentResult(int, int, double);

		//The x coordinate
		int x;

		//The y coordinate
		int y;

		//The z coordinate
		double z;
};

//Class that defines a kind of experiment that records the speed of processing
class slSpeedExperiment : public virtual slExperiment {
	public:
		//Create a speed experiment
		slSpeedExperiment(slInfrastructure *, slImplementation *);

		//Run before a pattern is generated
		virtual void runPrePatternGeneration();

		//Run after a pattern is generated
		virtual void runPostPatternGeneration();

		//Run before pattern is projected and captured
		virtual void runPreProjectAndCapture();

		//Run after pattern is projected and captured
		virtual void runPostProjectAndCapture();

		//Run before the implementation processes this iteration
		virtual void runPreIterationProcess();

		//Run after the implementation processes this iteration
		virtual void runPostIterationProcess();

		//Run before the implementation processes after all the iterations
		virtual void runPrePostIterationsProcess();

		//Run after the implementation processes after all the iterations
		virtual void runPostPostIterationsProcess();

		//Get the total clock value
		clock_t getTotalClock();

	private:
		//The previous stored clock
		clock_t previousClock;

		//Total clock taken to run this experiment
		long totalClock;
};

//Class that defines a kind of experiment that records the speed of processing and depth
class slSpeedDepthExperiment : public slSpeedExperiment, public slDepthExperiment {
	public:
		//Create a speed and depth experiment
		slSpeedDepthExperiment(slInfrastructure *, slImplementation *);
};

//A metric to measure for a benchmark
class slMetric {
	public:
		//Create a metric
		slMetric() {};

		//Clean up
		virtual ~slMetric() {};

		//Compare an experiment against the reference experiment
		virtual void compareExperimentAgainstReference(slExperiment *, slExperiment *) = 0;
};

//Metric that compares the processing speed of experiments
class slSpeedMetric : public slMetric {
	public:
		//Compare an experiment against the reference experiment
		virtual void compareExperimentAgainstReference(slExperiment *, slExperiment *);
};

//Metric that compares the depth accuracy of experiments
class slAccuracyMetric : public slMetric {
	public:
		//Compare an experiment against the reference experiment
		virtual void compareExperimentAgainstReference(slExperiment *, slExperiment *);
};

//Metric that compares the resolution of experiments
class slResolutionMetric : public slMetric {
	public:
		//Compare an experiment against the reference experiment
		virtual void compareExperimentAgainstReference(slExperiment *, slExperiment *);
};

//Abstract structured light benchmarking class that can compare measurable values of experiements
class slBenchmark {
	public:
		//Create a structured light benchmark given a reference experiment
		slBenchmark(slExperiment *);

		//Clean up
		~slBenchmark();

		//Add an metric to this benchmark
		void addMetric(slMetric *);

		//Add an experiment to this benchmark
		void addExperiment(slExperiment *);

		//Compare the experiments of this benchmark
		void compareExperiments();

	protected:
		//The reference experiment to compare all other experiments against
		slExperiment *referenceExperiment;

		//The metrics to benchmark against
		vector<slMetric *> *metrics;

		//The experiments to benchmark
		vector<slExperiment *> *experiments;
};

//Reconstruct the 3D data of a given depth expriment
class sl3DReconstructor {
	public:
		//Write a XYZ point cloud file for the given depth experiment
		static void writeXYZPointCloud(slDepthExperiment *);
};

#endif //SLBENCHMARK_H
