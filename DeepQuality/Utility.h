#pragma once

#include "mshadow/tensor.h"
#include <math.h>

using namespace std;
using namespace mshadow;
using namespace mshadow::expr;

//* ---------------------------------------- BEGIN
//* for matrix and vector printer, we don't change it, so use const

template<typename Matrix2d>
void MatrixPrinter(const Matrix2d &m) {
	for (int i = 0; i < m.size(0); ++i) {
		for (int j = 0; j < m.size(1); ++j) {
			printf(" %.2f |", m[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

template<typename Vector1d>
void VectorPrinter(const Vector1d &v) {
	for (int i = 0; i < v.size(0); i++) {
		printf(" %.2f |", v[i]);
	}
	printf("\n\n");
}

template<typename Vector1d>
void VectorCSV(const Vector1d &v, FILE * pFile) {
	fprintf(pFile, "%.4f", v[0]);
	for (int i = 1; i < v.size(0); i++) {
		fprintf(pFile, ",%.4f", v[i]);
	}
	fprintf(pFile, "\n");
}

//* ---------------------------------------- END

//* ---------------------------------------- BEGIN
//* some utility functions for matrix/vector manipulation

//* generage vector like python range, accept 1D vector objects that have size
template<typename Vector1d>
void vector_climbed(Vector1d &v) {
	v[0] = 0.0f;
	for (int i = 1; i < v.size(0); i++) {
		v[i] = v[i - 1] + 1;
	}
}

//* return the minimum value of a matrix
template<typename Matrix2d, typename DType>
DType matrix_min(const Matrix2d &m) {
	DType min = m[0][0];
	for (int i = 0; i < m.size(0); i++) {
		for (int j = 0; j < m.size(1); j++) {
			if (m[i][j] < min) min = m[i][j];
		}
	}
	return min;
}

//* define floor operation
struct Floor {
	MSHADOW_XINLINE static float Map(float a) {
		return  floor(a);
	}
};

//* define log operation
struct Log {
	MSHADOW_XINLINE static float Map(float a) {
		return  log(a);
	}
};

//* TODO: use template expression to do this
//* if index_col < 0 or >= l_x, then let index_col and value both equal 0
//* void mask_indices(Tensor<cpu, 1> &index_row, TensorContainer<cpu, 1> &value, const int l_x) {
template<typename Vector1d1, typename Vector1d2>
void mask_indices(Vector1d1 &index_row, Vector1d2 &value, const int l_x) {
	for (int i = 0; i < index_row.size(0); i++) {
		if (index_row[i] < 0 || index_row[i] >= l_x) { index_row[i] = 0.0f; value[i] = 0.0f; }
	}
}

//* ---------------------------------------- END
//* ---------------------------------------- Utility for Compressed Sensing END

