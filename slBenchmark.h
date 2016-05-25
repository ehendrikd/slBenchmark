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
#include <fstream>
#include <stdlib.h>
#include <ctime>
#include <sys/stat.h>
#include <opencv2/opencv.hpp>

//Default camera resolution
#define DEFAULT_CAMERA_WIDTH	1920
#define DEFAULT_CAMERA_HEIGHT	1080

//Default camera index
#define DEFAULT_CAMERA_INDEX	0

//Default projection and capture wait (pause) time in milliseconds
#define DEFAULT_WAIT_TIME	1000

using namespace std;
using namespace cv;

//Debug
#ifdef DEBUG_BUILD
#define DB(x) cerr << x << endl;
#else
#define DB(x)
#endif

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

		//Get the identifier
		string getIdentifier();

		//Check if there are any more pattern generation and capture iterations
		virtual bool hasMoreIterations();

		//Generate the structure light pattern
		virtual Mat generatePattern() = 0;

		//Process during an interation
		virtual void iterationProcess() {};

		//Process after the interations
		virtual void postIterationsProcess() {};

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

//Abstract infrastruture (projector and camera) class used for the benchmarking
class slInfrastructure {
	public:
		//Create an infrastructure instance with a benchmark and a name
		slInfrastructure(
			string,
			Size newCameraResolution = Size(DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT),
			Rect newCroppedArea = Rect(0, 0, DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT)
		);

		//Clean up
		virtual ~slInfrastructure() {};

		//Set the camera resolution
		void setCameraResolution(Size);

		//Set the cropped area
		void setCroppedArea(Rect);

		//Project the structured light implementation pattern and capture it
		virtual Mat projectAndCapture(Mat) = 0;

		//Return the name of this infrastructure 
		string getName();

		//Get the camera resolution
		Size getCameraResolution();

		//Get the cropped area
		Rect getCroppedArea();

		//A reference to the current experiment
		slExperiment *experiment;

	protected:
		//The camera resolution
		Size cameraResolution;

		//The cropped area
		Rect croppedArea;

	private:
		//The name of this infrastructure
		string name;
};

//Virtual infrastructure using open source 3D modelling software Blender (http://www.blender.org)
//NOTE: Ensure the blender executable can be found via the environment PATH variable to use
class slBlenderVirtualInfrastructure : public slInfrastructure {
	public:
		//Create a blender virtual infrastruture instance
		slBlenderVirtualInfrastructure();

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);
};

//Physical infrastructure using opencv projection and video capture
class slPhysicalInfrastructure : public slInfrastructure {
	public:
		//Create a physical infrastruture instance
		slPhysicalInfrastructure(
			Size newProjectorResolution = Size(DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT),
			Size newCameraResolution = Size(DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT),
			int newCameraIndex = DEFAULT_CAMERA_INDEX,			
			int newWaitTime = DEFAULT_WAIT_TIME
		);

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);

	private:
		//The projector resolution
		Size projectorResolution;

		//The camera index to use
		int cameraIndex;

		//The wait (pause) time in milliseconds between each projection and capture
		int waitTime;
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

		//Run before the implementation processes this iteration
		virtual void runPreIterationProcess() {};

		//Run after the implementation processes this iteration
		virtual void runPostIterationProcess() {};

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

		//Get a meaningful identifier of this experiment
		string getIdentifier();

	protected:
		//The infrastructure used for this experiment
		slInfrastructure *infrastructure;

		//The implememntation being tested in this experiement
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
		
		//Run after the implementation processes after all the iterations
		virtual void runPostImplementationPostIterationsProcess();

	private:
		//Check if the depth data value has been set
		bool *depthDataValued;

		//The depth data
		double *depthData;
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

//Abstract structured light benchmarking class that can compare measurable values of experiements
class slBenchmark {
	public:
		//Create a structured light benchmark
		slBenchmark();

		//Clean up
		~slBenchmark();

		//Add an experiment to this benchmark
		void addExperiment(slExperiment *);

		//Compare the experiments of this benchmark
		virtual void compareExperiments() = 0;

	protected:
		//The experiments to benchmark
		vector<slExperiment *> *experiments;
};

//Benchmark that compares the processing speed of each experiment
class slSpeedBenchmark : public slBenchmark {
	public:
		//Compare the experiments of this benchmark
		virtual void compareExperiments();
};

/*
//Abstract class that renderers human readable forms of the benchmarks
class slResultRenderer {
	public:
		//Render the results to the benchmark path
		void render(string);

		//Render the header
		virtual string renderHeader();

		//Render the footer
		virtual string renderFooter();

		//Render a paragraph
		virtual string renderParagraph(string) = 0;
};

//Render a HTML page of the results
class slHTMLResultRenderer : public slResultRenderer {
	public:
		//Render the header
		string renderHeader();

		//Render the footer
		string renderFooter();

		//Render a paragraph
		string renderParagraph(string);
};
*/
#endif //SLBENCHMARK_H
