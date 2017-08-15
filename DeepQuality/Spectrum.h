#pragma once

#include <vector>
#include "Utility.h"
#include "CS.h"

using namespace std;

//* the class to handle spectrum construction, peak appending, spectrum CS, etc.
//* NOTE: this class can only be used to process spectra one by one, not to store them all
class Spectrum {
private:
	//* store mz and intensity 
	vector<float> vMz;
	vector<float> vIntensity;
	//* use this to contruct Tensor
	float *pMz;
	float *pIntensity;	
	//* the number of parsed spectra
	int nSpectra;
	//* compressed sensing parameters
	float mz_min;
	float mz_max;
	float delta;
	int sample;
	int N;
	//* Tensors to do compressed sensing
	Tensor<cpu, 1> TMz;
	Tensor<cpu, 1> TIntensity;
	SensingMatrix *pA;
	Tensor<cpu, 2> subA;
	Tensor<cpu, 1> sensed;
public:
	//* abc -> flag 1 -> do, set 0;
	//* abc -> flag 0 -> skip;
	//* num -> flag 0 -> new spectrum, append
	//* num -> flag 1 -> append
	bool lineFlag;
	Spectrum(float min, float max, float del, int sam);
	~Spectrum();
	//* when a new spectrum come
	void reBuild();
	//* add new mz and intensity
	void addPeak(float mz, float intensity);
	//* compressed sensing and output
	void CS(FILE *oFile);
	//* turn flag
	void turnFlag();
	//* handle empty spectrum
	bool hasBegin;
};

Spectrum::Spectrum(float min, float max, float del, int sam) {
	//* initialization
	pMz = nullptr;
	pIntensity = nullptr;
	nSpectra = 0;
	lineFlag = false;
	hasBegin = false;
	mz_min = min;
	mz_max = max;
	delta = del;
	sample = sam;
	N = (mz_max - mz_min) / delta;
	//* construct A
	pA = new SensingMatrix(sample, N);
	//! printf("A:\n");
	//! MatrixPrinter(pA->tsA);
}

Spectrum::~Spectrum() {
	delete pA;
}

inline void Spectrum::reBuild() {
	//* initialization
	vMz.clear();
	vIntensity.clear();
	pMz = nullptr;
	pIntensity = nullptr;
	nSpectra += 1;
}

inline void Spectrum::addPeak(float mz, float intensity) {
	vMz.push_back(mz);
	vIntensity.push_back(intensity);
}

void Spectrum::CS(FILE *oFile) {
	//! printf("-> %d\n", vMz.size());
	//* convert vector to float[]
	pMz = &vMz[0];
	pIntensity = &vIntensity[0];
	TMz = Tensor<cpu, 1>(pMz, Shape1(vMz.size()));
	TIntensity = Tensor<cpu, 1>(pIntensity, Shape1(vIntensity.size()));
	TMz = F<Floor>((TMz - mz_min) / delta);
	TIntensity = F<Log>(TIntensity + 1);
	mask_indices<Tensor<cpu, 1>, Tensor<cpu, 1>>(TMz, TIntensity, N);
	//* conduct sensing
	float *data_sub = new float[vMz.size() * sample];
	subA = Tensor<cpu, 2>(data_sub, Shape2(vMz.size(), pA->getSample()));
	subA = take(TMz, pA->tsA);
	float *data_result = new float[pA->getSample()];
	sensed = Tensor<cpu, 1>(data_result, Shape1(pA->getSample()));
	sensed = dot(TIntensity, subA);
	
	/* used for debugging
	printf("index:\n");
	VectorPrinter(TMz);
	printf("intensity:\n");
	VectorPrinter(TIntensity);
	printf("sensed:\n");
	VectorPrinter(sensed);
	*/

	VectorCSV(sensed, oFile);
	if (nSpectra % 200 == 0) printf("->");

	delete data_sub;
	delete data_result;
}

inline void Spectrum::turnFlag() {
	lineFlag = !lineFlag;
}