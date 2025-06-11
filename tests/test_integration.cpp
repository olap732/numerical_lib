#include "integration.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

void testPolynomialIntegration() {
    cout << "Test całkowania wielomianu..." << endl;
    auto f = [](double x) { return x * x; }; // całka z x^2 od 0 do 1 = 1/3
    double result = gaussLegendre(f, 0.0, 1.0, 3);
    assert(abs(result - 1.0 / 3.0) < 1e-6);
}

void testConstantFunction() {
    cout << "Test całkowania funkcji stałej..." << endl;
    auto f = [](double x) { return 5.0; }; // całka = 5 * (b-a)
    double result = gaussLegendre(f, 0.0, 2.0, 2);
    assert(abs(result - 10.0) < 1e-6);
}

void testLinearFunction() {
    cout << "Test całkowania funkcji liniowej..." << endl;
    auto f = [](double x) { return 2*x + 3; }; // całka od 0 do 2 = [x^2 + 3x] = 4 + 6 = 10
    double result = gaussLegendre(f, 0.0, 2.0, 3);
    assert(abs(result - 10.0) < 1e-6);
}

void testSinFunction() {
    cout << "Test całkowania funkcji sin..." << endl;
    auto f = [](double x) { return sin(x); }; // całka sin(x) od 0 do π = 2
    double result = gaussLegendre(f, 0.0, M_PI, 4);
    assert(abs(result - 2.0) < 1e-3);
}

void testNegativeInterval() {
    cout << "Test całkowania na ujemnym przedziale..." << endl;
    auto f = [](double x) { return x * x; };
    double result = gaussLegendre(f, -1.0, 1.0, 3);
    assert(abs(result - 2.0/3.0) < 1e-6); // całka x^2 od -1 do 1 = 2/3
}

void testZeroInterval() {
    cout << "Test całkowania na zerowym przedziale..." << endl;
    auto f = [](double x) { return x * x + 5; };
    double result = gaussLegendre(f, 2.0, 2.0, 3);
    assert(abs(result) < 1e-10); // przedział zerowy = 0
}

void testReversedInterval() {
    cout << "Test całkowania na odwróconym przedziale..." << endl;
    auto f = [](double x) { return x; };
    double result1 = gaussLegendre(f, 0.0, 2.0, 3);
    double result2 = gaussLegendre(f, 2.0, 0.0, 3);
    assert(abs(result1 + result2) < 1e-6); // całka od a do b = - całka od b do a
}

void testDifferentOrders() {
    cout << "Test różnych rzędów kwadratury..." << endl;
    auto f = [](double x) { return x * x * x; }; // x^3
    double result2 = gaussLegendre(f, 0.0, 1.0, 2);
    double result3 = gaussLegendre(f, 0.0, 1.0, 3);
    double result4 = gaussLegendre(f, 0.0, 1.0, 4);
    double exact = 0.25; // całka x^3 od 0 do 1 = 1/4
    
    // Wyższy rząd powinien być dokładniejszy
    assert(abs(result4 - exact) < abs(result3 - exact));
    assert(abs(result3 - exact) < abs(result2 - exact));
}

void testOscillatoryFunction() {
    cout << "Test funkcji oscylacyjnej..." << endl;
    auto f = [](double x) { return sin(10*x); };
    double result = gaussLegendre(f, 0.0, 2*M_PI, 4);
    assert(abs(result) < 1e-2); // całka sin(10x) od 0 do 2π ≈ 0
}

int main() {
    try {
        testPolynomialIntegration();
        testConstantFunction();
        testLinearFunction();
        testSinFunction();
        testNegativeInterval();
        testZeroInterval();
        testReversedInterval();
        testDifferentOrders();
        testOscillatoryFunction();
        cout << "Wszystkie testy całkowania przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
