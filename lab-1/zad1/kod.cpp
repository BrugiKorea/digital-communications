#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;
double PI = 3.14159;

double funkcja(double t, double f, double o) {
    double y = cos((2.0 * PI * t * f) + o) * cos(2.5 * pow(t, o) * PI);
    return y;
}

int main() {
    double Tc = 5.0;
    double fs = 8500.0;
    double N = Tc * fs;
    std::vector<double> N_values;
    std::vector<double> y_values;
    double t;
    for (int i = 0; i <= N - 1; i++) {
        t = i / fs;
        N_values.push_back(t);
        y_values.push_back(funkcja(t, 12.54, 0.321));
    }
    plt::plot(N_values, y_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("y");
    plt::show();
}

// tabela 1