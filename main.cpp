#include "slBenchmark.h"

// Implementations
#include "BinaryImplementation.h"
#include "GrayCodedBinaryImplementation.h"
#include "PSMImplementation.h"
#include "DeBruijnImplementation.h"
#include "RaycastImplementation.h"
#include "SingleLineImplementation.h"

//Infrastructures
#include "slFileInfrastructure.h"


/**
 * Below is an example of the file infrasructure. For it to work, it is expected that
 * the folders existingCapturesPSM and existingCapturesGRAY exist and contain the 
 * captures from previous experiments.
 */
int fileInfrastructureExample() {
/*	
	// Infrastructures (one set of files per infrastructure) 
	slFileInfrastructure fileInfrastructurePhaseShift("SampleFiles/PhaseShift/captures");
	slFileInfrastructure fileInfrastructureBinary("SampleFiles/Binary/captures");
	slFileInfrastructure fileInfrastructureGray("SampleFiles/GrayCode/captures");
	slFileInfrastructure fileInfrastructureDeBruijn("SampleFiles/DeBruijn/captures");

	// Implementations 
	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;

	// Experiments 	
	slSpeedDepthExperiment experiment1(&fileInfrastructureBinary, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&fileInfrastructureGray, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&fileInfrastructurePhaseShift, &psmImplementation);
	slSpeedDepthExperiment experiment4(&fileInfrastructureDeBruijn, &deBruijnImplementation);

	experiment1.run();
	experiment2.run();
	experiment3.run();
	experiment4.run();

	slSpeedBenchmark speedBenchmark;

	speedBenchmark.addExperiment(&experiment1);
	speedBenchmark.addExperiment(&experiment2);
	speedBenchmark.addExperiment(&experiment3);
	speedBenchmark.addExperiment(&experiment4);

	speedBenchmark.compareExperiments();
*/
	return 0;
}

/**
 * An example of the Blender infrastructure example, where we run each 
 * implementation on the scene. It is expected that the scene is given in the 
 * appropriate blender file slVirtualScene.json.
 */
int blenderInfrastructureExample() {
	double hFOV = DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV;
	double vFOV = DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV;

	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
//	slPhysicalInfrastructure physicalInfrastructure(Size(1024, 768));
//	slFileInfrastructure fileInfrastructure("30_60_fov_45", 30, 17.142, 60, 35.983, 1);
	slFileInfrastructure fileInfrastructureSingleLine("default_fov_sphere_cube_singleline", hFOV, vFOV, hFOV, vFOV, 1);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary_2048x1080", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureGrayCoded("default_fov_sphere_cube_graycoded", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureDeBruijn("default_fov_sphere_cube_debruijn", hFOV, vFOV, hFOV, vFOV, 1);
	
/*	
	blenderVirtualInfrastructure.setCameraHorizontalFOV(30); 
	blenderVirtualInfrastructure.setCameraVerticalFOV(17.142);	
	blenderVirtualInfrastructure.setProjectorHorizontalFOV(60); 
	blenderVirtualInfrastructure.setProjectorVerticalFOV(35.983); 
*/
	blenderVirtualInfrastructure.setCameraResolution(Size(2048, 1080)); 

	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;

//	RaycastImplementation raycastImplementation(1920);
//	SingleLineImplementation singleLineImplementation(1920);
	RaycastImplementation raycastImplementation(2048);
	SingleLineImplementation singleLineImplementation(2048);

//	slSpeedDepthExperiment experiment1(&blenderVirtualInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment experiment1(&fileInfrastructureBinary, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&blenderVirtualInfrastructure, &grayCodedBinaryImplementation);
//	slSpeedDepthExperiment experiment2(&fileInfrastructureGrayCoded, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&blenderVirtualInfrastructure, &psmImplementation);
//	slSpeedDepthExperiment experiment4(&blenderVirtualInfrastructure, &deBruijnImplementation);
	slSpeedDepthExperiment experiment4(&fileInfrastructureDeBruijn, &deBruijnImplementation);

//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment6(&fileInfrastructure, &deBruijnImplementation);
//
	slSpeedDepthExperiment experiment7(&blenderVirtualInfrastructure, &raycastImplementation);
//	slSpeedDepthExperiment experiment8(&blenderVirtualInfrastructure, &singleLineImplementation);
	slSpeedDepthExperiment experiment8(&fileInfrastructureSingleLine, &singleLineImplementation);

	experiment1.run();
//	experiment2.run();
//	experiment3.run();
//	experiment4.run();
//	experiment5.run();
//	experiment6.run();
//	experiment7.run();
//	experiment8.run();
/*
	slBenchmark benchmark(&experiment7);

	benchmark.addExperiment(&experiment1);
//	benchmark.addExperiment(&experiment2);
//	benchmark.addExperiment(&experiment3);
//	benchmark.addExperiment(&experiment4);
//	benchmark.addExperiment(&experiment5);
//	benchmark.addExperiment(&experiment6);
//	benchmark.addExperiment(&experiment7);
	benchmark.addExperiment(&experiment8);

	benchmark.addMetric(new slSpeedMetric());
	benchmark.addMetric(new slAccuracyMetric());
	benchmark.addMetric(new slResolutionMetric());

	benchmark.compareExperiments();
*/
	sl3DReconstructor::writeXYZPointCloud(&experiment1);
//	sl3DReconstructor::writeXYZPointCloud(&experiment2);
//	sl3DReconstructor::writeXYZPointCloud(&experiment3);
//	sl3DReconstructor::writeXYZPointCloud(&experiment4);
//	sl3DReconstructor::writeXYZPointCloud(&experiment7);
//	sl3DReconstructor::writeXYZPointCloud(&experiment8);

	return 0;
}

int main()
{
//	return fileInfrastructureExample();
	return blenderInfrastructureExample();
}
