#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

int funkcja(int x) {
    int y = x * x * x;
    return y;
}

int main() {
    std::vector<int> x_values;
    std::vector<int> y_values;

    for (int x = -20; x <= 20; x++) {
        x_values.push_back(x);
        y_values.push_back(funkcja(x));
    }
    for (int x = -20; x <= 20; x++) {
        std::cout << "f(" << x << ") = " << funkcja(x) << std::endl;
    }
    plt::plot(x_values, y_values, "b-");
    plt::xlabel("x");
    plt::ylabel("y");
    plt::title("Wykres funkcji y = x^3");
    plt::show();

    return 0;
}

// sposób wykorzystania bilbioteki jest inspirowany przykładami na stronie: https://github.com/lava/matplotlib-cpp/tree/master/examples