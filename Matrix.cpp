#include "Matrix.h"
#include <iostream>
#include <fstream>

Matrix::Matrix(unsigned n)
	:n(n)
{
	A = new double*[n];
	for (unsigned i = 0; i < n; i++){
		A[i] = new double[n];
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < n; i++){
		delete[] A[i];
	}
	delete[] A;
}

void Matrix::SetMatrixA(double mat[5][5]) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = mat[i][j];
		}
	}
}

Matrix* Matrix::GenerateAlphaMatrix() {
	Matrix* Alpha = new Matrix(n);

	for (int i = 0; i < n; i++) {
		if (!A[i][i]) throw std::runtime_error("Nie mozna wygenerowac macierzy ALFA - na przekatnej macierzy A istnieja zera.");
		for (int j = 0; j < n; j++){
			Alpha->A[i][j] = -A[i][j] / A[i][i];
		}
		Alpha->A[i][i] = 0;
	}
	return Alpha;
}

double* Matrix::GenerateBetaVector(double* B) {
	double* Beta = new double[n];

	for (int i = 0; i < n; i++) {
		if (!A[i][i]) throw std::runtime_error("Nie mozna wygenerowac wektora BETA - na przekatnej macierzy A istnieja zera.");
		Beta[i] = B[i] / A[i][i];
	}

	return Beta;
}

double* Matrix::GetEquationSolutions(double* Beta, double Epsilon, unsigned MLI) {
	double* X = new double[n];
	memset(X, 0, sizeof(double)* n);

	double* R = new double[n];
	memcpy(R, Beta, sizeof(double)* n);

	auto& Alpha = this->A;

	unsigned iterator = 0;
	unsigned s = 0;

	//Szukanie maksymalnego modu³u elementu wektora R
	for (int i = 1; i < n; i++) {
		if (abs(R[s]) < abs(R[i])){
			s = i;
		}
	}

	//Metoda relaksacji
	do {
		iterator++;
		X[s] = X[s] + R[s];
		for (int i = 0; i < n; i++) {
			if (i == s) continue;
			R[i] += Alpha[i][s] * R[s];
		}
		R[s] = 0;
		s = 0;
		for (int i = 1; i < n; i++) {
			if (abs(R[s]) < abs(R[i])) {
				s = i;
			}
		}
	} while (iterator < MLI && abs(R[s]) > Epsilon);

	return X;
}

enum STATE { NONE, MATRIX_OPENED, ROW, MATRIX_CLOSED, VECTOR, VECTOR_OPENED };

bool ValidateToken(char token, const std::string& expectedchars, unsigned line, unsigned item_in_line) {
	if (expectedchars.find(token) == std::string::npos) {
		std::cout << "Niespodziewany znak. " << "(" << line << ", " << item_in_line << ")\n";
		std::cout << "Spodziewano jednego z \"" << expectedchars << "\"" << std::endl;
		return false;
	}
	return true;
}

bool Matrix::LoadMatrix(std::ifstream& ifs, double* B)
{
	STATE state = NONE;
	char ch = 0;
	unsigned line = 0;
	unsigned item_in_line = 0;
	double* item_in_matrix = &A[0][0];
	double* item_in_vector = &B[0];
	unsigned row = 0;
	unsigned column = 0;
	unsigned in_vector = 0;

	while (ch = ifs.get()) {
		if (ch == '\n') {
			item_in_line = 0;
			line++;
			continue;
		}

		item_in_line++;

		if (isblank(ch))
			continue;

		switch (state)
		{
		case NONE:
			if (!ValidateToken(ch, "{", line, item_in_line))
				return false;

			state = MATRIX_OPENED;
			break;
		case MATRIX_OPENED:
			if (!ValidateToken(ch, "{,}", line, item_in_line))
				return false;
			if (ch == ',') {
				column = 0;
				item_in_matrix = &A[++row][column];
			}
			if (ch == '{') {
				state = ROW;
			}
			if (ch == '}') {
				state = MATRIX_CLOSED;
			}
			break;
		case ROW:
			if (!ValidateToken(ch, "0123456789,}", line, item_in_line))
				return false;
			switch (ch)
			{
			case ',':
				item_in_matrix = &A[row][++column];
				break;
			case '}':
				if (column != n - 1) {
					std::cout << "Niepoprawna ilosc liczb w wierszu macierzy. " << "(" << line << ", " << item_in_line << ")\n";
					return false;
				}
				state = MATRIX_OPENED;
				break;
			default:
				ifs.unget();
				ifs >> *item_in_matrix;
				break;
			}
			break;
		case MATRIX_CLOSED:
			if (!ValidateToken(ch, "&", line, item_in_line))
				return false;
			state = VECTOR;
			break;
		case VECTOR:
			if (!ValidateToken(ch, "{", line, item_in_line))
				return false;
			state = VECTOR_OPENED;
			break;
		case VECTOR_OPENED:
			if (!ValidateToken(ch, "0123456789,}", line, item_in_line))
				return false;
			switch (ch)
			{
			case ',':
				item_in_vector = &B[++in_vector];
				break;
			case '}':
				if (in_vector != n - 1) {
					std::cout << "Niepoprawna ilosc liczb w wierszu macierzy. " << "(" << line << ", " << item_in_line << ")\n";
					return false;
				}
				state = NONE;
				return true;
			default:
				ifs.unget();
				ifs >> *item_in_vector;
				break;
			}
			break;
		}
	}
	return false;
}
