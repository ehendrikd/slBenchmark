#include "slBenchmark.h"

// Implementations
#include "BinaryImplementation.h"
#include "GrayCodedBinaryImplementation.h"
#include "PSMImplementation.h"
#include "DeBruijnImplementation.h"

//Infrastructures
#include "slFileInfrastructure.h"


/**
 * Below is an example of the file infrasructure. For it to work, it is expected that
 * the folders existingCapturesPSM and existingCapturesGRAY exist and contain the 
 * captures from previous experiments.
 */
int infrastructureExample() {
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


/**
 * An example of the Blender infrastructure example, where we run each 
 * implementation on the scene. It is expected that the scene is given in the 
 * appropriate blender file slVirtualScene.json.
 */
int blenderInfrastructureExample() {
	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
//	slPhysicalInfrastructure physicalInfrastructure(Size(1024, 768));

	blenderVirtualInfrastructure.setCroppedArea(Rect(619, 0, 1118, 1080)); 
//	fileInfrastructure.setCroppedArea(Rect(619, 0, 1118, 1080)); 

	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;

	slSpeedDepthExperiment experiment1(&blenderVirtualInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&blenderVirtualInfrastructure, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&blenderVirtualInfrastructure, &psmImplementation);
	slSpeedDepthExperiment experiment4(&blenderVirtualInfrastructure, &deBruijnImplementation);

//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment6(&fileInfrastructure, &deBruijnImplementation);

	experiment1.run();
	experiment2.run();
	experiment3.run();
	experiment4.run();
//	experiment5.run();
//	experiment6.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);
	speedBenchmark.addExperiment(&experiment2);
	speedBenchmark.addExperiment(&experiment3);
	speedBenchmark.addExperiment(&experiment4);
//	speedBenchmark.addExperiment(&experiment5);
//	speedBenchmark.addExperiment(&experiment6);

	speedBenchmark.compareExperiments();
/*
	slHTMLReportRenderer htmlReportRenderer;

	htmlReportRenderer.addBenchmark(&speedBenchmark);

	htmlReportRenderer.render();
*/
	return 0;
}

int main()
{
	return blenderInfrastructureExample();
}
