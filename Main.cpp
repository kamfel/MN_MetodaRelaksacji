#include "Matrix.h" 
#include <iostream>
#include <fstream>
#include <iomanip>

void ReadParams(unsigned& n, unsigned& MLI, double& Epsilon) {
	std::cout << "n?\n";
	std::cin >> n;
	std::cout << "MLI?\n";
	std::cin >> MLI;
	std::cout << "Epsilon?\n";
	std::cin >> Epsilon;
}

void SaveToFile(const Matrix& A, const Matrix& Alpha, double Epsilon, unsigned n, unsigned MLI, 
	double* B, double* Beta, double* R, double* X, unsigned iter) 
{
	std::ofstream ofs("Raport.txt", std::ios_base::trunc);
	if (!ofs.is_open())
	{
		std::cout << "Blad przy tworzeniu pliku \"Raport.txt\"" << std::endl;
		return;
	}
	//Macierz A
	ofs << "Macierz A:\n" << A.ToString() << std::endl;
	
	//Wektor B
	ofs << "Wektor B:\n";
	for (unsigned i = 0; i < n; i++) {
		ofs << B[i] << " ";
	}
	ofs << std::endl;

	//Epsilon, MLI
	ofs << "Epsilon: " << Epsilon << std::endl;
	ofs << "MLI: " << MLI << std::endl;

	//Macierz ALFA
	ofs << "Macierz ALFA:\n" << Alpha.ToString() << std::endl;

	//Wektor BETA
	ofs << "Wektor BETA:\n";
	for (unsigned i = 0; i < n; i++) {
		ofs << Beta[i] << " ";
	}
	ofs << std::endl;

	//Set scientific notation
	ofs << std::scientific << std::setprecision(10);

	//Wektor Residuum
	ofs << "Wektor Residuum:\n";
	for (unsigned i = 0; i < n; i++) {
		ofs << R[i] << " ";
	}
	ofs << std::endl;

	//Wektor X
	ofs << "Wektor X:\n";
	for (unsigned i = 0; i < n; i++) {
		ofs << X[i] << " ";
	}
	ofs << std::endl;

	//Iteracje
	ofs << "Iloœæ iteracji: " << iter << std::endl;

	std::cout << "Zapisano do \"Raport.txt\"" << std::endl;

	ofs.close();
}

int main() {
	unsigned n = 5;
	unsigned MLI = 60;
	double Epsilon = 0.0001f;
	double* X;
	Matrix* Alpha;
	double* Beta;
	double* R = nullptr;
	unsigned iter;

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
		X = Alpha->GetEquationSolutions(Beta, Epsilon, MLI, R, iter);

		std::cout << "Wektor X: ";
		for (unsigned i = 0; i < n; i++) {
			std::cout << X[i] << ", ";
		}
		std::cout << std::endl;

		SaveToFile(A, *Alpha, Epsilon, n, MLI, B, Beta, R, X, iter);
	}
	catch (std::runtime_error& err) {
		std::cout << err.what() << std::endl;
	}

	system("pause");

	return 0;
}