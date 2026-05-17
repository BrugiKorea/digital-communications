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
    srand(time(NULL));


    for (int i = 0; i <= N - 1; i++) {
 
        double o = 0.321;
        double u = cos(o) + sin(o);
        x_values.push_back(u);
    }

    double a, b;
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

    return 0;
}





