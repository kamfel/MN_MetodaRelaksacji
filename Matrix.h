#pragma once
#include <string>
#include <fstream>
class Matrix
{
public:
	int n;
	double** A;

	Matrix(unsigned n);
	~Matrix();

	void SetMatrixA(double mat[5][5]);

	Matrix* GenerateAlphaMatrix();
	double* GenerateBetaVector(double* B);

	std::string ToString() const;

	double* GetEquationSolutions(double* Beta, double Epsilon, unsigned MLI, double*& Rr, unsigned& iter);

	bool LoadMatrix(std::ifstream& ifs, double* B);
};

