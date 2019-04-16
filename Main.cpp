#include "Matrix.h" 
#include <iostream>
#include <fstream>

void ReadParams(unsigned& n, unsigned& MLI, double& Epsilon) {
	std::cout << "n?\n";
	std::cin >> n;
	std::cout << "MLI?\n";
	std::cin >> MLI;
	std::cout << "Epsilon?\n";
	std::cin >> Epsilon;
}

void SaveToFile(const Matrix& A, const Matrix& Alpha, double Epsilon, unsigned MLI, 
	double* B, double* Beta, double* R, double* X, unsigned iter) 
{
	std::ofstream ofs("Raport.txt", std::ios_base::trunc | std::ios_base::out);
	ofs << "Macierz A:\n" << A.ToString();
	//TODO
}

int main() {
	unsigned n = 5;
	unsigned MLI = 60;
	double Epsilon = 0.0001f;
	double* X;
	Matrix* Alpha;
	double* Beta;

	ReadParams(n, MLI, Epsilon);
	Matrix A(n);
	double* B = new double[n];

	std::ifstream ifs("Zestaw1.txt");
	if (!ifs.is_open()) {
		std::cout << "Nie znaleziono pliku. " << std::endl;
		std::getchar();
		return -1;
	}

	if (!A.LoadMatrix(ifs, B)) {
		std::getchar();
		return -2;
	}
	try {
		Alpha = A.GenerateAlphaMatrix();
		Beta = A.GenerateBetaVector(B);
		X = Alpha->GetEquationSolutions(Beta, Epsilon, MLI);

		std::cout << "Wektor X: ";
		for (unsigned i = 0; i < n; i++) {
			std::cout << X[i] << ", ";
		}
		std::cout << std::endl;
	}
	catch (std::runtime_error& err) {
		std::cout << err.what() << std::endl;
	}
	std::getchar();

	return 0;
}