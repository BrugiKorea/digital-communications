#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;
double PI = 3.14159;

double funkcjau(double t, double f, double o) {
    double u = 0.0;
    if (t >= 0.0 && t < 0.1) {
        u = sin(6.0 * PI * t) * cos(5.0 * PI * t);
    }
    else if (t >= 0.1 && t < 0.4) {
        u = -1.1 * t * cos(41.0 * PI * pow(t, 2.0));
    }
    else if (t >= 0.4 && t < 0.72) {
        u = t * sin(20.0 * pow(t, 4.0));
    }
    else if (t >= 0.72 && t < 1.0) {
        u = 3.3 * (t - 0.72) * cos(27.0 * t + 1.3);
    }
    return u;
}

int main() {
    double Tc = 5.0;
    double fs = 8500.0;
    double N = Tc * fs;
    std::vector<double> N_values;
    std::vector<double> u_values;

    double t;
    for (int i = 0; i <= N - 1; i++) {
        t = i / fs;
        double f = 12.54;
        double o = 0.321;
        N_values.push_back(t);
        double u = funkcjau(t, f, o);
        u_values.push_back(u);

    }
    plt::figure();
    plt::plot(N_values, u_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("funkcja u");
    plt::title("Wykres funkcji u(t)");
    plt::show();




}






