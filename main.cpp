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
	/* Infrastructures (one set of files per infrastructure) */
	slFileInfrastructure fileInfrastructurePhaseShift("SampleFiles/PhaseShift/captures");
	slFileInfrastructure fileInfrastructureBinary("SampleFiles/Binary/captures");
	slFileInfrastructure fileInfrastructureGray("SampleFiles/GrayCode/captures");
	slFileInfrastructure fileInfrastructureDeBruijn("SampleFiles/DeBruijn/captures");

	/* Implementations */
	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;

	/* Experiments */
/*	
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
	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
//	slPhysicalInfrastructure physicalInfrastructure(Size(1024, 768));
//	slFileInfrastructure fileInfrastructure("existingCapturesGT_1920");
//	slFileInfrastructure fileInfrastructure("Unity_captures_offset");
//	slFileInfrastructure fileInfrastructure("blender_same_fov");
//	slFileInfrastructure fileInfrastructure("singleline_2_cubes");
//	slFileInfrastructure fileInfrastructure("singleline_45_deg");
//	slFileInfrastructure fileInfrastructure("default_fov_45");
//	slFileInfrastructure fileInfrastructure("30_60_fov_45");
//	slFileInfrastructure fileInfrastructure("default_fov_cube_steps");
	slFileInfrastructure fileInfrastructure("default_fov_sphere_cube");
	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary");
	
	fileInfrastructure.setCameraHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV);	
	fileInfrastructure.setCameraVerticalFOV(DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV);	
	fileInfrastructure.setProjectorHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV);	
	fileInfrastructure.setProjectorVerticalFOV(DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV);	
	fileInfrastructureBinary.setCameraHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV);	
	fileInfrastructureBinary.setCameraVerticalFOV(DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV);	
	fileInfrastructureBinary.setProjectorHorizontalFOV(DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV);	
	fileInfrastructureBinary.setProjectorVerticalFOV(DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV);	
/*	
	fileInfrastructure.setCameraHorizontalFOV(30);	
	fileInfrastructure.setCameraVerticalFOV(17.142);	
	fileInfrastructure.setProjectorHorizontalFOV(60);	
	fileInfrastructure.setProjectorVerticalFOV(35.983);	
	
	blenderVirtualInfrastructure.setCameraHorizontalFOV(30); 
	blenderVirtualInfrastructure.setCameraVerticalFOV(17.142);	
	blenderVirtualInfrastructure.setProjectorHorizontalFOV(60); 
	blenderVirtualInfrastructure.setProjectorVerticalFOV(35.983); 
*/
//	blenderVirtualInfrastructure.setCameraResolution(Size(2048, 100)); 

	BinaryImplementation binaryImplementation;
	GrayCodedBinaryImplementation grayCodedBinaryImplementation;
	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation;

	RaycastImplementation raycastImplementation;
	SingleLineImplementation singleLineImplementation(1920);
//	SingleLineImplementation singleLineImplementation(2048);

//	slSpeedDepthExperiment experiment1(&blenderVirtualInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment experiment1(&fileInfrastructureBinary, &binaryImplementation);
	slSpeedDepthExperiment experiment2(&blenderVirtualInfrastructure, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment3(&blenderVirtualInfrastructure, &psmImplementation);
	slSpeedDepthExperiment experiment4(&blenderVirtualInfrastructure, &deBruijnImplementation);

//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment6(&fileInfrastructure, &deBruijnImplementation);
//
	slSpeedDepthExperiment experiment7(&blenderVirtualInfrastructure, &raycastImplementation);
//	slSpeedDepthExperiment experiment8(&blenderVirtualInfrastructure, &singleLineImplementation);
	slSpeedDepthExperiment experiment8(&fileInfrastructure, &singleLineImplementation);

	experiment1.run();
//	experiment2.run();
//	experiment3.run();
//	experiment4.run();
//	experiment5.run();
//	experiment6.run();
//	experiment7.run();
	experiment8.run();
/*
	slBenchmark benchmark(&experiment7);

	benchmark.addExperiment(&experiment1);
//	benchmark.addExperiment(&experiment2);
//	benchmark.addExperiment(&experiment3);
//	benchmark.addExperiment(&experiment4);
//	benchmark.addExperiment(&experiment5);
//	benchmark.addExperiment(&experiment6);
//	benchmark.addExperiment(&experiment7);
//	benchmark.addExperiment(&experiment8);

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
	sl3DReconstructor::writeXYZPointCloud(&experiment8);

	return 0;
}

int main()
{
//	return fileInfrastructureExample();
	return blenderInfrastructureExample();
}
