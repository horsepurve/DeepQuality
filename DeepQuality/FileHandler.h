#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>  
#include <ctime> 
#include "Spectrum.h"
#include <string>

//* for JSON parsing
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

const int MAX_LINE = 1000; //* maximun length of one line

using namespace std;

template <typename DoLineFunc>
void load_file(const char* file, const char *ofile, DoLineFunc doline, Spectrum &sp) {
	//* for output data csv
	FILE * oFile;
	errno_t oerr;
	oerr = fopen_s(&oFile, ofile, "w");
	if (oerr != 0) {
		printf("Error opening file\n");
	}

	//* time it
	time_t start, end;
	start = clock();
	//! cout << "Loading file: " << file << " ..." << endl;

	char input_line[MAX_LINE];
	char *result;
	//! int line_count = 0;

	FILE * pFile;
	errno_t err;
	err = fopen_s(&pFile, file, "r");
	if (err != 0)	{
		cout << "Error opening file\n";
	}
	
	else {
		while ((result = fgets(input_line, MAX_LINE, pFile)) != NULL)	{
			//! line_count++;
			doline(result, oFile, sp);
		}
		fclose(pFile);
	}
	
	end = clock();
	//! cout << "Loading file end, costing time: " << end - start << " ms" << endl;

	//* output end
	fclose(oFile);
}

//* ---------------------------------------- BEGIN
//* for file reading and converting

void doline(char* line, FILE * oFile, Spectrum &sp) {
	if (line[0] < 48 || line[0] > 57) { //* we got a abc here
		//* handle empty spectrum
		//* got a BEGIN here
		if (line[0] == 66) {
			if (sp.hasBegin) {
				sp.reBuild();
				sp.addPeak(0.0, 0.0);
				sp.CS(oFile);
			}
			else {
				sp.hasBegin = true;
			}
		}
		if (sp.lineFlag) {
			sp.CS(oFile);
			sp.turnFlag();
			sp.hasBegin = false;
		}
	}
	else { //* we got number here
		float mz, intensity;
		char *pch1 = strchr(line, ' ');
		char *pch2 = strchr(line, '\n');
		pch2[0] = '\0'; //* cut at end
		if (pch1 != NULL) {
			pch1[0] = '\0'; //* cut at space
			pch2 = strchr(pch1 + 1, ' ');
			if (pch2 != NULL) pch2[0] = '\0'; //* if there is the second space, cut
			//* now we have 1. line, 2. pch+1
			mz = atof(line);
			intensity = atof(pch1 + 1);
			//*printf("%f--%f\n", mz, intensity);
			if (sp.lineFlag) sp.addPeak(mz, intensity);
			else {
				sp.reBuild();
				sp.addPeak(mz, intensity);
				sp.turnFlag();
			} //* end doing spectrum
		}
	} //* end chose abc and 123
}

//* ---------------------------------------- END

//* read configuration file for compressed sensing, deep learning, and gradient boosting
struct Parameter {
	//* CompressedSensing
	float min_mz;
	float max_mz;
	float delta;
	int sample;
	bool train_only;
	bool test_only;
	string train_mgf;
	string test_mgf;
	//* DeepLearning
	string train_label;
	string test_label;
	bool use_mlp;
	bool use_cnn;
	int mlp_nh;
	float mlp_lr;
	int mlp_batch;
	
	float cnn_lr;
	int cnn_batch;
	//* GradientBoosting
	string novor_train;
	string novor_test;
	int round;
public:
	void getParameter(Document &d);
};

void Parameter::getParameter(Document &d) {
	//* CompressedSensing
	Value& v = d["CompressedSensing"];
	min_mz = v["min_mz"].GetDouble();
	max_mz = v["max_mz"].GetDouble();
	delta = v["delta"].GetDouble();
	sample = v["sample"].GetInt();
	train_only = v["train_only"].GetBool();
	test_only = v["test_only"].GetBool();
	train_mgf = v["train_mgf"].GetString();
	test_mgf = v["test_mgf"].GetString();
	//* DeepLearning
	v = d["DeepLearning"];
	train_label = v["train_label"].GetString();
	test_label = v["test_label"].GetString();
	use_mlp = v["use_mlp"].GetBool();
	use_cnn = v["use_cnn"].GetBool();
	mlp_nh = v["mlp"]["nh"].GetInt();
	mlp_lr = v["mlp"]["lr"].GetDouble();
	mlp_batch = v["mlp"]["batch"].GetInt();

	cnn_lr = v["cnn"]["lr"].GetDouble();
	cnn_batch = v["cnn"]["batch"].GetInt();
	//* GradientBoosting
	v = d["GradientBoosting"];
	novor_train = v["novor_train"].GetString();
	novor_test = v["novor_test"].GetString();
	round = v["round"].GetInt();

	printf("Reading parameter file done!\n");
}

void read_config(const char * config_file, Parameter &p) {
	//* read the whole file
	FILE * pFile;
	errno_t err;
	err = fopen_s(&pFile, config_file, "rb");
	if (err != 0) {
		cout << "Error opening file\n";
	}

	fseek(pFile, 0, SEEK_END);
	int size = ftell(pFile);
	rewind(pFile);

	char *buffer = (char*)malloc(sizeof(char)*size);
	int result = fread(buffer, 1, size, pFile);
	buffer[size] = '\0';
	//* read the whole file end
	//! printf("%s\n", buffer);

	Document d;
	d.Parse(buffer);
	
	p.getParameter(d);
}