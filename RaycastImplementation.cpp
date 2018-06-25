#include "RaycastImplementation.h"

RaycastImplementation::RaycastImplementation(int newWidth): slImplementation(string("RaycastImplementation")), width(newWidth) {
}

void RaycastImplementation::preExperimentRun() {
	((slBlenderVirtualInfrastructure *)experiment->getInfrastructure())->saveBlenderFile = true;
}

double RaycastImplementation::getPatternWidth() {
	return (double)width;
}

bool RaycastImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() == 0;
}

Mat RaycastImplementation::generatePattern() {
	Size projectorResolution = experiment->getInfrastructure()->getProjectorResolution();
	
	int iterationIndex = experiment->getIterationIndex();

	int projectorHeight = (int)projectorResolution.height;

	Mat pattern(projectorHeight, width, CV_8UC3, Scalar(0, 0, 0));
	return pattern;
}

void RaycastImplementation::postIterationsProcess() {
	stringstream blenderFilename, outputFilename, blenderCommandLine;

	blenderFilename << experiment->getPath() << "slVirtualScene_0.blend";
	outputFilename << experiment->getPath() << "raycast_depth.xyz";

	slInfrastructure *infrastructure = experiment->getInfrastructure();
	Size cameraResolution = infrastructure->getCameraResolution();

	blenderCommandLine
		<< "blender -b -P RaycastDepth.py -- "
			<< blenderFilename.str() << " "
			<< outputFilename.str() << " "
			<< width << " "
			<< (int)cameraResolution.height;

	DB("blenderCommandLine: " << blenderCommandLine.str())

	int exeResult = system(blenderCommandLine.str().c_str());

	ifstream raycastDepthfile(outputFilename.str().c_str());
	string line;

//	int lineNumber = 0;
//	DB("starting to read raycast_depth.xyz file...");
	while (getline(raycastDepthfile, line)) {
/*
		DB("#" << lineNumber << " " << line)

		if (lineNumber == 143643) { 
			for (int index = 0; index < line.length(); index++) {
				DB("line[" << index << "]: " << line[index] << " (" << (int)line[index] << ")")
			}
			//exit(0);
		}
*/
		stringstream lineStream(line);
		istream_iterator<string> iterator(lineStream);
		istream_iterator<string> end;
		vector<string> results(iterator, end);

//		DB("converting x...");
		int x = atoi(results[0].c_str());
//		DB("converting y...");
		int y = atoi(results[1].c_str());
//		DB("converting z...");
		double z = atof(results[2].c_str());

//		DB("converting done");
		slDepthExperimentResult result(x, y, z);
//		DB("about to store result...");
		experiment->storeResult(&result);
//		DB("result stored");

//		lineNumber++;
	}
//	DB("raycast_depth.xyz file read");

	((slBlenderVirtualInfrastructure *)experiment->getInfrastructure())->saveBlenderFile = false;
	//remove(outputFilename.str().c_str());
}

