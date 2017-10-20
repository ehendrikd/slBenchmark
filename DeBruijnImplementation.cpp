#include "DeBruijnImplementation.h"

DeBruijnImplementation::DeBruijnImplementation(): slImplementation(string("DeBruijnImplementation")),numberEdges(124) {
}

DeBruijnImplementation::DeBruijnImplementation(unsigned int numEdges): slImplementation(string("DeBruijnImplementation")),numberEdges(numEdges) {
}
void DeBruijnImplementation::preExperimentRun() {
	slInfrastructure *infrastructure = experiment->getInfrastructure();

	transitions = new Vec3s[getNumberEdges()];
}

void DeBruijnImplementation::postExperimentRun() {
	delete[] transitions;
}

// For these implementations, the "width" of the pattern
// is the number of columns.
double DeBruijnImplementation::getPatternWidth() {
	return this->getNumberColumns();
}
bool DeBruijnImplementation::hasMoreIterations() {
        return experiment->getIterationIndex() < 1;
}

unsigned int DeBruijnImplementation::getNumberColumns() {
    return this->numberEdges+1;
}

unsigned int DeBruijnImplementation::getNumberEdges() {
    return this->numberEdges;
}

Mat DeBruijnImplementation::generatePattern() {
	Size cameraResolution = experiment->getInfrastructure()->getCameraResolution();

	int screenWidth = (int)cameraResolution.width;
	int screenHeight = (int)cameraResolution.height;

	float columnWidth = (float)screenWidth / getNumberColumns();

	int size = DEBRUIJN_K * DEBRUIJN_N;
	vector<int> a(size), sequence;

	fill(a.begin(), a.end(), 0);
	db(1, 1, DEBRUIJN_K, DEBRUIJN_N, a, sequence);

	Mat pattern(screenHeight, screenWidth, CV_8UC3, Scalar(0, 0, 0));

	float columnX = 0;
	int pj = 1;

	for (int columnIndex = 0; columnIndex < getNumberColumns(); columnIndex++) {
		int dj = sequence[columnIndex] + 1;

		int pjInit = pj;
		Vec3s pInit(0,0,0);

		for(int c=0;c<3;c++){
			if (pjInit & (1 << c)) {
				pInit[c] = 255;
			}
		}

		if (columnIndex > 0) {
			pj = pj ^ dj;
		}

		Vec3s point(0,0,0);
		for(int c=0;c<3;c++) {
			if (pj & (1 << c)) {
				point[c] = 255;
			}
		}

		if (columnIndex > 0) {
			transitions[columnIndex-1] = (point-pInit)/255;
		}

		rectangle(pattern, Point(columnX, 0), Point(columnX + columnWidth, screenHeight), point, FILLED);
		columnX += columnWidth;
	}

	return pattern;
}

void DeBruijnImplementation::postIterationsProcess() {
	slInfrastructure *infrastructure = experiment->getInfrastructure();
	Rect croppedArea = infrastructure->getCroppedArea();
	Mat captureMat = experiment->getLastCapture();
	double zScale = infrastructure->getScale();

	float columnWidth = (float)infrastructure->getCameraResolution().width / (float)getNumberColumns();

	for (int y = 0; y < croppedArea.height; y++) {
		int prevR = 0;
		int prevG = 0;
		int prevB = 0;
		Vec3s prevCapturelBGR(0,0,0);

		int rgbWidth = croppedArea.width * 3;

		int gradients[croppedArea.width];
		int correspondence[croppedArea.width];
		/*int differences[rgbWidth];*/
		Vec3s *differences = new Vec3s[rgbWidth];
		Vec3s *edges = new Vec3s[rgbWidth];

		for (int x = 0; x < croppedArea.width; x++) {
			Vec3s capturelBGR = captureMat.at<Vec3b>(y, x); /* Stored in signed ints to be able to take the difference */
			
			differences[x] = capturelBGR - prevCapturelBGR;
			gradients[x] = norm(differences[x],NORM_L2SQR) ;
			prevCapturelBGR = capturelBGR;
		}

		int edgeIndex = 0;

		for (int x = 1;x < (croppedArea.width - 1); x++) {
			if (
				(gradients[x - 1] + DEBRUIJN_THRESHOLD) < gradients[x] && 
				(gradients[x + 1] + DEBRUIJN_THRESHOLD) < gradients[x]
			) {
				edges[edgeIndex] = differences[x];
				correspondence[edgeIndex] = x;
				edgeIndex++;
			}
		}

		pairScore **S;
		int nCorrespondences = 0;

		S = (pairScore**) malloc(edgeIndex * sizeof(*S));

		for (int i = 0; i< edgeIndex; i++) { 
			S[i] = (pairScore*)malloc(getNumberEdges() * sizeof(pairScore));

			for (int j = 0;j < getNumberEdges(); j++) {
				S[i][j].score = 0;
				S[i][j].numberItems = 0;
				S[i][j].caseSigma = 0;
			}
		}

		sigma(edgeIndex - 1, getNumberEdges() - 1, transitions, edges, S);

		nCorrespondences = S[edgeIndex - 1][getNumberEdges() - 1].numberItems;
		
//		DB("Number of correspondences for y=" << y << ": " << nCorrespondences)

		int (*correspondences)[2] = new int[nCorrespondences][2];
		
		populateCorrespondences(edgeIndex - 1, getNumberEdges() - 1, correspondences, S);

		for (int i = 0; i < edgeIndex; i++) {
			free(S[i]);
		}
		free(S);

		for (int i = 0; i < nCorrespondences ; i++) {
			int newX = correspondences[i][0];
			int x = correspondence[newX];

			int xPos = (correspondences[i][1] + 1);
			
			double displacement = getDisplacement(xPos,x);

			slDepthExperimentResult result(x, y, displacement * zScale);
			experiment->storeResult(&result);
    		}

		delete[] correspondences;
		delete[] differences;
		delete[] edges;
	}
}

void DeBruijnImplementation::db(int t, int p, int k, int n, vector<int> &a, vector<int> &sequence) {
	if (t > n) {
		if (n % p == 0) {
			for (int toAdd = 1; toAdd < (p + 1); toAdd++) {
				sequence.push_back(a[toAdd]);
			}
		}
	} else {
		a[t] = a[t - p];
		db(t + 1, p, k, n, a, sequence);

		for (int j = a[t - p] + 1; j < k; j++) {
			a[t] = j;
			db(t + 1, t, k, n, a, sequence);
		}	
	}
}

double DeBruijnImplementation::clamp(double x, double x0, double x1) {
	if (x < x0) {
		return x0;
	}
	if (x > x1) { 
		return x1;
	}
	return x;
}

double DeBruijnImplementation::consistency(int qc, int ec) {
	double ecd = (double)ec / 255.0;

	switch (qc) {
		case 1:
			return clamp((ecd - DEBRUIJN_ALPHA) / (DEBRUIJN_BETA - DEBRUIJN_ALPHA), -1, 1);
		case 0:
			return clamp(1.0 - ((fabs(ecd) - DEBRUIJN_ALPHA) / (DEBRUIJN_BETA - DEBRUIJN_ALPHA)), -1, 1);
		case -1:
			return consistency(1, -ec);
		default:
			exit(-1);
	}
}

double DeBruijnImplementation::score(Vec3s q, Vec3s e) {
	double sr = consistency(q[0], e[0]);
	double sg = consistency(q[1], e[1]);
	double sb = consistency(q[2], e[2]);

	if (sr > sg && sb > sg) {
		return sg;
	}
	if (sr > sb) {
		return sb;
	}
	return sr;
}

double DeBruijnImplementation::sigma(int i, int  j, Vec3s *patterns, Vec3s *edges, pairScore **Scache) {
	double value1, value2, value3;

	if (i < 0 || j <  0) {
		return 0;
	}

	if (Scache[i][j].caseSigma != 0) {
		return Scache[i][j].score;
	}

	value1 = sigma(i - 1, j - 1, patterns, edges, Scache) + score(patterns[j], edges[i]);
	value2 = sigma(i - 1, j, patterns, edges, Scache);
	value3 = sigma(i, j - 1, patterns, edges, Scache);

	if (value1 > value2 && value1 > value3) {
		Scache[i][j].caseSigma = 1;
		Scache[i][j].score = value1;

		if (i == 0 || j == 0) {
			Scache[i][j].numberItems = 1;
		} else {
			Scache[i][j].numberItems = Scache[i - 1][j - 1].numberItems + 1;
		}

		return value1;
	}

	if (value2 > value3) {
		Scache[i][j].caseSigma = 2;
		Scache[i][j].score = value2;

		if (i == 0) {
			Scache[i][j].numberItems = 0;
		} else {
			Scache[i][j].numberItems = Scache[i - 1][j].numberItems;
		}

		return value2;
	}

	Scache[i][j].caseSigma = 3;
	Scache[i][j].score = value3;

	if (j == 0) {
		Scache[i][j].numberItems = 0;
	} else {
		Scache[i][j].numberItems = Scache[i][j - 1].numberItems;
	}

	return value3;
}

void DeBruijnImplementation::populateCorrespondences(int i, int j, int correspondences[][2], pairScore **S) {
	if (i < 0 || j < 0) {
		return;
	}
	
	/* Find if we are in case 1, 2 or 3 */
	/*if(S[i][j].score > S[i-1][j].score && S[i][j].score > S[i][j-1].score){*/
	switch (S[i][j].caseSigma) {
		case 1:
			correspondences[S[i][j].numberItems - 1][0] = i;
			correspondences[S[i][j].numberItems - 1][1] = j;
			populateCorrespondences(i - 1, j - 1, correspondences, S);
			break;
		case 2:
			populateCorrespondences(i - 1, j, correspondences, S);
			break;
		case 3:
			populateCorrespondences(i, j - 1, correspondences, S);
			break;
		default:
			DB("ERROR CHECKING CASE in populateCorrespondences")
	}
}

