#include "ode_solver.h"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace std;

// y' = -2y + 1, rozwiązanie analityczne: y = 0.5(1 - exp(-2t)) + y0*exp(-2t)
double f1(double t, double y) { return -2 * y + 1; }
double analytical1(double t, double y0) { return 0.5 * (1 - exp(-2*t)) + y0 * exp(-2*t); }

// y' = y, rozwiązanie analityczne: y = y0 * exp(t)
double f2(double t, double y) { return y; }
double analytical2(double t, double y0) { return y0 * exp(t); }

// y' = t, rozwiązanie analityczne: y = 0.5*t^2 + y0
double f3(double t, double y) { return t; }
double analytical3(double t, double y0) { return 0.5 * t * t + y0; }

// y' = 0, rozwiązanie analityczne: y = y0 (stała)
double f4(double t, double y) { return 0; }

// y' = sin(t), rozwiązanie analityczne: y = -cos(t) + y0 + cos(0)
double f5(double t, double y) { return sin(t); }
double analytical5(double t, double y0) { return -cos(t) + y0 + 1; }

void testBasicODE() {
    cout << "Test podstawowego równania różniczkowego..." << endl;
    double y0 = 0.0, t0 = 0.0, t_end = 0.5;
    int steps = 10;
    vector<double> wyniki = rungeKutta4(f1, y0, t0, t_end, steps);
    
    double y_oczekiwane = analytical1(t_end, y0);
    double y_liczone = wyniki.back();
    assert(abs(y_liczone - y_oczekiwane) < 1e-3);
}

void testExponentialGrowth() {
    cout << "Test wzrostu wykładniczego..." << endl;
    double y0 = 1.0, t0 = 0.0, t_end = 1.0;
    int steps = 20;
    vector<double> wyniki = rungeKutta4(f2, y0, t0, t_end, steps);
    
    double y_oczekiwane = analytical2(t_end, y0); // e^1 = e
    double y_liczone = wyniki.back();
    assert(abs(y_liczone - M_E) < 1e-2);
}

void testPolynomialFunction() {
    cout << "Test funkcji wielomianowej..." << endl;
    double y0 = 2.0, t0 = 0.0, t_end = 2.0;
    int steps = 10;
    vector<double> wyniki = rungeKutta4(f3, y0, t0, t_end, steps);
    
    double y_oczekiwane = analytical3(t_end, y0); // 0.5*4 + 2 = 4
    double y_liczone = wyniki.back();
    assert(abs(y_liczone - 4.0) < 1e-6);
}

void testConstantFunction() {
    cout << "Test funkcji stałej..." << endl;
    double y0 = 5.0, t0 = 0.0, t_end = 10.0;
    int steps = 5;
    vector<double> wyniki = rungeKutta4(f4, y0, t0, t_end, steps);
    
    // Wszystkie wartości powinny być równe y0
    for (double val : wyniki) {
        assert(abs(val - y0) < 1e-10);
    }
}

void testTrigonometricFunction() {
    cout << "Test funkcji trygonometrycznej..." << endl;
    double y0 = 0.0, t0 = 0.0, t_end = M_PI;
    int steps = 50;
    vector<double> wyniki = rungeKutta4(f5, y0, t0, t_end, steps);
    
    double y_oczekiwane = analytical5(t_end, y0); // -cos(π) + 0 + 1 = 2
    double y_liczone = wyniki.back();
    assert(abs(y_liczone - 2.0) < 1e-3);
}

void testZeroSteps() {
    cout << "Test z zerowymi krokami..." << endl;
    double y0 = 3.0, t0 = 0.0, t_end = 1.0;
    int steps = 0;
    vector<double> wyniki = rungeKutta4(f1, y0, t0, t_end, steps);
    
    // Powinien zwrócić tylko wartość początkową
    assert(wyniki.size() == 1);
    assert(abs(wyniki[0] - y0) < 1e-10);
}

void testSingleStep() {
    cout << "Test z jednym krokiem..." << endl;
    double y0 = 1.0, t0 = 0.0, t_end = 0.1;
    int steps = 1;
    vector<double> wyniki = rungeKutta4(f2, y0, t0, t_end, steps);
    
    assert(wyniki.size() == 2); // y0 + 1 krok
    double y_oczekiwane = analytical2(t_end, y0);
    assert(abs(wyniki.back() - y_oczekiwane) < 1e-4);
}

void testLargeTimeInterval() {
    cout << "Test dużego przedziału czasowego..." << endl;
    double y0 = 0.1, t0 = 0.0, t_end = 5.0;
    int steps = 100;
    vector<double> wyniki = rungeKutta4(f1, y0, t0, t_end, steps);
    
    double y_oczekiwane = analytical1(t_end, y0);
    double y_liczone = wyniki.back();
    assert(abs(y_liczone - y_oczekiwane) < 1e-2);
}

void testNegativeTimeInterval() {
    cout << "Test ujemnego przedziału czasowego..." << endl;
    double y0 = 2.0, t0 = 1.0, t_end = 0.0; // Idź wstecz w czasie
    int steps = 10;
    vector<double> wyniki = rungeKutta4(f3, y0, t0, t_end, steps);
    
    // h będzie ujemne, sprawdź czy algorytm radzi sobie z tym
    assert(wyniki.size() == steps + 1);
    
    // Sprawdź czy końcowa wartość ma sens
    double h = (t_end - t0) / steps; // ujemne h
    double expected_t = t0 + steps * h; // powinno być t_end
    assert(abs(expected_t - t_end) < 1e-10);
}

void testStepSizeEffect() {
    cout << "Test wpływu wielkości kroku..." << endl;
    double y0 = 1.0, t0 = 0.0, t_end = 1.0;
    
    // Różne liczby kroków
    vector<double> wyniki_10 = rungeKutta4(f2, y0, t0, t_end, 10);
    vector<double> wyniki_100 = rungeKutta4(f2, y0, t0, t_end, 100);
    
    double analytical = analytical2(t_end, y0);
    double error_10 = abs(wyniki_10.back() - analytical);
    double error_100 = abs(wyniki_100.back() - analytical);
    
    // Więcej kroków powinno dać lepszą dokładność
    assert(error_100 < error_10);
    cout << "Błąd dla 10 kroków: " << error_10 << endl;
    cout << "Błąd dla 100 kroków: " << error_100 << endl;
}

void testStiffEquation() {
    cout << "Test sztywnego równania..." << endl;
    // y' = -1000*y + 1, bardzo szybka dynamika
    auto stiff_f = [](double t, double y) { return -1000 * y + 1; };
    
    double y0 = 0.0, t0 = 0.0, t_end = 0.01; // Krótki czas
    int steps = 100; // Dużo kroków dla stabilności
    
    vector<double> wyniki = rungeKutta4(stiff_f, y0, t0, t_end, steps);
    
    // Sprawdź czy nie ma eksplozji numerycznej
    for (double val : wyniki) {
        assert(!isnan(val) && !isinf(val));
        assert(abs(val) < 10); // Rozumna wielkość
    }
}

int main() {
    try {
        testBasicODE();
        testExponentialGrowth();
        testPolynomialFunction();
        testConstantFunction();
        testTrigonometricFunction();
        testZeroSteps();
        testSingleStep();
        testLargeTimeInterval();
        testNegativeTimeInterval();
        testStepSizeEffect();
        testStiffEquation();
        cout << "Wszystkie testy równań różniczkowych przeszły pomyślnie!" << endl;
    } catch (const exception& e) {
        cout << "Błąd w testach: " << e.what() << endl;
        return 1;
    }
    return 0;
}
