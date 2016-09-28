#include "slBenchmark.h"

// Implementations
#include "PSMImplementation.h"
#include "GrayCodedBinaryImplementation.h"

//Infrastructures
#include "slFileInfrastructure.h"


int main() {
	slFileInfrastructure fileInfrastructurePSM("existingCapturesPSM");
	slFileInfrastructure fileInfrastructureGRAY("existingCapturesGRAY");
	fileInfrastructurePSM.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructureGRAY.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructurePSM.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructureGRAY.setCroppedArea(Rect(619, 0, 1118, 1080)); 

	PSMImplementation psmImplementation;
	GrayCodedBinaryImplementation graycodeImplementation;

	slSpeedDepthExperiment experiment1(&fileInfrastructurePSM, &psmImplementation);
	slSpeedDepthExperiment experiment2(&fileInfrastructureGRAY, &graycodeImplementation);

	experiment1.run();
	experiment2.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);
	speedBenchmark.addExperiment(&experiment2);

	speedBenchmark.compareExperiments();

	return 0;
}
