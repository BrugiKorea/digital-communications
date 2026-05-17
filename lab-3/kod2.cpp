#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime>   

namespace plt = matplotlibcpp;
double PI = 3.14159;
double mt(double t, double fm) {
    double m = sin(2 * PI * fm * t);
    return m;
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
    double Ka = 0.96;
    double fm = 2;
    double fn = fs / 4;
    double kp = 9.452;
    double kf = 0.52;

    for (int i = 0; i < N; i++) {
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

    for (double k = 0; k <= N / 2 - 1; k++) {
        M = sqrt(pow(tab_re[k], 2) + pow(tab_im[k], 2));
        M_value.push_back(10 * log10(M));
        fk = k * fs / N;
        fk_values.push_back(fk);
    }
    double max = M_value[0];
    for (double k = 0; k < N / 2 - 1; k++) {
        if (M_value[k] > max) {
            max = M_value[k];
        }
    }

    std::cout << "max: " << max << std::endl;
    plt::figure();
    plt::plot(N_values, z_values, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("sygnal z(a)_a");
    plt::show();

    std::vector<double> M_value_z;
    std::vector<double> fk_values_z;
    std::vector<double> kreska;
    double limit_left = 200;
    double limit_right = 300;

    for (double i = 0; i < fk_values.size(); i++) {
        if (fk_values[i] >= limit_left && fk_values[i] <= limit_right) {
            M_value_z.push_back(M_value[i]);
            fk_values_z.push_back(fk_values[i]);
            kreska.push_back(max - 3);
        }
    }
    int left_index;
    int right_index;
    for (double i = 0; i < fk_values_z.size(); ++i) {
        if (M_value_z[i] >= max - 3) {
            left_index = i;
            break;
        }
    }
    for (double i = fk_values_z.size() - 1; i >= 0; --i) {
        if (M_value_z[i] >= max - 3) {
            right_index = i;
            break;
        }
    }
    std::cout << "left : " << fk_values_z[left_index] << std::endl;
    std::cout << "right : " << fk_values_z[right_index] << std::endl;
    double width = fk_values_z[right_index] - fk_values_z[left_index];
    std::cout << "szerokosc pasma: " << width << std::endl;
    plt::plot(fk_values, M_value, "b-");
    plt::plot(fk_values_z, M_value_z, "b-");
    plt::plot(fk_values_z, kreska, "g-");
    std::cout << "Liczby w wektorze:" << std::endl;
    std::cout << fk_values[1];
    plt::xlabel("f(k)");
    plt::ylabel("M'(k)");
    plt::title("widmo z(p)_c");
    plt::show();
    return 0;
}
