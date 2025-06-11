// test_approximation.cpp - Rozbudowane testy aproksymacji
#include "approximation.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

void testBasicApproximation() {
    cout << "Test podstawowej aproksymacji..." << endl;
    vector<double> x = { 0, 1, 2 };
    vector<double> y = { 1, 3, 7 }; // y = 1 + x + x^2
    vector<double> coeffs = computeLeastSquares(x, y, 2);
    assert(abs(coeffs[0] - 1.0) < 1e-6);
    assert(abs(coeffs[1] - 1.0) < 1e-6);
    assert(abs(coeffs[2] - 1.0) < 1e-6);
    double val = evaluatePolynomial(3, coeffs);
    assert(abs(val - 13.0) < 1e-6);
}

void testLinearApproximation() {
    cout << "Test aproksymacji liniowej..." << endl;
    vector<double> x = { 0, 1, 2, 3 };
    vector<double> y = { 1, 3, 5, 7 }; // y = 1 + 2x
    vector<double> coeffs = computeLeastSquares(x, y, 1);
    assert(abs(coeffs[0] - 1.0) < 1e-6);
    assert(abs(coeffs[1] - 2.0) < 1e-6);
}

void testSinglePoint() {
    cout << "Test pojedynczego punktu..." << endl;
    vector<double> x = { 5.0 };
    vector<double> y = { 7.0 };
    vector<double> coeffs = computeLeastSquares(x, y, 0);
    assert(abs(coeffs[0] - 7.0) < 1e-6);
    double val = evaluatePolynomial(5.0, coeffs);
    assert(abs(val - 7.0) < 1e-6);
}

void testHighDegreePolynomial() {
    cout << "Test wielomianu wysokiego stopnia..." << endl;
    vector<double> x = { 0, 1, 2, 3, 4 };
    vector<double> y = { 1, 2, 9, 28, 65 }; // y = 1 + x + x^3
    vector<double> coeffs = computeLeastSquares(x, y, 3);
    double val = evaluatePolynomial(2.5, coeffs);
    assert(val > 0); // Sprawdź czy funkcja nie crashuje
}

void testNoisyData() {
    cout << "Test danych z szumem..." << endl;
    vector<double> x = { 0, 1, 2, 3, 4, 5 };
    vector<double> y = { 1.1, 2.9, 5.2, 6.8, 9.1, 10.9 }; // ~2x + 1 + szum
    vector<double> coeffs = computeLeastSquares(x, y, 1);
    assert(abs(coeffs[1] - 2.0) < 0.2); // Tolerancja na szum
}

void testZeroPolynomial() {
    cout << "Test wielomianu zerowego..." << endl;
    vector<double> x = { 1, 2, 3 };
    vector<double> y = { 0, 0, 0 };
    vector<double> coeffs = computeLeastSquares(x, y, 1);
    assert(abs(coeffs[0]) < 1e-6);
    assert(abs(coeffs[1]) < 1e-6);
}

void testConstantFunction() {
    cout << "Test funkcji stałej..." << endl;
    vector<double> x = { 1, 2, 3, 4 };
    vector<double> y = { 5, 5, 5, 5 };
    vector<double> coeffs = computeLeastSquares(x, y, 0);
    assert(abs(coeffs[0] - 5.0) < 1e-6);
}

void testOverdeterminedSystem() {
    cout << "Test systemu nadokreślonego..." << endl;
    vector<double> x = { 0, 1, 2, 3, 4, 5, 6 };
    vector<double> y = { 1, 3, 5, 7, 9, 11, 13 }; // y = 1 + 2x
    vector<double> coeffs = computeLeastSquares(x, y, 1);
    assert(abs(coeffs[0] - 1.0) < 1e-6);
    assert(abs(coeffs[1] - 2.0) < 1e-6);
}

int main() {
    try {
        testBasicApproximation();
        testLinearApproximation();
        testSinglePoint();
        testHighDegreePolynomial();
        testNoisyData();
        testZeroPolynomial();
        testConstantFunction();
        testOverdeterminedSystem();
        cout << "Wszystkie testy aproksymacji przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
