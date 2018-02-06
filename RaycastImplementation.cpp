#include "RaycastImplementation.h"

RaycastImplementation::RaycastImplementation(int newWidth): slImplementation(string("RaycastImplementation")), width(newWidth) {
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
	Size projectorResolution = infrastructure->getProjectorResolution();

	blenderCommandLine
		<< "blender -b -P RaycastDepth.py -- "
			<< blenderFilename.str() << " "
			<< outputFilename.str() << " "
			<< width << " "
			<< (int)projectorResolution.height;

	DB("blenderCommandLine: " << blenderCommandLine.str())

	int exeResult = system(blenderCommandLine.str().c_str());

	ifstream raycastDepthfile(outputFilename.str().c_str());
	string line;

	while (getline(raycastDepthfile, line)) {
		stringstream lineStream(line);
		istream_iterator<string> iterator(lineStream);
		istream_iterator<string> end;
		vector<string> results(iterator, end);

		int x = atoi(results[0].c_str());
		int y = atoi(results[1].c_str());
		double z = atof(results[2].c_str());

		slDepthExperimentResult result(x, y, z);
		experiment->storeResult(&result);
	}

	//remove(outputFilename.str().c_str());
}
