#include <iostream>
#include "matplotlibcpp.h"
#include <vector>
#include <cmath>
#include <cstdlib> 
#include <ctime>   
#include <string>
#include <cstring> 
#include "kiss_fft.h"
#include<Eigen/Dense>

using namespace Eigen;


namespace plt = matplotlibcpp;
double PI = 3.14159;


Matrix<int, 11, 15> createG(Matrix<int, 11, 4>& P) {
    Matrix<int, 11, 15> G;

    Matrix<int, 15, 4> build;
    for (int i = 0; i < 15; ++i) {
        build(i, 0) = ((i + 1) >> 3) & 1;
        build(i, 1) = ((i + 1) >> 2) & 1;
        build(i, 2) = ((i + 1) >> 1) & 1;
        build(i, 3) = (i + 1) & 1;
    }
 

    int row_p = 0;
    for (int row_build = 0; row_build < 15; ++row_build) {
        if ((row_build & (row_build + 1)) == 0)
            continue;

        for (int col = 0; col < 4; ++col) {
            P(row_p, col) = build(row_build, 3 - col);
        }
        ++row_p;
    }


    Matrix<int, 11, 11> I_k = Matrix<int, 11, 11>::Identity();
    G << P, I_k;
    return G;
}

Matrix<int, 4, 15> createH(const Matrix<int, 11, 4>& P) {

    Matrix<int, 4, 11> P_transposed = P.transpose();


    Matrix<int, 4, 4> I_m = Matrix<int, 4, 4>::Identity();


    Matrix<int, 4, 15> H;
    H << I_m, P_transposed;

   

    return H;
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


void hamming(std::vector<int>& binaryResult, std::vector<int>& hamming) {
    int index = 0;
    int x3, x5, x6, x7;
    x3 = binaryResult[0];
    x5 = binaryResult[1];
    x6 = binaryResult[2];
    x7 = binaryResult[3];

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
    std::cout << "Zakodowany wektor:  ";
    for (int i = 0; i < 7; i++) {
        std::cout << hamming[i] << " ";
    }
    std::cout << "\n";
}
void hamming2(std::vector<int>& binaryResult, std::vector<int>& hamming) {
    int index = 0;

    for (int i = 0; i < 15; i++) {
        if (((i + 1) & ((i + 1) - 1)) != 0) {
            hamming[i] = binaryResult[index];
            index++;
        }
        else {
            hamming[i] = 9;
        }
    }

}

void hamming_d(std::vector<int>& hamming) {

    hamming[5] = !hamming[5];
    std::cout << "Wektor wprowadzony do dekodera: ";
    for (int i = 0; i < 7; i++) {
        std::cout << hamming[i] << " ";

    }
    int suma;
    do {

        int x1 = hamming[0] ^ hamming[2] ^ hamming[4] ^ hamming[6];
        int x2 = hamming[1] ^ hamming[2] ^ hamming[5] ^ hamming[6];
        int x4 = hamming[3] ^ hamming[4] ^ hamming[5] ^ hamming[6];
        
        std::cout << "\n";

        int xs[3] = { x1, x2, x4 };
        suma = 0;
        for (int i = 0; i < 3; i++) {
            suma += xs[i] * std::pow(2, i);
        }
        
        if (suma != 0) {
            std::cout << "Wykryto blad, zmieniony zostal bit x" << suma;
            hamming[suma - 1] = !hamming[suma - 1];
        }

    } while (suma != 0);


    std::cout << "Wektor po dekodowaniu: ";
    std::cout << hamming[2] << " " << hamming[4] << " " << hamming[5] << " " << hamming[6] << "\n" << std::endl;
}
void decode_c(Matrix<int, 1, 4>& s, Matrix<int, 1, 15>& c, Matrix<int,15,4>& H_t) {
  
    
    int syndrom;
    do {
        s = c * H_t;
        for (int i = 0; i < 11; ++i) {
            s(i) = s(i) % 2;
        }
        syndrom = 0;
        for (int i = 0; i < s.size(); ++i) {
            syndrom += s(i) * std::pow(2, i);
        }
        if (syndrom != 0) {
            std::cout << "wykryto blad, zmieiony zostal bit x" << syndrom << "\n";
            if (syndrom == 1 || syndrom == 2 || syndrom == 4 || syndrom == 8) {
                std::wcout << "zostal zmieniony bit nalezy do struktury m, nie ma to wplywu na kod wejsciowy" << std::endl;
                syndrom = 0;
            }
            else if (syndrom == 3) {
                c[syndrom + 1] = !c[syndrom + 1];

            }
            else if (syndrom == 5 || syndrom == 6 || syndrom == 7) {
                c[syndrom] = !c[syndrom];
            }
            else {
                c[syndrom - 1] = !c[syndrom - 1];
            }
        }
        
       
    } while (syndrom != 0);
    Matrix<int, 1, 11> decoded = c.block<1, 11>(0, 4);
    std::cout << "Zadekodowany wektor " << decoded << std::endl;
   
}


int main() {
    double B = 4;
    std::string input = "zupa";
    Matrix<int, 11, 4> P;
    std::vector<int> binaryResult = asciiToBinary(input);
    std::vector<int> binaryResult2 = { 1,1,0,1 };
    std::vector<int> binaryResult3 = { 1,0,1,1,0,1,1,0,1,1,0 };
    std::cout << "Hamming (7,4)  \n" << std::endl;
    std::cout << "Wektor wejsciowy:   ";
    for (int i = 0; i < B; i++) {
        std::cout << binaryResult2[i] << " ";
    }
    std::cout << "\n";
    std::vector<int> hamming_w(7);
    hamming(binaryResult2, hamming_w);

    hamming_d(hamming_w);
    Matrix<int, 11, 15> G = createG(P);
    Matrix<int, 1, 11> d;
    d << 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0;
    Matrix<int, 1, 15> c = d * G;

    for (int i = 0; i < c.size(); ++i) {
        c(i) = c(i) % 2;
    }
    std::cout << "Hamming (15,11) \n" << std::endl;
    std::cout << "Wektor wejsciowy:          " << d << std::endl;
    std::cout << "Zakodowany wektor: " << c << std::endl;

    Matrix<int, 15, 4> H_t = createH(P).transpose();
    c(12) = !c(12);
    std::cout << "Wektor dekodowany  " << c << std::endl;
    Matrix<int, 1, 4> s;
    decode_c(s,c,H_t);
    return 0;
}






