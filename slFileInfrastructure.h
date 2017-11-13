/*
 * File: slFileInfrastructure.h
 * 
 * Copyright 2016 Evan Dekker
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Description:
 *
 * This file defines classes slBenchmark and slImplementation. The
 * slBenchmark class conducts the structured light scanning tests
 * and stores various benchmarking information. The slImplementation
 * class allows a custom structured light implementation to be
 * tested.
 */ 
#ifndef SLFILEINFRASTRUCTURE_H
#define SLFILEINFRASTRUCTURE_H

#include "slBenchmark.h"


/* A simple infrastructure class that reads capture files stored in the system, for example as a result of a previous experiment */
class slFileInfrastructure : public slInfrastructure {
	private: 
		stringstream dirname; /* The directory from which to read the capture files */

	public:
		//Create a blender virtual infrastruture instance
		slFileInfrastructure(string, double, double, double, double, double);

		//Project the structured light implementation pattern and capture it
		Mat projectAndCapture(Mat);
};
#endif //SLFILEINFRASTRUCTURE_H
