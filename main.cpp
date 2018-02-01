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
//	double hFOV = DEFAULT_CAMERA_PROJECTOR_HORIZONTAL_FOV;
//	double vFOV = DEFAULT_CAMERA_PROJECTOR_VERTICAL_FOV;

	Size logitechC920Resolution(1920, 1080);
	double logitechC920vFOV = 43.3;
	double logitechC920hFOV = 70.42;
	Size acerX1261PResolution(1024, 768);
	double acerX1261PvFOV = 23.66278431;
	double acerX1261PhFOV = 31.21119655;
	Size hitachiPJTX100Resolution(1280, 720);
	double hitachiPJTX100vFOV = 22.39062111;
	double hitachiPJTX100hFOV = 38.76968605;

	Size cameraResolution = logitechC920Resolution;
	double cameraVFOV = logitechC920vFOV;
	double cameraHFOV = logitechC920hFOV;
	Size projectorResolution = hitachiPJTX100Resolution;
	double projectorVFOV = hitachiPJTX100vFOV;
	double projectorHFOV = hitachiPJTX100hFOV;

	double cameraProjectorSeparation = 14.0;

//	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
	slPhysicalInfrastructure physicalInfrastructure(projectorResolution, cameraResolution, cameraProjectorSeparation);
//	slFileInfrastructure fileInfrastructure("30_60_fov_45", 30, 17.142, 60, 35.983, 1);
//	slFileInfrastructure fileInfrastructureSingleLine("default_fov_sphere_cube_singleline", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureSingleLine("physical_singleline_plane", cameraHFOV, cameraVFOV, projectorHFOV, projectorVFOV, cameraProjectorSeparation);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureBinary("physical_binary_plane", cameraHFOV, cameraVFOV, projectorHFOV, projectorVFOV, cameraProjectorSeparation);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_plane_binary", hFOV, vFOV, hFOV, vFOV, 1);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary_2048x1080", hFOV, vFOV, hFOV, vFOV, 1);
//	slFileInfrastructure fileInfrastructureGrayCoded("default_fov_sphere_cube_graycoded", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureGrayCoded("physical_graycoded_plane", cameraHFOV, cameraVFOV, projectorHFOV, projectorVFOV, cameraProjectorSeparation);
//	slFileInfrastructure fileInfrastructureDeBruijn("default_fov_sphere_cube_debruijn", hFOV, vFOV, hFOV, vFOV, 1);
	slFileInfrastructure fileInfrastructureDeBruijn("physical_debruijn_plane", cameraHFOV, cameraVFOV, projectorHFOV, projectorVFOV, cameraProjectorSeparation);
//	slFileInfrastructure fileInfrastructurePSM("default_fov_sphere_cube_psm", hFOV, vFOV, hFOV, vFOV, 1);
//	slFileInfrastructure fileInfrastructurePSM("default_fov_plane_psm", hFOV, vFOV, hFOV, vFOV, 1);
	
/*	
	blenderVirtualInfrastructure.setCameraHorizontalFOV(30); 
	blenderVirtualInfrastructure.setCameraVerticalFOV(17.142);	
	blenderVirtualInfrastructure.setProjectorHorizontalFOV(60); 
	blenderVirtualInfrastructure.setProjectorVerticalFOV(35.983); 
*/
/*	
	blenderVirtualInfrastructure.setCameraResolution(Size(2048, 1080)); 
	blenderVirtualInfrastructure.setProjectorResolution(Size(2048, 1080)); 
	fileInfrastructureBinary.setCameraResolution(Size(2048, 1080)); 
	fileInfrastructureBinary.setProjectorResolution(Size(2048, 1080)); 
*/

	physicalInfrastructure.setCameraVerticalFOV(cameraVFOV);
	physicalInfrastructure.setCameraHorizontalFOV(cameraHFOV);
	physicalInfrastructure.setProjectorVerticalFOV(projectorVFOV);
	physicalInfrastructure.setProjectorHorizontalFOV(projectorHFOV);

	fileInfrastructureSingleLine.setCameraResolution(cameraResolution); 
	fileInfrastructureSingleLine.setProjectorResolution(projectorResolution);
	fileInfrastructureBinary.setCameraResolution(cameraResolution); 
	fileInfrastructureBinary.setProjectorResolution(projectorResolution);
	fileInfrastructureGrayCoded.setCameraResolution(cameraResolution); 
	fileInfrastructureGrayCoded.setProjectorResolution(projectorResolution);
	fileInfrastructureDeBruijn.setCameraResolution(cameraResolution); 
	fileInfrastructureDeBruijn.setProjectorResolution(projectorResolution);

	BinaryImplementation binaryImplementation(128);
	GrayCodedBinaryImplementation grayCodedBinaryImplementation(128);
//	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation(128);

	SingleLineImplementation singleLineImplementation((int)projectorResolution.width);
//	RaycastImplementation raycastImplementation(1920);
//	SingleLineImplementation singleLineImplementation(1920);
//	RaycastImplementation raycastImplementation(2048);
//	SingleLineImplementation singleLineImplementation(2048);

//	slSpeedDepthExperiment experiment1(&blenderVirtualInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment experiment1(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment1(&fileInfrastructureBinary, &binaryImplementation);
//	slSpeedDepthExperiment experiment2(&blenderVirtualInfrastructure, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment experiment2(&physicalInfrastructure, &grayCodedBinaryImplementation);
//	slSpeedDepthExperiment experiment2(&fileInfrastructureGrayCoded, &grayCodedBinaryImplementation);
//	slSpeedDepthExperiment experiment3(&blenderVirtualInfrastructure, &psmImplementation);
//	slSpeedDepthExperiment experiment3(&fileInfrastructurePSM, &psmImplementation);
//	slSpeedDepthExperiment experiment4(&blenderVirtualInfrastructure, &deBruijnImplementation);
	slSpeedDepthExperiment experiment4(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment4(&fileInfrastructureDeBruijn, &deBruijnImplementation);

//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment experiment5(&physicalInfrastructure, &binaryImplementation);
//	slSpeedDepthExperiment experiment6(&fileInfrastructure, &deBruijnImplementation);
//
//	slSpeedDepthExperiment experiment7(&blenderVirtualInfrastructure, &raycastImplementation);
//	slSpeedDepthExperiment experiment8(&blenderVirtualInfrastructure, &singleLineImplementation);
	slSpeedDepthExperiment experiment8(&physicalInfrastructure, &singleLineImplementation);
//	slSpeedDepthExperiment experiment8(&fileInfrastructureSingleLine, &singleLineImplementation);

//	experiment1.run();
//	experiment2.run();
//	experiment3.run();
	experiment4.run();
//	experiment5.run();
//	experiment6.run();
//	experiment7.run();
//	experiment8.run();
/*	
	slBenchmark benchmark(&experiment8);

	benchmark.addExperiment(&experiment1);
	benchmark.addExperiment(&experiment2);
//	benchmark.addExperiment(&experiment3);
	benchmark.addExperiment(&experiment4);

	benchmark.addMetric(new slSpeedMetric());
	benchmark.addMetric(new slAccuracyMetric());
	benchmark.addMetric(new slResolutionMetric());

	benchmark.compareExperiments();
*/
//	sl3DReconstructor::writeXYZPointCloud(&experiment1);
//	sl3DReconstructor::writeXYZPointCloud(&experiment2);
//	sl3DReconstructor::writeXYZPointCloud(&experiment3);
	sl3DReconstructor::writeXYZPointCloud(&experiment4);
//	sl3DReconstructor::writeXYZPointCloud(&experiment7);
//	sl3DReconstructor::writeXYZPointCloud(&experiment8);
/*
	int numberPatterns = grayCodedBinaryImplementation.getNumberPatterns();
	int pos63 = grayCodedBinaryImplementation.convertGrayCodeToInteger(63, numberPatterns);	
	int neg64 = grayCodedBinaryImplementation.convertGrayCodeToInteger(-64, numberPatterns);	
	
	DB("numberPatterns: " << numberPatterns << " pos63: " << pos63 << " neg64: " << neg64)
*/
	return 0;
}

int main()
{
//	return fileInfrastructureExample();
	return blenderInfrastructureExample();
}
