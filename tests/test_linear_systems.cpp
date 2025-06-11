#include "linear_systems.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

void testBasicSystem() {
    cout << "Test podstawowego układu równań..." << endl;
    vector<vector<double>> A = {{2, 1}, {8, 7}};
    vector<double> b = {3, 23};
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    assert(abs(x[0] - 1) < 1e-6);
    assert(abs(x[1] - 1) < 1e-6);
}

void test3x3System() {
    cout << "Test układu 3x3..." << endl;
    vector<vector<double>> A = {
        {2, -1, 0},
        {-1, 2, -1},
        {0, -1, 2}
    };
    vector<double> b = {1, 0, 1};
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Sprawdź rozwiązanie przez podstawienie do oryginalnego układu
    for (int i = 0; i < A.size(); i++) {
        double sum = 0;
        for (int j = 0; j < A[i].size(); j++) {
            sum += A[i][j] * x[j];
        }
        assert(abs(sum - b[i]) < 1e-10);
    }
}

void testIdentityMatrix() {
    cout << "Test macierzy jednostkowej..." << endl;
    vector<vector<double>> A = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };
    vector<double> b = {5, 3, 7};
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Dla macierzy jednostkowej x = b
    for (int i = 0; i < b.size(); i++) {
        assert(abs(x[i] - b[i]) < 1e-10);
    }
}

void testUpperTriangularMatrix() {
    cout << "Test macierzy górnotrójkątnej..." << endl;
    vector<vector<double>> A = {
        {2, 3, 1},
        {0, 4, 2},
        {0, 0, 3}
    };
    vector<double> b = {11, 10, 6};
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Sprawdź rozwiązanie
    assert(abs(x[2] - 2) < 1e-10); // z równania 3*x[2] = 6
    assert(abs(x[1] - 1.5) < 1e-10); // z równania 4*x[1] + 2*2 = 10
    assert(abs(x[0] - 1) < 1e-10); // z równania 2*x[0] + 3*1.5 + 1*2 = 11
}

void testLowerTriangularMatrix() {
    cout << "Test macierzy dolnotrójkątnej..." << endl;
    vector<vector<double>> A = {
        {2, 0, 0},
        {1, 3, 0},
        {4, 2, 5}
    };
    vector<double> b = {4, 7, 22};
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Sprawdź rozwiązanie przez podstawienie
    for (int i = 0; i < A.size(); i++) {
        double sum = 0;
        for (int j = 0; j < A[i].size(); j++) {
            sum += A[i][j] * x[j];
        }
        assert(abs(sum - b[i]) < 1e-10);
    }
}

void testSingularMatrix() {
    cout << "Test macierzy osobliwej..." << endl;
    vector<vector<double>> A = {
        {1, 2, 3},
        {2, 4, 6},  // Drugi wiersz = 2 * pierwszy
        {1, 1, 1}
    };
    vector<double> b = {6, 12, 3};
    
    try {
        vector<vector<double>> L, U;
        luDecomposition(A, L, U);
        
        // Sprawdź czy na przekątnej U pojawia się zero
        bool hasZeroDiagonal = false;
        for (int i = 0; i < U.size(); i++) {
            if (abs(U[i][i]) < 1e-12) {
                hasZeroDiagonal = true;
                break;
            }
        }
        
        if (hasZeroDiagonal) {
            cout << "Poprawnie wykryto macierz osobliwą (zero na przekątnej)" << endl;
        } else {
            // Spróbuj rozwiązać - może prowadzić do błędów numerycznych
            vector<double> z = forwardSubstitution(L, b);
            vector<double> x = backSubstitution(U, z);
            cout << "Rozwiązanie dla macierzy osobliwej: ";
            for (double val : x) cout << val << " ";
            cout << endl;
        }
    } catch (...) {
        cout << "Poprawnie wykryto błąd dla macierzy osobliwej" << endl;
    }
}

void testLargeSystem() {
    cout << "Test dużego układu równań..." << endl;
    int n = 5;
    vector<vector<double>> A(n, vector<double>(n));
    vector<double> b(n);
    
    // Utwórz macierz diagonalnie dominującą
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                A[i][j] = 10.0; // Dominująca przekątna
            } else {
                A[i][j] = 1.0;
            }
        }
        b[i] = 10.0 + (n-1); // Suma wierszy
    }
    
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Sprawdź rozwiązanie
    for (int i = 0; i < A.size(); i++) {
        double sum = 0;
        for (int j = 0; j < A[i].size(); j++) {
            sum += A[i][j] * x[j];
        }
        assert(abs(sum - b[i]) < 1e-8);
    }
}

void testIllConditionedMatrix() {
    cout << "Test źle uwarunkowanej macierzy..." << endl;
    vector<vector<double>> A = {
        {1, 1},
        {1, 1.0001}  // Prawie osobliwa
    };
    vector<double> b = {2, 2.0001};
    
    vector<vector<double>> L, U;
    luDecomposition(A, L, U);
    vector<double> z = forwardSubstitution(L, b);
    vector<double> x = backSubstitution(U, z);
    
    // Sprawdź czy rozwiązanie ma sens (może być niestabilne)
    cout << "Rozwiązanie źle uwarunkowanego układu: " << x[0] << ", " << x[1] << endl;
    
    // Sprawdź przez podstawienie
    for (int i = 0; i < A.size(); i++) {
        double sum = 0;
        for (int j = 0; j < A[i].size(); j++) {
            sum += A[i][j] * x[j];
        }
        assert(abs(sum - b[i]) < 1e-6); // Luźniejsza tolerancja
    }
}

int main() {
    try {
        testBasicSystem();
        test3x3System();
        testIdentityMatrix();
        testUpperTriangularMatrix();
        testLowerTriangularMatrix();
        testSingularMatrix();
        testLargeSystem();
        testIllConditionedMatrix();
        cout << "Wszystkie testy układów liniowych przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
