#include <iostream>
#include <cmath>
#include "rational.h"
#include "matrix.h"
#include "binary_adder.h"
#include <complex>
#include <numbers>
#include <vector>
using namespace std;

const double Pi = 3.14159265358979323846;

//tutu
matrix<complex<double>> I({
    {1, 0},
    {0, 1}
    });

matrix<complex<double>> X_({
    {0, 1},
    {1, 0}
    });

matrix<complex<double>> Z_({
    {1, 0},
    {0, -1}
    });

matrix<complex<double>> Y_({
    {0, complex<double>(0, -1)},
    {complex<double>(0, 1), 0}
    });

matrix<complex<double>> H_({
    {sqrt(2)/2, sqrt(2) / 2},
    {sqrt(2) / 2, -sqrt(2) / 2}
    });

matrix<complex<double>> SWAP({
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

    std::vector<complex<double>> Vector;

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
        Vector.push_back(complex<double>(1, 0));
        Vector.push_back(complex<double>(0, 0));
    }

    Q_Sim(vector<complex<double>> a) : Vector(a)
    {
        if (!(log2(a.size()) == int(log2(a.size()))))
            throw 0;
        count_of_qubits = (int)log2(Vector.size());
    }

    complex<double>& operator[](int i)
    {
        return Vector[i];
    }

    void Ry(double alpha, int n)
    {
        matrix<complex<double>> ry({
            {cos(alpha/2), -sin(alpha/2)},
            {sin(alpha / 2), cos(alpha/2)}
            });
        Vector = Create_Gate(ry, n) * Vector;
    }
    void Rz(double alpha, int n)
    {
        matrix<complex<double>> rz({
            {complex<double>(cos(-alpha/2), sin(-alpha/2)), 0},
            {0, complex<double>(cos(alpha / 2), sin(alpha / 2))}
            });
        Vector = Create_Gate(rz, n) * Vector;
    }

    void X(int n)
    {
        Vector = Create_Gate(X_, n) * Vector;
    }

    void Z(int n)
    {
        Vector = Create_Gate(Z_, n) * Vector;
    }

    void Y(int n)
    {
        Vector = Create_Gate(Y_, n) * Vector;
    }

    void H(int n)
    {
        Vector = Create_Gate(H_, n) * Vector;
    }

    void P(double alpha, int n)
    {
        matrix<complex<double>> p({
            {1, 0},
            {0, complex<double>(cos(alpha), sin(alpha))}
            });
        Vector = Create_Gate(p, n) * Vector;
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

        Vector = CNOT * Vector;
    }

    friend ostream& operator<<(ostream& os, const Q_Sim& q)
    {
        for (const complex<double>& a : q.Vector)
        {
            if (a.imag() == 0)
                os << a.real() << endl;
            else
                os << '(' << a.real() << " + " << a.imag() << "i)" << endl;
        }
        return os;
    }
    
};


ostream& operator<<(ostream& os, const complex<double>& a)
{   
    if (a.imag() == 0)
        os << a.real();
    else
        os << '(' << a.real() << " + " << a.imag() << "i)";
    return os;
}



int main()
{   
    //Dense Coding
    /*Q_Sim q(vector<complex<double>>{1,2,3,4,0,0,0,0,0,0,0,0,0,0,0,0});
    q.H(2);
    q.CNOT(2, 3);
    q.CNOT(1, 2);
    q.H(2);
    q.CNOT(0, 2);
    q.H(2);
    q.CNOT(2, 3);
    q.H(2);
    
    cout << q;*/

    //


    string sum = Adder(Adder("10", "10"), Adder("1101", "1100"));

    cout << sum;
        

    return 0;
}

