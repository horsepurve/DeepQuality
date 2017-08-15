#pragma once

#define _USE_MATH_DEFINES

#include "mshadow/tensor.h"
#include <math.h>
#include <iostream>
#include "Utility.h"

using namespace std;
using namespace mshadow;
using namespace mshadow::expr;

//* utility function declarations
template<typename Matrix2d> void _generate_center_coordinates(Matrix2d &m);

class SensingMatrix {
private:
	//* sample size of signal
	int Sample;
	//* original size of signal
	int N;
	//* linear size of image array
	int l_x;
	//* number of angles at which projections are acquired
	int n_dir;
public:
	//* tensor pointer to sensing matrix A, we should use this outside class
	TensorContainer<cpu, 2, float> tsA;
	SensingMatrix();
	SensingMatrix(int sample, int n);
	void Build();
	int getSample();
	~SensingMatrix() {};
};

SensingMatrix::SensingMatrix() {
	//* we only use this for testing
	l_x = 3;
	n_dir = 3;
	Build();
}

//* construct from sample, N to l_x, n_dir
SensingMatrix::SensingMatrix(int sample, int n):Sample(sample),N(n) {
	l_x = ceil(sqrt(N));
	n_dir = Sample / l_x;
	cout << "The sensing matrix is of size(we want): " << Sample << " * " << N << endl;
	cout << "The projection operator is of size: " << l_x << " * " << n_dir << endl;
	cout << "Ths sensing matrix is actually of size: " << l_x * n_dir << " * " << l_x * l_x << endl;
	Build();
}

//* after having got l_x and n_dir, we build the A
void SensingMatrix::Build() {
	printf("\nGenerating sensing matrix:\n");
	TensorContainer<cpu, 2> X(Shape2(l_x, l_x));
	//* generate coordinates X, Y
	_generate_center_coordinates(X);
	//* construct A
	tsA = TensorContainer<cpu, 2>(Shape2(l_x * l_x, l_x * n_dir)); //* we firstly construct the transpose of (Shape2(l_x * n_dir, l_x * l_x));
	//* fill with zeros
	tsA = 0.0f;

	//* indicator of columns
	//* NOTE: because Xrot is 2-D, then flatten, it's still 2-D, so, these must be 2-D too, and if Xrot is 1-D, these should also be 1-D
	TensorContainer<cpu, 1> index_col(Shape1(l_x * l_x)); 
	vector_climbed(index_col);
	//* indicator of rows, but we don't need this
	//* TensorContainer<cpu, 1> index_row(Shape1(l_x * l_x));
	//* values to fill A
	TensorContainer<cpu, 1> value(Shape1(l_x * l_x));
	//* we re-use the three tensors above in n_dir loops

	//* prepare some variables
	//TensorContainer<cpu, 2> Xrot(Shape2(l_x, l_x));
	//* why we use Tensor instead of TensorContainer: it's easy to flatten
	float *Xrot_data = new float[l_x * l_x];
	Tensor<cpu, 2> Xrot_(Xrot_data, Shape2(l_x, l_x)); //* this is used for accept data
	Tensor<cpu, 1> Xrot(Xrot_data, Shape1(l_x * l_x)); //* this is used for following steps
	float Xrot_min;
	float angle;

	//* because we need two runs
	//* TODO: eliminate these two variables, it's not elegant
	TensorContainer<cpu, 1> value2(Shape1(l_x * l_x));
	TensorContainer<cpu, 1> Xrot2(Shape1(l_x * l_x));

	//* loop by angles (same length as n_dir)
	for (int i = 0; i < n_dir; i++) {
		angle = M_PI / n_dir * i;

		//* TODO: Xrot is a intermediate variable, we need to eliminate it
		Xrot_ = cos(angle) * X - sin(angle) * X.T();
		//* NOTE: two sides of = must have same shape

		Xrot_min = matrix_min<TensorContainer<cpu, 2>, float>(X);
		value = (Xrot - Xrot_min) - F<Floor>(Xrot - Xrot_min);
		Xrot = Xrot - Xrot_min - value;
		//* now we get index_row, and value

		value2 = 1 - value;
		Xrot2 = Xrot + 1;

		//* mask those elements that indices < 0 or >= l_x
		mask_indices<Tensor<cpu, 1>, TensorContainer<cpu, 1>>(Xrot, value2, l_x);
		mask_indices<Tensor<cpu, 1>, TensorContainer<cpu, 1>>(Xrot2, value, l_x);

		//* fill the data
		tsA = mat_fill_row_element(tsA, value2, Xrot + l_x * i); //* this operation is bloody slow
		tsA = mat_fill_row_element(tsA, value, Xrot2 + l_x * i);
		//! MatrixPrinter(tsA);
		printf("->");
	}
	printf("Done!\n\n");
	delete Xrot_data;
}

inline int SensingMatrix::getSample() {
	return l_x * n_dir;
}
//* ---------------------------------------- BEGIN
//* for SensingMatrix construction

//* here we only generate X, and Y = X.T()
//* fill in the matrix, accept 2D matrix objects that have size
template<typename Matrix2d>
void _generate_center_coordinates(Matrix2d &m) {
	m[0] = 0.0f;
	//* TODO: or we can use repmat to do such
	for (int i = 1; i < m.size(0); i++) {
		m[i] = m[i - 1][0] + 1;
	}
	m += 0.5f - m.size(0) / 2.0f; //* NOTE: if we don't use 2.0f, it'll be int
}

//* ---------------------------------------- END
