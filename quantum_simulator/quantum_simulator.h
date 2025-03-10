#pragma once
#include <iostream>
#include <cmath>
#include "matrix.h"
#include <complex>
#include <numbers>
#include <vector>
#include <algorithm>
using namespace std;

const double Pi = 3.14159265358979323846;

const matrix<complex<double>> I({
    {1, 0},
    {0, 1}
    });

const matrix<complex<double>> X_({
    {0, 1},
    {1, 0}
    });

const matrix<complex<double>> Z_({
    {1, 0},
    {0, -1}
    });

const matrix<complex<double>> Y_({
    {0, complex<double>(0, -1)},
    {complex<double>(0, 1), 0}
    });

const matrix<complex<double>> H_({
    {sqrt(2)/2, sqrt(2) / 2},
    {sqrt(2) / 2, -sqrt(2) / 2}
    });

const matrix<complex<double>> SWAP({
         {1, 0, 0, 0},
         {0, 0, 1, 0},
         {0, 1, 0, 0},
         {0, 0, 0, 1}
    });

/// <summary>
/// /////////////////////////////////////////////////////////////////////////////////////////////////
/// </summary>
class Q_Sim
{
private:

    std::vector<complex<double>> vector_state_;

    int count_of_qubits;

    matrix<complex<double>> Create_Gate(matrix<complex<double>> M, int n)
    {   
        for (int i = 0; i < n; ++i)
        {
            M = (M && I);
        }
        for (int j = n + 1; j < count_of_qubits; ++j)
            M = (I && M);

        return M;
    }
public:

    Q_Sim(): count_of_qubits(1)
    {
        vector_state_.push_back(complex<double>(1, 0));
        vector_state_.push_back(complex<double>(0, 0));
    }

    Q_Sim(vector<complex<double>> a) : vector_state_(a)
    {
        if (!(log2(a.size()) == int(log2(a.size()))))
            throw 0;
        count_of_qubits = (int)log2(vector_state_.size());
    }

    complex<double>& operator[](int i)
    {
        return vector_state_[i];
    }
    // Матричный Ry
    /*void Ry(double alpha, int n)
    {
        matrix<complex<double>> ry({
            {cos(alpha/2), -sin(alpha/2)},
            {sin(alpha / 2), cos(alpha/2)}
            });
        vector_state_ = Create_Gate(ry, n) * vector_state_;
    }*/

    // Битовый R
    void Ry(double alpha, int n)
    {
        matrix<complex<double>> ry({
            {cos(alpha / 2), -sin(alpha / 2)},
            {sin(alpha / 2), cos(alpha / 2)}
            });
        vector_state_ = Create_Gate(ry, n) * vector_state_;
    }
    void Rz(double alpha, int n)
    {
        matrix<complex<double>> rz({
            {complex<double>(cos(-alpha/2), sin(-alpha/2)), 0},
            {0, complex<double>(cos(alpha / 2), sin(alpha / 2))}
            });
        vector_state_ = Create_Gate(rz, n) * vector_state_;
    }

    void X(int n)
    {
        vector_state_ = Create_Gate(X_, n) * vector_state_;
    }

   
    void Z(int n)
    {
        //vector_state_ = Create_Gate(Z_, n) * vector_state_;

    }

    void Y(int n)
    {
        vector_state_ = Create_Gate(Y_, n) * vector_state_;
    }

    void H(int n)
    {
        vector_state_ = Create_Gate(H_, n) * vector_state_;
    }
    
    void P(double alpha, int n)
    {
        matrix<complex<double>> p({
            {1, 0},
            {0, complex<double>(cos(alpha), sin(alpha))}
            });
        vector_state_ = Create_Gate(p, n) * vector_state_;
    }

    void CNOT(int control_qbit, int target_qbit)
    {
        int N = 1 << count_of_qubits;
        matrix<complex<double>> CNOT(N, N);

        for (int i = 0; i < N; ++i) {
            int j = i;
            if ((i & (1 << control_qbit)) != 0) {
                j ^= (1 << target_qbit);
            }

            CNOT.numbers[i][j] = 1;
        }

        vector_state_ = CNOT * vector_state_;
        cout << CNOT;
    }

    friend ostream& operator<<(ostream& os, const Q_Sim& q)
    {
        for (const complex<double>& a : q.vector_state_)
        {
            if (a.imag() == 0)
                os << a.real() << endl;
            else
                os << '(' << a.real() << " + " << a.imag() << "i)" << endl;
        }
        return os;
    }
    
};





