#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime>   

namespace plt = matplotlibcpp;
double PI = 3.14159;
double mt(double t, double fm) {
    double y = sin(2 * PI * fm * t);
    return y;
}

double modulacjaA(double m, double Ka, double fn, double t) {
    double za = (Ka * m + 1) * cos(2 * PI * fn * t);
    return za;
}
double modulacjaF(double fn, double m, double t, double kp) {
    double zp = cos(2 * PI * fn * t + kp * m);
    return zp;
}
double modulacjaC(double fn, double t, double fm, double m, double kf) {
    double zf = cos(2 * PI * fn * t + kf / fm * m);
    return zf;
}

int main() {
    double Tc = 3.0;
    double fs = 1000;
    double N = Tc * fs;
    std::vector<double> N_values;
    std::vector<double> fk_values;
    std::vector<double> tab_re;
    std::vector<double> tab_im;
    std::vector<double> y_values;
    std::vector<double> z_values;
    std::vector<double> M_value;
    double t;
    double Ka = 0.924;
    double fm = 2;
        double fn = 400;
  

    for (int i = 0; i <= N - 1; i++) {
        t = i / fs;
        N_values.push_back(t);
        double m = mt(t, fm); 
        z_values.push_back(modulacjaA(m, Ka, fn, t));

    }

    double a, b;
    double M;
    double fk;
    for (int k = 0; k < N; k++) {
        a = 0;
        b = 0;
        for (int n = 0; n < N; n++) {
            a += z_values[n] * cos(-1 * (2 * PI * k * n / N));
            b += z_values[n] * sin(-1 * (2 * PI * k * n / N));
        }
        tab_re.push_back(a);
        tab_im.push_back(b);
    }

    
    for (int k = 0; k <= N / 2 - 1; k++) {
        M = sqrt(pow(tab_re[k], 2) + pow(tab_im[k], 2));
        M_value.push_back(10 * log10(M));
        fk = k * fs / N;
        fk_values.push_back(fk);
    }
    
    plt::figure();
    plt::plot(N_values, z_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("sygnal z(a)_a");
    plt::show();

    plt::plot(fk_values, M_value, "b-");
    plt::xlabel("f(k)");
    plt::ylabel("M'(k)");
    plt::title("widmo z(a)_a");
    plt::show();
    return 0;
}
