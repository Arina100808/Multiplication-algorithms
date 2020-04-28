#include "Python.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <fstream>
#include <string>

class Multiplicator {

public:
    std::vector<int> data;

    Multiplicator() = default;

    /// 2. Create a method that generates a random number of k-digits (vector)
    void randomizer(const int &k) {
        std::mt19937 generate(clock()); // clock - start point for the randomizer
        for (int i = 0; i < k; i++) {   // (each time different value)
            if (i == 0) {
                std::uniform_int_distribution<> x(1, 9);
                data.push_back(x(generate));
            }
            else {
                std::uniform_int_distribution<> y(0, 9);
                data.push_back(y(generate));
            }
        }
    }


    /// 3. Create auxiliary methods
    long long int digits2int(const std::vector<int> data){
        long long int res = 0;
        long long int p = 1;
        for (int i = data.size() - 1; i >= 0; --i) {
            res += data[i] * p;
            p *= 10;
        }
        return res;
    }


    std::vector<int> int2vec(const long long int x){
        std::vector<int> vv;
        std::string str = std::to_string(x);
        for (int i = 0; i < str.size(); i++){
            vv.push_back((int)str[i] - 48);
        }
        return vv;
    }


    std::vector<int> addzeros(std::vector<int> vv, const int y){
        for (int i = 1; i <= y; ++i){
            vv.push_back(0);
        }
        return vv;
    }


    /// multiply integer by 10^n
    long long int mult10n (int ab, const int n){
        std::vector<int> vab = int2vec(ab); /// int to vector
        std::vector<int> vab2 = addzeros(vab, n); /// add n zeros to the end
        long long int d = digits2int(vab2); /// vector to int
        return d;
    }



    void changesize(std::vector<int> &A, std::vector<int> &B) {
        int n = A.size();
        int k = B.size();
        if ((int) fmax(n, k) % 2 != 0) {
            if (n <= k) {
                B.insert(B.begin(), 0);
                for (int i = 0; i < k + 1 - n; i++) {
                    A.insert(A.begin(), 0);
                }
            }
            else {
                A.insert(A.begin(), 0);
                for (int i = 0; i < n + 1 - k; i++) {
                    B.insert(B.begin(), 0);
                }
            }
        }
        else if (((int) fmax(n, k) % 2 == 0) && n != k) {
            if (n < k) {
                for (int i = 0; i < k - n; i++) {
                    A.insert(A.begin(), 0);
                }
            }
            else {
                for (int i = 0; i < n - k; i++) {
                    B.insert(B.begin(), 0);
                }
            }
        }
    }


    void divide2(const std::vector<int> X, std::vector<int> &Xl, std::vector<int> &Xr){
        int n = X.size();
        int m = n/2;
        for(int j = 0; j < m; j++){
            Xl.push_back(X[j]);
        }
        for(int j = 0; j < n-m; j++){
            Xr.push_back(X[j+m]);
        }
    }


    std::vector<int> summ(const std::vector<int> x1, const std::vector<int> x2) {
        std::vector<int> X;
        int m = x1.size();
        for (int j = 0; j < m; j++) {
            for (int i = 0; i < m; i++) {
                X.push_back(x1[j] + x2[i]);
                j++;
            }
        }
        return X;
    }


    ///4.1. Grade School Multiplication algorithm + time
    double GradeSchool (std::vector<int> A, std::vector<int> B) {

        clock_t start = clock();
        int os, r = 0;

        std::vector<std::vector<int>> vvv;
        std::vector<int> temp;

        for(int i = B.size() - 1; i >= 0; i--){
            for(int n = 1; n <= B.size() - 1 - i; n++) {
                temp.push_back(0);
            }
            for(int k = A.size() - 1; k >= 0; k--){
                if (k == 0){
                    os = (A[k] * B[i] + r) % 10;
                    r = (A[k] * B[i] + r) / 10;
                    temp.push_back(os);
                    if(r != 0) {
                        temp.push_back(r);
                    }
                }
                else {
                    os = (A[k] * B[i] + r) % 10;
                    r = (A[k] * B[i] + r) / 10;
                    temp.push_back(os);
                }
            }
            reverse(temp.begin(), temp.end());
            vvv.push_back(temp);
            temp.clear();
            r = 0;
        }
        long long int intres = 0;
        long long int p;
        for (int i = 0; i < vvv.size(); i++) {
            p = digits2int(vvv[i]);
            intres += p;
        }
        clock_t end = clock();
        double dif = end - start;
        double res = 1000*(dif/CLOCKS_PER_SEC);
        return res;
    }


    ///4.2. Divide-and-Conquer Multiplication algorithm + time
    double DaC (std::vector<int> A, std::vector<int> B) {

        clock_t start = clock();

        long long int result = 0;
        int n = A.size();
        int k = B.size();

        if (n == 1 & k == 1) {
            result = A[0] * B[0];
        }
        else if (n == 2 & k == 2) {
            long long int d = mult10n(A[0] * B[0], 2);
            long long int dd = mult10n(A[0] * B[1] + A[1] * B[0], 1);
            result = d + dd + A[1] * B[1];
        }
        else {
            changesize(A, B);
            n = A.size();

            int m = n/2;

            std::vector<int> Al;
            std::vector<int> Ar;
            divide2(A, Al, Ar);
            std::vector<int> Bl;
            std::vector<int> Br;
            divide2(B, Bl, Br);

            long long int v1 = DaC(Al, Bl); // ac //
            long long int v2 = DaC(Ar, Bl); // bc
            long long int v3 = DaC(Al, Br); // ad
            long long int v4 = DaC(Ar, Br); // bd //

            long long int d = mult10n(v1, n);
            long long int dd = mult10n(v2+v3, n/2);
            result = d + dd + v4;
        }
        clock_t end = clock();
        double dif = end - start;
        double res = 1000*(dif/CLOCKS_PER_SEC);
        return res;
    }


    ///4.3. Karatsuba algorithm + time
    double Karatsuba (std::vector<int> A, std::vector<int> B) {

        clock_t start = clock();

        long long int result = 0;
        int n = A.size();
        int k = B.size();

        if (n == 1 & k == 1) {
            result = A[0] * B[0];
        }
        else {
            changesize(A, B);
            n = A.size();

            int m = n/2;

            std::vector<int> Al;
            std::vector<int> Ar;
            divide2(A, Al, Ar);
            std::vector<int> Bl;
            std::vector<int> Br;
            divide2(B, Bl, Br);

            std::vector<int> S1 = summ (Al, Ar); // S1 = a + b = Al + Ar
            std::vector<int> S2 = summ (Bl, Br); // S2 = c + d = Bl + Br

            long long int v1 = DaC(Al, Bl); // ac //
            long long int v2 = DaC(S1, S2); // (a+b)(c+d)
            long long int v4 = DaC(Ar, Br); // bd //

            long long int d = mult10n(v1, n);
            long long int dd = mult10n(v2-v1-v4, n/2);
            result = d + dd + v4;
        }
        clock_t end = clock();
        double dif = end - start;
        double res = 1000*(dif/CLOCKS_PER_SEC);
        return res;
    }


    ///// 5. Create a vector member for storing estimated results of experiments in milliseconds
    std::vector<std::vector<double>> ComputeTime(int numb){
        std::vector<std::vector<double>> allres;
        for (int n = 1; n <= numb; ++n){
            std::vector<double> tp = time4n(n);
            allres.push_back(tp);
        }
        return allres;
    }


    ////// 6. Create a method that performs calculation of a series of numbers containing from 1 to k digits
    std::vector<double> time4n(int n) {
        Multiplicator A;
        A.randomizer(n);
        Multiplicator B;
        B.randomizer(n);
        double tGS = GradeSchool(A.data, B.data);
        double tDaC = DaC(A.data, B.data);
        double tKar = Karatsuba(A.data, B.data);
        std::vector<double> temp = {tGS, tDaC, tKar};
        return temp;
    }


    /// 7. Create a method that outputs the experimental results stored in the vector as a CSV (text) file
    void outputfile (std::vector<std::vector<double>> vv) {
        std::ofstream FileTime;
        FileTime.open("TimeTest.csv");

        FileTime << "n" << ";" << "Grade School" << ";" << "Divide and Conquer" << ";" << "Karatsuba" << std::endl;
        for (int i = 0; i < vv.size(); i++) {
            FileTime << i+1 << ";" << vv[i][0] << ";" << vv[i][1] << ";" << vv[i][2] << std::endl;
        }
    }


    /// 8. Create a method that joins all the developed methods together and conducts experiments
    void RunExperiment(){
        std::vector<std::vector<double>> vv = ComputeTime(1000);
        outputfile(vv);
        Py_Initialize(); // initializing the interpreter
        PyRun_SimpleString("import matplotlib.pyplot as plt\n"
                           "import pandas as pd\n"
                           "url = '/Users/arina/CLionProjects/MultExperiment/cmake-build-debug/TimeTest.csv'\n"
                           "data = pd.read_csv(url, sep = \";\")\n"
                           "data.set_index(\"n\", inplace=True)\n"
                           "data.plot()\n"
                           "plt.title(\"Multiplying n-digit integers\")\n"
                           "plt.xlabel(\"n\")\n"
                           "plt.ylabel(\"Time (ms)\")\n"
                           "plt.grid()\n"
                           "plt.show()");
        Py_Finalize(); // clearing memory assigned to the interpreter
    }
};

int main() {

    Multiplicator A;
    A.RunExperiment();

}