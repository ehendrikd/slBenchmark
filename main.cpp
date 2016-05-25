#include "slBenchmark.h"

// Implementations
#include "BinaryImplementation.h"
#include "GrayCodedBinaryImplementation.h"
#include "PSMImplementation.h"
#include "DeBruijnImplementation.h"

//Infrastructures
#include "slFileInfrastructure.h"


int main() {
	slBlenderVirtualInfrastructure blenderVirtualInfrastructure;
//	slPhysicalInfrastructure physicalInfrastructure(Size(1024, 768));
//	slFileInfrastructure fileInfrastructure("existingCaptures");

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
