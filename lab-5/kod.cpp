#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime>   
#include <string>
#include <cstring> 
#include "kiss_fft.h"


namespace plt = matplotlibcpp;
double PI = 3.14159;

std::vector<int> asciiToBinary(const std::string& text) {
    std::vector<int> result;
    for (char c : text) {
        if (c >= 32 && c <= 127) {
            for (int i = 0; i <= 6; i++) {
                int bit = (c >> i) & 1;
                result.push_back(bit);
            }
        }
        else {
            std::cout << "niepoprawny znak" << std::endl;
        }
    }
    return result;
}
void modulacja(std::vector<double>& in, double N, double A, double fn, double fs) {
    for (int i = 0; i < N; i++) {
        double t = i / fs;
        double mod = A * sin(2 * PI * fn * t);
        in[i] = mod * in[i];
    }
}
void modulacja_FSK(std::vector<double>& in, double N, double A, double fn1, double fn2, double fs, std::vector<double>& x1, std::vector<double>& x2) {
    for (int i = 0; i < N; i++) {
        double t = i / fs;
        double mod = A * sin(2 * PI * fn1 * t);
        x1.push_back(mod * in[i]);
    }
    for (int i = 0; i < N; i++) {
        double t = i / fs;
        double mod = A * sin(2 * PI * fn2 * t);
        x2.push_back(mod * in[i]);
    }
}
void ct(std::vector<double>& in, double N, double M, std::vector<double>& maxs) {
    double max = 0;


    for (int n = 1; n <= N; n++) {
        if (n % static_cast<int>(M) == 0) {
            maxs.push_back(in[n - 1]);
        }
        else {

        }
    }
    for (int i = 0; i < N; i++) {
        if (in[i] < 101) {
            in[i] = 0;
        }
        else {
            in[i] = 1;
        }
    }


    //  std::cout << "20. " << in[20] << "\n";
}
void ct_psk(std::vector<double>& in, double N, double M, std::vector<double>& maxs) {
    double max = 0;


    for (int n = 1; n <= N; n++) {
        if (n % static_cast<int>(M) == 0) {
            maxs.push_back(in[n - 1]);
        }
        else {

        }
    }
    for (int i = 0; i < N; i++) {
        if (in[i] < 0) {
            in[i] = 1;
        }
        else {
            in[i] = 0;
        }
    }

}
void cts_psk(std::vector<double>& out, double N, double M, std::vector<double>& maxs, double B, double Tb, std::vector<double>& N_values, double fs) {
    double t;
    out.clear();
    for (int n = 0; n < maxs.size(); n++) {
        if (maxs[n] < 0) {
            out.push_back(0);
        }
        else {
            out.push_back(1);
        }
    }
}
void cts(std::vector<double>& out, double N, double M, std::vector<double>& maxs, double B, double Tb, std::vector<double>& N_values, double fs) {
    double t;
    out.clear();
    for (int n = 0; n < maxs.size(); n++) {
        if (maxs[n] > 110) {
            out.push_back(1);
        }
        else {
            out.push_back(0);
        }
    }
}

void calka(std::vector<double>& in, double N, double A, double fn, double fs, double M) {
    double suma;
    for (int n = 0; n < N; n++) {
        if (n % static_cast<int>(M) == 0) {
            suma = 0;
        }
        else {
            suma = suma + in[n];
            in[n] = suma;
        }
    }
}
void calka_FSK(std::vector<double>& x1, std::vector<double>& x2, std::vector<double>& x3, double N, double A, double fn, double fs, double M) {
    double suma1;
    double suma2;
    for (int n = 0; n < N; n++) {
        if (n % static_cast<int>(M) == 0) {
            suma1 = 0;
        }
        else {
            suma1 = suma1 + x1[n];
            x1[n] = suma1;
        }
    }
    for (int n = 0; n < N; n++) {
        if (n % static_cast<int>(M) == 0) {
            suma2 = 0;
        }
        else {
            suma2 = suma2 + x2[n];
            x2[n] = suma2;
        }
    }
    for (int n = 0; n < N; n++) {
        x3[n] = (x1[n] - x2[n]);
    }
}


void ASK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B) {
    double t;
    double N = M * 10;
    for (int n = 0; n < B; n++) {

        if (binaryResult[n] == 0) {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(A1 * sin(2 * PI * fn * t));

            }

        }
        else {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(A2 * sin(2 * PI * fn * t));
            }

        }
    }



}

void ASK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs) {
    modulacja(out, N, A1, fn, fs);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("asksss_a");
    plt::show();

    calka(out, N, A1, fn, fs, M);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();

    ct(out, N, M, maxs);

    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();

    cts(out, N, M, maxs, B, Tb, N_values, fs);
    std::cout << "Wektor po demodulacji ";
    for (int i = 0; i < maxs.size();i++) {
        std::cout << out[i];
    }
    std::cout << std::endl;

}




void PSK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double fn, double M, double Tb, double fs, double B) {
    double t;
    for (int n = 0; n < B; n++) {
        if (binaryResult[n] == 0) {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(sin(2 * PI * fn * t));
            }
        }
        if (binaryResult[n] == 1) {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(sin(2 * PI * fn * t + PI));
            }
        }
    }



}

void PSK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs) {
    modulacja(out, N, A1, fn, fs);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("asksss_a");
    plt::show();
    calka(out, N, A1, fn, fs, M);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();
    ct_psk(out, N, M, maxs);

    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();
    cts_psk(out, N, M, maxs, B, Tb, N_values, fs);
    std::cout << "Wektor po demodulacji ";
    for (int i = 0; i < maxs.size();i++) {
        std::cout << out[i];
    }
    std::cout << std::endl;
}


void FSK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double fn1, double fn2, double M, double Tb, double fs, double B) {
    double t;
    for (int n = 0; n < B; n++) {
        if (binaryResult[n] == 0) {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(sin(2 * PI * fn1 * t));
            }
        }
        if (binaryResult[n] == 1) {
            for (int m = 0; m < M; m++) {
                t = n * Tb + m / fs;
                N_values.push_back(t);
                out.push_back(sin(2 * PI * fn2 * t));
            }
        }
    }


}

void FSK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn1, double fn2, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs, std::vector<double>& x1, std::vector<double>& x2, std::vector<double>& x3, double A, double fn) {
    modulacja_FSK(out, N, A1, fn1, fn2, fs, x1, x2);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("asksss_a");
    plt::show();
    //calka_FSK(x1, x2, x3, N, A1, fn, fs, M);
    plt::figure();
    plt::plot(N_values, x1, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();
    plt::figure();
    plt::plot(N_values, x2, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();
    calka_FSK(x1, x2, out, N, A, fn, fs, M);
    ct(out, N, M, maxs);
    plt::figure();
    plt::plot(N_values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("ask_a");
    plt::show();
    cts_psk(out, N, M, maxs, B, Tb, N_values, fs);
    std::cout << "Wektor po demodulacji ";
    for (int i = 0; i < maxs.size();i++) {
        std::cout << out[i];
    }
    std::cout << std::endl;

}


void DFT(double N, std::vector<double>& out, std::vector<double>& tab_re, std::vector<double>& tab_im, std::vector<double>& fk_values, std::vector<double>& M_value, double fs) {
    double a, b;
    double fk;
    double M;

    for (int k = 0; k < N; k++) {
        a = 0;
        b = 0;
        for (int n = 0; n < N; n++) {
            a += out[n] * cos(-1 * (2 * PI * k * n / N));
            b += out[n] * sin(-1 * (2 * PI * k * n / N));
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
}



int main() {
    double B = 10;
    std::string input = "niko";

    std::vector<int> binaryResult = asciiToBinary(input);
    std::cout << "Ciag bitow na wejsciu: " << std::endl;
    for (int i = 0; i < B; i++) {
        std::cout << binaryResult[i];
    }
    std::cout << "\n" << std::endl;

    std::vector<double> N_values;
    std::vector<double> out;
    std::vector<double> maxs;
    std::vector<double> x1;
    std::vector<double> x2;
    std::vector<double> x3;
    double Tc = 10.0;
    double fs = 200;
    double N = Tc * fs;
    double W = 2;
    double A1 = 1;
    double A2 = 2;
    double Tb = Tc / B;
    double M = Tb * fs;
    double fn = W * (1 / Tb);
    double fn1 = W + 1 / Tb;
    double fn2 = W + 3 / Tb;
    
    std::vector<int> values;
    for (int i = 0; i < 10; ++i) {
        values.push_back(i);
    }
    std::vector<int> binaryResultLimited;
    for (size_t i = 0; i < 10 && i < binaryResult.size(); ++i) {
        binaryResultLimited.push_back(binaryResult[i]);
    }

    //PSK(encoded, N_values, out, fn, M, Tb, fs, B);
    ASK(binaryResult, N_values, out, A1, A2, fn, M, Tb, fs, B);
    //FSK(encoded, N_values, out, fn1,fn2, M, Tb, fs, B);
     

    //PSK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);
    ASK_d(binaryResult, N_values, out, A1, A2, fn, M, Tb, fs, B, N, maxs);
    //FSK_d(encoded, N_values, out_m, A1, A2, fn1,fn2, M, Tb, fs, B, N, maxs,x1,x2,x3,A,fn);

    // Pierwszy wykres
    plt::plot(values, binaryResultLimited, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("wykres wektora wejsciowego bitow");

    // Drugi wykres
    plt::figure();
    plt::plot(values, out, "b-");
    plt::xlabel("czas");
    plt::ylabel("z(a)");
    plt::title("wykres wektora wyjsciowego bitow");

    plt::show();
    return 0;
}






