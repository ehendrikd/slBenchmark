#include "slBenchmark.h"

// Implementations
#include "BinaryImplementation.h"
#include "GrayCodedBinaryImplementation.h"
#include "PSMImplementation.h"
#include "DeBruijnImplementation.h"
#include "GroundTruthImplementation.h"

//Infrastructures
#include "slFileInfrastructure.h"


/**
 * Below is an example of the file infrasructure. For it to work, it is expected that
 * the folders existingCapturesPSM and existingCapturesGRAY exist and contain the 
 * captures from previous experiments.
 */
int fileInfrastructureExample() {
	/* Infrastructures (one set of files per infrastructure) */
	slFileInfrastructure fileInfrastructurePhaseShift("SampleFiles/PhaseShift/captures");
	slFileInfrastructure fileInfrastructureBinary("SampleFiles/Binary/captures");
	slFileInfrastructure fileInfrastructureGray("SampleFiles/GrayCode/captures");
	slFileInfrastructure fileInfrastructureDeBruijn("SampleFiles/DeBruijn/captures");
	fileInfrastructurePhaseShift.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructureBinary.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructureGray.setCroppedArea(Rect(619, 0, 1118, 1080)); 
	fileInfrastructureDeBruijn.setCroppedArea(Rect(619, 0, 1118, 1080)); 

	/* Implementations */
	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;
//	GroundTruthImplementation groundTruthImplementation;

	/* Experiments */
	slSpeedDepthExperiment experiment1(&fileInfrastructureBinary, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&fileInfrastructureGray, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&fileInfrastructurePhaseShift, &psmImplementation);
	slSpeedDepthExperiment experiment4(&fileInfrastructureDeBruijn, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&blenderVirtualInfrastructure, &groundTruthImplementation);

//

	experiment1.run();
	experiment2.run();
	experiment3.run();
	experiment4.run();
//	experiment5.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);
	speedBenchmark.addExperiment(&experiment2);
	speedBenchmark.addExperiment(&experiment3);
	speedBenchmark.addExperiment(&experiment4);
//	speedBenchmark.addExperiment(&experiment5);

	speedBenchmark.compareExperiments();
/*
	slHTMLReportRenderer htmlReportRenderer;

	htmlReportRenderer.addBenchmark(&speedBenchmark);

	htmlReportRenderer.render();
*/
	return 0;
}

int groundTruthExample() {
	slFileInfrastructure fileInfrastructureGT("existingCapturesGT");
	fileInfrastructureGT.setCroppedArea(Rect(619, 0, 1118, 1080)); 

	GroundTruthImplementation groundTruthImplementation;

	slSpeedDepthExperiment experiment1(&fileInfrastructureGT, &groundTruthImplementation);

	experiment1.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);

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
	GroundTruthImplementation groundTruthImplementation;

	slSpeedDepthExperiment experiment1(&blenderVirtualInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&blenderVirtualInfrastructure, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&blenderVirtualInfrastructure, &psmImplementation);
	slSpeedDepthExperiment experiment4(&blenderVirtualInfrastructure, &deBruijnImplementation);
	slSpeedDepthExperiment experiment7(&blenderVirtualInfrastructure, &groundTruthImplementation);

//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment6(&fileInfrastructure, &deBruijnImplementation);
//

	experiment1.run();
	experiment2.run();
	experiment3.run();
	experiment4.run();
//	experiment5.run();
//	experiment6.run();
	experiment7.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);
	speedBenchmark.addExperiment(&experiment2);
	speedBenchmark.addExperiment(&experiment3);
	speedBenchmark.addExperiment(&experiment4);
//	speedBenchmark.addExperiment(&experiment5);
//	speedBenchmark.addExperiment(&experiment6);
	speedBenchmark.addExperiment(&experiment7);

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
	return fileInfrastructureExample();
//	return blenderInfrastructureExample();
//	return groundTruthExample();
}
