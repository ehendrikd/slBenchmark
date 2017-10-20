#include "RaycastImplementation.h"

RaycastImplementation::RaycastImplementation(): slImplementation(string("RaycastImplementation")) {
}

bool RaycastImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() == 0;
}

Mat RaycastImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();
	
	int iterationIndex = experiment->getIterationIndex();

	int cameraWidth = (int)cameraResolution.width;
	int cameraHeight = (int)cameraResolution.height;

	Mat pattern(cameraHeight, cameraWidth, CV_8UC3, Scalar(0, 0, 0));
	return pattern;
}

void RaycastImplementation::postIterationsProcess() {
	stringstream blenderFilename, outputFilename, blenderCommandLine;

	blenderFilename << experiment->getPath() << "slVirtualScene_0.blend";
	outputFilename << experiment->getPath() << "raycast_depth.xyz";

	slInfrastructure *infrastructure = experiment->getInfrastructure();
	Size cameraResolution = infrastructure->getCameraResolution();
	Rect croppedArea = infrastructure->getCroppedArea();
	double zScale = infrastructure->getScale();

	blenderCommandLine
		<< "blender -b -P RaycastDepth.py -- "
			<< blenderFilename.str() << " "
			<< outputFilename.str() << " "
			<< (int)cameraResolution.width << " "
			<< (int)cameraResolution.height;

	DB("blenderCommandLine: " << blenderCommandLine.str())

	int exeResult = system(blenderCommandLine.str().c_str());

	ifstream raycastDepthfile(outputFilename.str());
	string line;

	while (getline(raycastDepthfile, line)) {
		stringstream lineStream(line);
		istream_iterator<string> iterator(lineStream);
		istream_iterator<string> end;
		vector<string> results(iterator, end);

		int x = atoi(results[0].c_str());
		int y = atoi(results[1].c_str());
		double z = atof(results[2].c_str());

		if (x >= croppedArea.x && x < (croppedArea.x + croppedArea.width) && y >= croppedArea.y && y < (croppedArea.y + croppedArea.height)) {
			slDepthExperimentResult result(x - croppedArea.x, y - croppedArea.y, z * zScale);
			experiment->storeResult(&result);
		}
	}

	remove(outputFilename.str().c_str());
}
