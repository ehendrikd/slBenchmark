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
	slCameraDevice logitechC920(1920, 1080, 70.42, 43.3);
	//slCameraDevice canonLegriaHFG25(1920, 1080, 71.00817551, 43.72998906, string("decklinkvideosrc mode=11 ! videobalance brightness=-0.31496063 contrast=1.393700787 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
	slCameraDevice canonLegriaHFG25(1920, 1080, 71.00817551, 43.72998906, string("decklinkvideosrc mode=11 ! videobalance brightness=-0.2 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
	//slCameraDevice canonLegriaHFG25(1920, 500, 71.00817551, 20.245365306, string("decklinkvideosrc mode=11 ! videobalance brightness=-0.31496063 contrast=1.393700787 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink"));
	slProjectorDevice acerX1261P(1024, 768, 31.21119655, 23.66278431);
	slProjectorDevice hitachiPJTX100(1280, 720, 38.76968605, 22.39062111);

	slInfrastructureSetup physicalSetup(canonLegriaHFG25, hitachiPJTX100, 17.5);
	slInfrastructureSetup virtualSetup;
	
	slInfrastructureSetup currentSetup = physicalSetup;

//	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
	slPhysicalInfrastructure physicalInfrastructure(currentSetup);
//	slFileInfrastructure fileInfrastructureSingleLine("default_fov_sphere_cube_singleline", currentSetup);
	slFileInfrastructure fileInfrastructureSingleLine("physical_singleline_plane", currentSetup);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary", currentSetup);
	slFileInfrastructure fileInfrastructureBinary("physical_binary_plane", currentSetup);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_plane_binary", currentSetup);
//	slFileInfrastructure fileInfrastructureBinary("default_fov_sphere_cube_binary_2048x1080", currentSetup);
//	slFileInfrastructure fileInfrastructureGrayCoded("default_fov_sphere_cube_graycoded", currentSetup);
	slFileInfrastructure fileInfrastructureGrayCoded("physical_graycoded_plane", currentSetup);
//	slFileInfrastructure fileInfrastructureDeBruijn("default_fov_sphere_cube_debruijn", currentSetup);
	slFileInfrastructure fileInfrastructureDeBruijn("physical_debruijn_plane", currentSetup);
//	slFileInfrastructure fileInfrastructurePSM("default_fov_sphere_cube_psm", currentSetup);
//	slFileInfrastructure fileInfrastructurePSM("default_fov_plane_psm", currentSetup);
	
	BinaryImplementation binaryImplementation(128);
	GrayCodedBinaryImplementation grayCodedBinaryImplementation(128);
//	PSMImplementation psmImplementation;
	DeBruijnImplementation deBruijnImplementation(128);

	SingleLineImplementation singleLineImplementation((int)currentSetup.projectorDevice.resolution.width);
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

	experiment1.run();
	experiment2.run();
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
	sl3DReconstructor::writeXYZPointCloud(&experiment1);
	sl3DReconstructor::writeXYZPointCloud(&experiment2);
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
/*
	
	VideoCapture videoCapture("decklinkvideosrc mode=11 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink");

	if (!videoCapture.isOpened()) {
		FATAL("Could not open video capture device")
	}

	Mat captureMat;

	namedWindow("main", CV_WINDOW_NORMAL);

	while(true) {
		videoCapture.read(captureMat);

		if(captureMat.empty())
            		break;


	        imshow("main", captureMat);
        	if(waitKey(1) == 's')
           		 break;
	}
*/
}
