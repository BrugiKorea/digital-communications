#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

namespace plt = matplotlibcpp;
double PI = 3.14159;

int main() {
    double Tc = 5.0;
    double fs = 20;
    double N = Tc * fs;
    std::vector<double> N_values;
    std::vector<double> tab_re;
    std::vector<double> tab_im;
    std::vector<double> x_values;
    std::vector<double> M_value;
    srand(time(NULL));


    for (int i = 0; i <= N - 1; i++) {

        double o = 0.321;
        double u = cos(o) + sin(o);
        x_values.push_back(u);
    }

    double a, b;
    double M;
    double fk;
    for (int k = 0; k < N; k++) {
        a = 0;
        b = 0;
        for (int n = 0; n < N; n++) {
            a += x_values[n] * cos(-1 * (2 * PI * k * n / N));
            b += x_values[n] * sin(-1 * (2 * PI * k * n / N));
        }
        tab_re.push_back(a);
        tab_im.push_back(b);
    }

    for (int i = 0; i < tab_re.size(); ++i) {
        std::cout << "Tab[" << i << "] = " << tab_re[i] << " + " << tab_im[i] << "i" << std::endl;
    }
    for (int k = 0; k < N / 2 - 1; k++) {
        M = sqrt(pow(tab_re[k], 2) + pow(tab_im[k], 2));
        M_value.push_back(10 * log10(M));
        fk = k * fs / N;
        N_values.push_back(fk);
    }
    plt::figure();
    plt::plot(N_values, M_value, "b-");
    plt::xlabel("f(k)");
    plt::ylabel("M'(k)");
    plt::title("widmo");
    plt::show();
    return 0;
}
