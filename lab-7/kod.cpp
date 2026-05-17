#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime>   
#include <string>
#include <cstring> 
#include <math.h>
#include <ctime>
#include "kiss_fft.h"
#include<Eigen/Dense>



using namespace Eigen;


namespace plt = matplotlibcpp;
double PI = 3.14159;

void modulacja(std::vector<double>& in, double N, double A, double fn, double fs) {
    for (int i = 0; i < N; i++) {
        double t = i / fs;
        double mod = A * sin(2 * PI * fn * t);
        in[i] = mod * in[i];
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

void modulacja_FSK(std::vector<double>& in, double N, double A, double fn1, double fn2, double fs, std::vector<double>& x1, std::vector<double>& x2) {
    x1.clear();
    x2.clear();
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
    maxs.clear();
    double max = 0;


    for (int n = 1; n <= N; n++) {
        if (n % static_cast<int>(M) == 0) {
            maxs.push_back(in[n - 1]);
        }
        else {

        }
    }
    for (int i = 0; i < N; i++) {
        if (in[i] < 50) {
            in[i] = 0;
        }
        else {
            in[i] = 1;
        }
    }


    //  std::cout << "20. " << in[20] << "\n";
}
void ct_psk(std::vector<double>& in, double N, double M, std::vector<double>& maxs) {
    maxs.clear();
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
            out.push_back(1);
        }
        else {
            out.push_back(0);
        }
    }
}
void cts(std::vector<double>& out, double N, double M, std::vector<double>& maxs, double B, double Tb, std::vector<double>& N_values, double fs) {
    double t;
    out.clear();
    for (int n = 0; n < maxs.size(); n++) {
        if (maxs[n] > 50) {
            out.push_back(1);
        }
        else {
            out.push_back(0);
        }
    }
}


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

void ASK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B) {
    N_values.clear();
    out.clear();
    double t;
    double N = M * B;
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

std::vector<double> ASK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs) {
    std::vector<double> out_copy = out; 
    modulacja(out_copy, N, A1, fn, fs);
    calka(out_copy, N, A1, fn, fs, M);
    ct(out_copy, N, M, maxs);
    cts(out_copy, N, M, maxs, B, Tb, N_values, fs);
    return out_copy;
}

void PSK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double fn, double M, double Tb, double fs, double B) {
    double t;
    N_values.clear();
    out.clear();
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

std::vector<double> PSK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs) {
    std::vector<double> out_copy = out;
    modulacja(out_copy, N, A1, fn, fs);
    calka(out_copy, N, A1, fn, fs, M);
    ct_psk(out_copy, N, M, maxs);
    cts_psk(out_copy, N, M, maxs, B, Tb, N_values, fs);
 

    return out_copy;
}


void FSK(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double fn1, double fn2, double M, double Tb, double fs, double B) {
    double t;
    N_values.clear();
    out.clear();
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
std::vector<double> FSK_d(std::vector<int>& binaryResult, std::vector<double>& N_values, std::vector<double>& out, double A1, double A2, double fn1, double fn2, double M, double Tb, double fs, double B, double N, std::vector<double>& maxs, std::vector<double>& x1, std::vector<double>& x2, std::vector<double>& x3, double A, double fn) {
    std::vector<double> out_copy = out;
    modulacja_FSK(out_copy, N, A1, fn1, fn2, fs, x1, x2);
    calka_FSK(x1, x2, out_copy, N, A, fn, fs, M);
    ct(out_copy, N, M, maxs);
    cts_psk(out_copy, N, M, maxs, B, Tb, N_values, fs);

    return out_copy;
}


void hamming_block(const std::vector<int>& binaryResult, std::vector<int>& hamming) {
    int x3, x5, x6, x7;
    x3 = binaryResult[0];
    x5 = binaryResult[1];
    x6 = binaryResult[2];
    x7 = binaryResult[3];

    int index = 0;
    for (int i = 0; i < 7; i++) {
        if (((i + 1) & ((i + 1) - 1)) == 0) {
            if (i == 0)
                hamming[i] = x3 ^ x5 ^ x7;
            else if (i == 1)
                hamming[i] = x3 ^ x6 ^ x7;
            else if (i == 3)
                hamming[i] = x5 ^ x6 ^ x7;
        }
        else {
            hamming[i] = binaryResult[index];
            index++;
        }
    }
}

void hamming_encode(const std::vector<int>& input, std::vector<int>& encoded, double B) {
    int n = input.size();
    for (int i = 0; i < n; i += 4) {
        std::vector<int> binaryResult(input.begin() + i, input.begin() + i + 4);
        std::vector<int> hamming(7);
        hamming_block(binaryResult, hamming);
        encoded.insert(encoded.end(), hamming.begin(), hamming.end());
    }
    std::cout << "Zakodowany wektor:    ";
    for (int i = 0; i < B; i++) {
        std::cout << encoded[i];
    }
    std::cout << "\n" << std::endl;
}

void hamming_decode_block(std::vector<int>& hamming) {
    int suma;
    do {
        int x1 = hamming[0] ^ hamming[2] ^ hamming[4] ^ hamming[6];
        int x2 = hamming[1] ^ hamming[2] ^ hamming[5] ^ hamming[6];
        int x4 = hamming[3] ^ hamming[4] ^ hamming[5] ^ hamming[6];

        int xs[3] = { x1, x2, x4 };
        suma = 0;
        for (int i = 0; i < 3; i++) {
            suma += xs[i] * std::pow(2, i);
        }

        if (suma != 0) {

            hamming[suma - 1] = !hamming[suma - 1];
        }
    } while (suma != 0);
}

std::vector<int> hamming_decode(std::vector<double>& encoded, double h) {
    std::vector<int> decoded;
    int n = encoded.size();
    for (int i = 0; i < n; i += 7) {
        std::vector<int> hamming(encoded.begin() + i, encoded.begin() + i + 7);
        hamming_decode_block(hamming);
        decoded.insert(decoded.end(), { hamming[2], hamming[4], hamming[5], hamming[6] });
    }
    
    return decoded;
}


int main() {
    srand(time(0));
    const static int q = 15;
    const static float c1 = (1 << q) - 1;
    const static float c2 = ((int)(c1 / 3)) + 1;
    const static float c3 = 1.f / c1;

 

    double B = 77;
    double h = 44;

    std::vector<int> hamming_w(7);
    std::vector<double> N_values;
    std::vector<double> out;
    std::vector<double> maxs;
    double Tc = 77.0;
    double fs = 200;
    double N = Tc * fs;
    double W = 2;
    double A1 = 1;
    double A2 = 0.5;
    double A = 1;
    std::vector<double> x1;
    std::vector<double> x2;
    std::vector<double> x3;
    double Tb = Tc / B;
    double M = Tb * fs;
    double fn = W * (1 / Tb);
    double fn1 = W + 1 / Tb;
    double fn2 = W + 3 / Tb;
    double E = 0;
    double BER;
    float random = 0.f;
    float noise = 0.f;


    std::string input = "nikolakovacaadwer";
    Matrix<int, 11, 4> P;
    //std::vector<int> binaryResult2 = asciiToBinary(input);
    std::vector<int> binaryResult = { 1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1 };
    std::vector<int> encoded;
    std::vector<int> decoded;
    std::vector<double> dmodulated;
    std::vector<double> szum;
    std::vector<double> out_m(N);
    std::vector<double> out_m2(N);

    double x;
    std::cout << "Hamming (7,4)  \n" << std::endl;
    std::cout << "Wektor wejsciowy:     ";
    for (int i = 0; i < h; i++) {
        std::cout << binaryResult[i];
    }
    std::cout << std::endl;




  
    //PSK(encoded, N_values, out, fn, M, Tb, fs, B);
    //ASK(encoded, N_values, out, A1, A2, fn, M, Tb, fs, B);
    //FSK(encoded, N_values, out, fn1,fn2, M, Tb, fs, B);


   

    hamming_encode(binaryResult, encoded, B);
    ASK(encoded, N_values, out, A1, A2, fn, M, Tb, fs, B);
    //ASK(encoded, N_values, out, A1, A2, fn, M, Tb, fs, B);
    std::vector<double> noise_values(N);
    std::vector<double> ber_values;
    std::vector<double> alpha_values;
    std::vector<double> beta_values;
    for (int i = 0; i < N; i++)
    {
        float random = ((float)rand() / (float)(RAND_MAX + 1));
        float noise = (2.f * ((random * c2) + (random * c2) + (random * c2)) - 3.f * (c2 - 1.f)) * c3;
        noise_values[i] = noise;
    }


    /*for (double alpha = 0; alpha < 1; alpha = alpha + 0.1) {
        for (int i = 0; i < N; i++)
        {
            double x = out[i];
            out_m[i] = (x + alpha * noise_values[i]);
        }

        
        std::cout << "alpha: " << alpha << std::endl;

        dmodulated = PSK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);

        std::cout << "Wektor po demodulacji:";
        for (int i = 0; i < maxs.size(); i++) {
            std::cout << dmodulated[i];
        }
        std::cout << std::endl;

        decoded = hamming_decode(dmodulated, h);

        int E = 0;
        for (int i = 0; i < 44; i++) {
            if (binaryResult[i] != decoded[i]) {
                E++;
            }
        }

        std::cout << "Wektor wyjsciowy:     ";
        for (int i = 0; i < h; i++) {
            std::cout << decoded[i];
        }
        std::cout << "\n";

        double BER = static_cast<double>(E) / 44.0 * 100; 

        std::cout << "BER: " << BER << "%\n";

        ber_values.push_back(BER);
        alpha_values.push_back(alpha);

        decoded.clear();
    }

    plt::plot(alpha_values, ber_values);
    plt::xlabel("Alpha");
    plt::ylabel("BER (%)");
    plt::title("BER vs Alpha");
    plt::show();
    */



    //ASK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);
    //ASK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);
    //hamming_decode(out_m, decoded, h);


    /*double e;
  
    for (double beta = 0; beta < 10; beta++) {
        for (int i = 0; i < N; i++)
        {
            
            e = exp(-1 * beta * i);
            double x = out[i];
            out_m[i] = (x * e);
        }


        std::cout << "alpha: " << beta << std::endl;

        dmodulated = PSK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);

        std::cout << "Wektor po demodulacji:";
        for (int i = 0; i < maxs.size(); i++) {
            std::cout << dmodulated[i];
        }
        std::cout << std::endl;

        decoded = hamming_decode(dmodulated, h);

        int E = 0;
        for (int i = 0; i < 44; i++) {
            if (binaryResult[i] != decoded[i]) {
                E++;
            }
        }

        std::cout << "Wektor wyjsciowy:     ";
        for (int i = 0; i < h; i++) {
            std::cout << decoded[i];
        }
        std::cout << "\n";

        double BER = static_cast<double>(E) / 44.0 * 100;

        std::cout << "BER: " << BER << "%\n";

        ber_values.push_back(BER);
        beta_values.push_back(beta);

        decoded.clear();
    }

    plt::plot(beta_values, ber_values);
    plt::xlabel("Alpha");
    plt::ylabel("BER (%)");
    plt::title("BER vs Beta");
    plt::show();
    
    */



double e;
double alpha = 0;

for (double beta = 0; beta < 10; beta++) {
    alpha = alpha + 0.1;
    for (int i = 0; i < N; i++)
    {

        e = exp(-1 * beta * i);
        double x = out[i];
        out_m[i] = (x * e);
    }
    for (int i = 0; i < N; i++)
    {
        double x = out_m[i];
        out_m2[i] = (x + alpha * noise_values[i]);
        
    }

    /*
    for (int i = 0; i < N; i++)
    {
        double x = out_m[i];
        out_m[i] = (x + alpha * noise_values[i]);
        
    }
    for (int i = 0; i < N; i++)
    {
        double x = out_m[i];
        out_m2[i] = (x + alpha * noise_values[i]);
        
    }
    */

    std::cout << "alpha: " << alpha << std::endl;
    std::cout << "beta: " << beta << std::endl;

    dmodulated = ASK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);

    std::cout << "Wektor po demodulacji:";
    for (int i = 0; i < maxs.size(); i++) {
        std::cout << dmodulated[i];
    }
    std::cout << std::endl;

    decoded = hamming_decode(dmodulated, h);

    int E = 0;
    for (int i = 0; i < 44; i++) {
        if (binaryResult[i] != decoded[i]) {
            E++;
        }
    }

    std::cout << "Wektor wyjsciowy:     ";
    for (int i = 0; i < h; i++) {
        std::cout << decoded[i];
    }
    std::cout << "\n";

    double BER = static_cast<double>(E) / 44.0 * 100;

    std::cout << "BER: " << BER << "%\n";

    ber_values.push_back(BER);
    beta_values.push_back(beta);
    alpha_values.push_back(alpha);

    decoded.clear();
}

plt::contour(alpha_values, beta_values, ber_values);
plt::xlabel("Alpha");
plt::ylabel("Beta");
plt::title("BER as function of Alpha and Beta");
plt::show();


    //PSK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);
    //ASK_d(encoded, N_values, out_m, A1, A2, fn, M, Tb, fs, B, N, maxs);
    //FSK_d(encoded, N_values, out_m, A1, A2, fn1,fn2, M, Tb, fs, B, N, maxs,x1,x2,x3,A,fn);

   
    return 0;
}






