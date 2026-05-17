#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;
double PI = 3.14159;

double funkcjax(double t, double f, double o) {
    double x = cos((2.0 * PI * t * f) + o) * cos(2.5 * pow(t, o) * PI);
    return x;
}
double funkcjay(double x, double t, double f, double o) {
    double y = x * t / (3 + cos(20 * PI * t));
    return y;
}
double funkcjaz(double x, double y, double t, double f, double o) {
    double z = (t * t) * abs(x * y - (2 / (10 + y)));
    return z;
}
double funkcjav(double z, double y, double x, double t, double f, double o) {
    double v = pow(z, 3) + 3 * sin(z * y * abs(y - x));
    return v;
}

int main() {
    double Tc = 5.0;
    double fs = 8500.0;
    double N = Tc * fs;
    std::vector<double> N_values;
    std::vector<double> x_values;
    std::vector<double> y_values;
    std::vector<double> z_values;
    std::vector<double> v_values;
    double t;
    for (int i = 0; i <= N - 1; i++) {
        t = i / fs;
        double f = 12.54;
        double o = 0.321;
        N_values.push_back(t);
        double x = funkcjax(t, f, o);
        x_values.push_back(x);
        double y = funkcjay(x, t, f, o);
        y_values.push_back(y);
        double z = funkcjaz(x, y, t, f, o);
        z_values.push_back(z);
        double v = funkcjav(z, y, x, t, f, o);
        v_values.push_back(v);
    }
    plt::figure();
    plt::plot(N_values, x_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("funkcja x");
    plt::title("Wykres funkcji x(t)");
    plt::show();

    plt::figure();
    plt::plot(N_values, y_values, "r-");
    plt::xlabel("czas");
    plt::ylabel("funkcja y");
    plt::title("Wykres funkcji y(t)");
    plt::show();

    plt::figure();
    plt::plot(N_values, z_values, "g-");
    plt::xlabel("czas");
    plt::ylabel("funkcja z");
    plt::title("Wykres funkcji z(t)");
    plt::show();

    plt::figure();
    plt::plot(N_values, v_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("funkcja z");
    plt::title("Wykres funkcji v(t)");
    plt::show();


}

// tabela 1




