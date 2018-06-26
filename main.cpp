#include "slBenchmark.h"

// Implementations
#include "BinaryImplementation.h"
#include "GrayCodedBinaryImplementation.h"
#include "PSMImplementation.h"
#include "DeBruijnImplementation.h"
#include "RaycastImplementation.h"
#include "SingleLineImplementation.h"

int main() {
	slCameraDevice logitechC920(1920, 1080, 70.42, 43.3);
	slCameraDevice canonLegriaHFG25(1920, 1080, 71.00817551, 43.72998906, 
		string("decklinkvideosrc mode=11 ! videobalance brightness=-0.2 contrast=1.75 saturation=1.75 ! videoconvert ! video/x-raw, format=(string)BGR ! appsink")
	);
	slProjectorDevice acerX1261P(1024, 768, 31.21119655, 23.66278431);
	slProjectorDevice hitachiPJTX100(1280, 720, 38.76968605, 22.39062111);

	slInfrastructureSetup setup(canonLegriaHFG25, hitachiPJTX100, 17.5);
	
	slBlenderVirtualInfrastructure blenderVirtualInfrastructure(setup);
	slPhysicalInfrastructure physicalInfrastructure(setup);
	slFileInfrastructure fileInfrastructure(setup);

//	slInfrastructure *currentInfrastructure = &blenderVirtualInfrastructure;
//	slInfrastructure *currentInfrastructure = &physicalInfrastructure;
	slInfrastructure *currentInfrastructure = &fileInfrastructure;

	int implementationColumns = 128;
	int projectorWidth = (int)setup.projectorDevice.resolution.width;

	BinaryImplementation binaryImplementation(implementationColumns);
	GrayCodedBinaryImplementation grayCodedBinaryImplementation(implementationColumns);
	DeBruijnImplementation deBruijnImplementation(implementationColumns);
//	PSMImplementation psmImplementation;
	SingleLineImplementation singleLineImplementation(projectorWidth);
	RaycastImplementation raycastImplementation(projectorWidth);

	slSpeedDepthExperiment binaryExperiment(currentInfrastructure, &binaryImplementation);
	slSpeedDepthExperiment grayCodedBinaryExperiment(currentInfrastructure, &grayCodedBinaryImplementation);
	slSpeedDepthExperiment deBruijnExperiment(currentInfrastructure, &deBruijnImplementation);
//	slSpeedDepthExperiment psmExperiment(currentInfrastructure, &psmImplementation);
	slSpeedDepthExperiment singleLineExperiment(currentInfrastructure, &singleLineImplementation);
	slSpeedDepthExperiment raycastExperiment(&blenderVirtualInfrastructure, &raycastImplementation);

	binaryExperiment.run();
	grayCodedBinaryExperiment.run();
	deBruijnExperiment.run();
//	psmExperiment.run();
	raycastExperiment.run();
	singleLineExperiment.run();
	
	slBenchmark benchmark(&raycastExperiment);
//	slBenchmark benchmark(&singleLineExperiment);

	benchmark.addExperiment(&binaryExperiment);
	benchmark.addExperiment(&grayCodedBinaryExperiment);
	benchmark.addExperiment(&deBruijnExperiment);
//	benchmark.addExperiment(&psmExperiment);
	benchmark.addExperiment(&singleLineExperiment);

	benchmark.addMetric(new slSpeedMetric());
	benchmark.addMetric(new slAccuracyMetric());
	benchmark.addMetric(new slResolutionMetric());

	benchmark.compareExperiments();

	sl3DReconstructor::writeXYZPointCloud(&binaryExperiment);
	sl3DReconstructor::writeXYZPointCloud(&grayCodedBinaryExperiment);
	sl3DReconstructor::writeXYZPointCloud(&deBruijnExperiment);
//	sl3DReconstructor::writeXYZPointCloud(&psmExperiment);
	sl3DReconstructor::writeXYZPointCloud(&raycastExperiment);
	sl3DReconstructor::writeXYZPointCloud(&singleLineExperiment);

	return 0;
}
