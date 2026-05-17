#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>

namespace plt = matplotlibcpp;
double PI = 3.14159;

double funkcjab(double fs, double H, double t) {
    double sum = 0;
    for (double h = 1; h <= H; h++) {
        double wartosc = cos(4 * PI * h * t) / 4 * h * (sin(8 * PI * h * t) + 2);
        sum = sum + wartosc;
    }
    return sum;
}

int main() {
    double Tc = 1.0;
    double fs = 22050;
    double N = Tc * fs;
    double H1 = 1.0;
    double H2 = 5.0;
    double H3 = 50.0;
    std::vector<double> N_values;
    std::vector<double> u_values;

    double t;
    for (double i = 0; i <= N - 1; i++) {
        t = i / fs;
        N_values.push_back(t);
        double u = funkcjab(fs, H1, t);
        u_values.push_back(u);

    }

    plt::figure();
    plt::plot(N_values, u_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("wartosci");
    plt::title("Wykres funkcji b dla H1 ");
    plt::show();




}

// tabela 3