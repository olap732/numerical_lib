#include "nonlinear_solver.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

double f1(double x) { return x * x - 2; }
double df1(double x) { return 2 * x; }

double f2(double x) { return x * x * x - x - 2; }
double df2(double x) { return 3 * x * x - 1; }

double f3(double x) { return sin(x) - 0.5; }
double df3(double x) { return cos(x); }

double f4(double x) { return exp(x) - 2; }
double df4(double x) { return exp(x); }

double f_zero_derivative(double x) { return x * x - 1; }
double df_zero_derivative(double x) { return 0; } // Zawsze zero!

double f_flat(double x) { return 0.0001 * (x - 1000); }
double df_flat(double x) { return 0.0001; }

void testBasicNewton() {
    cout << "Test podstawowej metody Newtona..." << endl;
    double wynik = newtonAnalytical(f1, df1, 1.0);
    assert(!isnan(wynik));
    assert(abs(wynik - sqrt(2)) < 1e-6);
}

void testCubicEquation() {
    cout << "Test równania sześciennego..." << endl;
    double wynik = newtonAnalytical(f2, df2, 1.5);
    assert(!isnan(wynik));
    assert(abs(f2(wynik)) < 1e-6); // f(root) ≈ 0
}

void testTrigonometricEquation() {
    cout << "Test równania trygonometrycznego..." << endl;
    double wynik = newtonAnalytical(f3, df3, 0.5);
    assert(!isnan(wynik));
    assert(abs(sin(wynik) - 0.5) < 1e-6);
    assert(abs(wynik - M_PI/6) < 1e-6); // sin(π/6) = 0.5
}

void testExponentialEquation() {
    cout << "Test równania wykładniczego..." << endl;
    double wynik = newtonAnalytical(f4, df4, 1.0);
    assert(!isnan(wynik));
    assert(abs(exp(wynik) - 2) < 1e-6);
    assert(abs(wynik - log(2)) < 1e-6);
}

void testZeroDerivative() {
    cout << "Test z zerową pochodną..." << endl;
    double wynik = newtonAnalytical(f_zero_derivative, df_zero_derivative, 0.5);
    // Powinno zwrócić NaN lub błąd gdy pochodna = 0
    assert(isnan(wynik));
    cout << "Poprawnie wykryto zerową pochodną" << endl;
}

void testConvergenceFromDifferentStarts() {
    cout << "Test zbieżności z różnych punktów startowych..." << endl;
    
    // Test dla f(x) = x^2 - 2, pierwiastek = sqrt(2)
    double wynik1 = newtonAnalytical(f1, df1, 1.0);
    double wynik2 = newtonAnalytical(f1, df1, 2.0);
    double wynik3 = newtonAnalytical(f1, df1, 0.1);
    
    assert(!isnan(wynik1) && !isnan(wynik2) && !isnan(wynik3));
    assert(abs(wynik1 - sqrt(2)) < 1e-6);
    assert(abs(wynik2 - sqrt(2)) < 1e-6);
    assert(abs(wynik3 - sqrt(2)) < 1e-6);
}

void testFlatFunction() {
    cout << "Test funkcji płaskiej..." << endl;
    // Funkcja bardzo płaska - może wymagać więcej iteracji
    double wynik = newtonAnalytical(f_flat, df_flat, 500.0, 1e-8, 1000);
    if (!isnan(wynik)) {
        assert(abs(f_flat(wynik)) < 1e-6);
        assert(abs(wynik - 1000) < 1e-3);
    } else {
        cout << "Nie osiągnięto zbieżności dla funkcji płaskiej" << endl;
    }
}

void testHighPrecision() {
    cout << "Test wysokiej precyzji..." << endl;
    double wynik = newtonAnalytical(f1, df1, 1.0, 1e-12, 100);
    assert(!isnan(wynik));
    assert(abs(wynik - sqrt(2)) < 1e-10);
}

void testMaxIterations() {
    cout << "Test maksymalnej liczby iteracji..." << endl;
    // Ustaw bardzo małą liczbę iteracji
    double wynik = newtonAnalytical(f1, df1, 1.0, 1e-12, 2);
    // Może nie osiągnąć zbieżności
    if (isnan(wynik)) {
        cout << "Poprawnie przerwano po osiągnięciu max iteracji" << endl;
    } else {
        cout << "Osiągnięto zbieżność w " << 2 << " iteracjach" << endl;
    }
}

void testNegativeRoot() {
    cout << "Test ujemnego pierwiastka..." << endl;
    // Szukaj ujemnego pierwiastka dla x^2 - 2 = 0
    double wynik = newtonAnalytical(f1, df1, -1.0);
    assert(!isnan(wynik));
    assert(abs(wynik + sqrt(2)) < 1e-6); // -sqrt(2)
}

void testMultipleRoots() {
    cout << "Test funkcji z wieloma pierwiastkami..." << endl;
    
    auto f_multiple = [](double x) { return (x-1)*(x-2)*(x-3); };
    auto df_multiple = [](double x) { return 3*x*x - 12*x + 11; };
    
    // Znajdź każdy pierwiastek z odpowiedniego punktu startowego
    double root1 = newtonAnalytical(f_multiple, df_multiple, 0.5);
    double root2 = newtonAnalytical(f_multiple, df_multiple, 1.5);
    double root3 = newtonAnalytical(f_multiple, df_multiple, 2.5);
    
    assert(abs(root1 - 1.0) < 1e-6);
    assert(abs(root2 - 2.0) < 1e-6);
    assert(abs(root3 - 3.0) < 1e-6);
}

int main() {
    try {
        testBasicNewton();
        testCubicEquation();
        testTrigonometricEquation();
        testExponentialEquation();
        testZeroDerivative();
        testConvergenceFromDifferentStarts();
        testFlatFunction();
        testHighPrecision();
        testMaxIterations();
        testNegativeRoot();
        testMultipleRoots();
        cout << "Wszystkie testy równań nieliniowych przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
