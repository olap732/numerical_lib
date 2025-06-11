#include "interpolation.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

void testBasicInterpolation() {
    cout << "Test podstawowej interpolacji..." << endl;
    vector<double> x = { 0, 1, 2 };
    vector<double> y = { 1, 2, 3 };
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    double result = newtonInterpolation(1.5, x, coeffs);
    double expected = 2.5;
    assert(abs(result - expected) < 1e-6);
}

void testSinglePoint() {
    cout << "Test interpolacji pojedynczego punktu..." << endl;
    vector<double> x = { 3.0 };
    vector<double> y = { 7.0 };
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    double result = newtonInterpolation(3.0, x, coeffs);
    assert(abs(result - 7.0) < 1e-6);
    
    // Test w innym punkcie - powinien zwrócić stałą wartość
    double result2 = newtonInterpolation(5.0, x, coeffs);
    assert(abs(result2 - 7.0) < 1e-6);
}

void testTwoPoints() {
    cout << "Test interpolacji dwóch punktów..." << endl;
    vector<double> x = { 1, 3 };
    vector<double> y = { 2, 8 };
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    
    // Sprawdź punkty węzłowe
    assert(abs(newtonInterpolation(1, x, coeffs) - 2) < 1e-6);
    assert(abs(newtonInterpolation(3, x, coeffs) - 8) < 1e-6);
    
    // Sprawdź punkt środkowy (linia prosta: y = 3x - 1)
    double result = newtonInterpolation(2, x, coeffs);
    assert(abs(result - 5) < 1e-6);
}

void testQuadraticInterpolation() {
    cout << "Test interpolacji kwadratowej..." << endl;
    vector<double> x = { 0, 1, 2 };
    vector<double> y = { 1, 4, 9 }; // y = (x+1)^2
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    
    // Test punktów węzłowych
    for (int i = 0; i < 3; i++) {
        assert(abs(newtonInterpolation(x[i], x, coeffs) - y[i]) < 1e-6);
    }
    
    // Test punktu pośredniego
    double result = newtonInterpolation(0.5, x, coeffs);
    double expected = 2.25; // (0.5+1)^2 = 2.25
    assert(abs(result - expected) < 1e-6);
}

void testExtrapolation() {
    cout << "Test ekstrapolacji..." << endl;
    vector<double> x = { 0, 1, 2 };
    vector<double> y = { 0, 1, 4 }; // y = x^2
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    
    // Ekstrapolacja w lewo
    double result1 = newtonInterpolation(-1, x, coeffs);
    assert(abs(result1 - 1) < 1e-6); // (-1)^2 = 1
    
    // Ekstrapolacja w prawo
    double result2 = newtonInterpolation(3, x, coeffs);
    assert(abs(result2 - 9) < 1e-6); // 3^2 = 9
}

void testIdenticalXValues() {
    cout << "Test identycznych wartości x..." << endl;
    vector<double> x = { 1, 1, 2 };
    vector<double> y = { 3, 5, 7 };
    
    // To powinno prowadzić do dzielenia przez zero w dzielonych różnicach
    try {
        vector<double> coeffs = computeNewtonCoefficients(x, y);
        // Jeśli nie wyrzuci wyjątku, sprawdź czy wynik ma sens
        double result = newtonInterpolation(1.5, x, coeffs);
        cout << "Wynik dla identycznych x: " << result << endl;
    } catch (...) {
        cout << "Poprawnie wykryto błąd dla identycznych wartości x" << endl;
    }
}

void testLargeDataset() {
    cout << "Test dużego zbioru danych..." << endl;
    vector<double> x, y;
    // Generuj wielomian 5. stopnia
    for (int i = 0; i <= 10; i++) {
        double xi = i * 0.5;
        x.push_back(xi);
        y.push_back(xi*xi*xi - 2*xi*xi + xi + 1);
    }
    
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    
    // Test w kilku punktach
    for (int i = 0; i < x.size(); i++) {
        double result = newtonInterpolation(x[i], x, coeffs);
        assert(abs(result - y[i]) < 1e-10);
    }
}

void testConstantFunction() {
    cout << "Test funkcji stałej..." << endl;
    vector<double> x = { 1, 2, 3, 4 };
    vector<double> y = { 5, 5, 5, 5 };
    vector<double> coeffs = computeNewtonCoefficients(x, y);
    
    // Wszędzie powinno być 5
    assert(abs(newtonInterpolation(2.5, x, coeffs) - 5) < 1e-6);
    assert(abs(newtonInterpolation(10, x, coeffs) - 5) < 1e-6);
}

int main() {
    try {
        testBasicInterpolation();
        testSinglePoint();
        testTwoPoints();
        testQuadraticInterpolation();
        testExtrapolation();
        testIdenticalXValues();
        testLargeDataset();
        testConstantFunction();
        cout << "Wszystkie testy interpolacji przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
