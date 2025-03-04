#include <iostream>
#include <cmath>
#include "rational.h"
#include "matrix.h"
#include <complex>
#include <numbers>
#include <vector>
using namespace std;

const double Pi = 3.14159265358979323846;

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

    matrix<complex<double>> Create_Gate(matrix<complex<double>> M, int n)
    {   

        int count_matrix = log2(Vector.size());


        for (int i = 0; i < n; ++i)
        {
            M = (I && M);
        }
        for (int j = n + 1; j < count_matrix; ++j)
            M = M && I;

        return M;
    }

public:

    int count_of_qubits;

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
            if ((i & (1 << (count_of_qubits - control_qbit - 1))) != 0) { // Если управляющий бит = 1
                j ^= (1 << (count_of_qubits - target_qbit - 1)); // Флип целевого бита
            }
            CNOT.numbers[i][j] = 1;
        }
        Vector = CNOT * Vector;
    }

    matrix<complex<double>> CZ(int a, int b) // ?
    {
        if (a == 0 && b == 1)
            return matrix<complex<double>>({
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, -1}
                });
        else if (a == 1 && b == 0)
            return matrix<complex<double>>({
                { 1, 0, 0, 0 },
                { 0, 1, 0, 0 },
                { 0, 0, 1, 0 },
                { 0, 0, 0, -1 }
                });
        else
            throw 0;
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

void Dense_Coding(Q_Sim& qsim, int alice, int bob) {
    // 1. Подготовка запутанного состояния Белла |Φ+> = (|00> + |11>)/√2
    qsim.H(alice);
    qsim.CNOT(alice, bob);

    cout << qsim << endl;
    // 2. Алиса кодирует свой кубит, применяя X и Z по необходимости
    if (alice)
        qsim.X(alice);  // Симулируем случайную информацию (замени на нужную)
    if (bob)
        qsim.Z(alice);

    cout << qsim << endl;
    // 3. Алиса измеряет свой кубит (неявно, она передает классические биты Бобу)

    // 4. Боб декодирует сообщение
    qsim.CNOT(alice, bob);
    qsim.H(alice);

    cout << qsim << endl;
}

int main()
{   
    Q_Sim q(vector<complex<double>>{0, 0,0,0,0,0,0,0,1,0,0,0,0,0,0,0});

    q.H(2);
    q.CNOT(2, 3);
    q.CNOT(1, 2);
    q.H(2);
    q.CNOT(0, 2);
    q.H(2);
    q.CNOT(2, 3);
    q.H(2);
    
    cout << q;
    /*Q.Ry(Pi / 2, 0);
    Q.X(0);
    Q[0] = 1;
    std::cout << Q;*/
   // matrix<complex<double>> Ry({ {sqrt(2)/2, -sqrt(2) / 2}, {sqrt(2) / 2, sqrt(2) / 2}});
   // matrix<double> I({ {1, 0}, {0, 1} });
   // matrix<double> X({ {0, 1}, {1, 0} });
   // matrix<complex<double>> CNOT01({
   //     {1, 0, 0, 0},
   //     {0, 1, 0, 0},
   //     {0, 0, 0, 1},
   //     {0, 0, 1, 0}
   //     });
   // matrix<complex<double>> CNOT10({
   //     {1, 0, 0, 0},
   //     {0, 0, 0, 1},
   //     {0, 0, 1, 0},
   //     {0, 1, 0, 0}
   //     });
   // matrix<complex<double>> SWAP = CNOT01 * CNOT10 * CNOT01;
   // /*std::vector<double> vec1 = {1, 2, 3, 4};
   // vec1 = (I && X) * vec1;*/
   // //vec = SWAP * vec;
   // //for (auto i : vec)
   // //    std::cout << i << std::endl;    
   // ///*std::cout << (I && I && X) << std::endl;*/

   // /*std::vector<complex<double>> vec = { 1, 0, 0, 0 };
   // vec = (Ry && I) * vec;


   // vec = (X && I) * vec;

   // vec = CNOT01 * vec;

   // vec = (X && I) * vec;

   // for (auto i : vec)
   //     std::cout << i << std::endl;

   // matrix<complex<double>> RzPi_2({
   //     {complex<double>(sqrt(2)/2, -sqrt(2)/2), 0},
   //     {0, complex<double>(sqrt(2) / 2, sqrt(2)/ 2)}
   //     });*/

   ///* vector<complex<double>> vec{ 1, 0, 0, 0 };

   // vec = (ry && i) * vec;

   // vec = cnot01 * vec;

   // vec = (rzpi_2 && i) * vec;

   // for (auto i : vec)
   //     std::cout << i << std::endl;*/

   // /*vector<complex<double>> vec{ 0, 1, 2, 3, 4, 5, 6, 7 };

   // vec = (I && I && X) * vec;

   // for (auto i : vec)
   //     std::cout << i << std::endl;*/
   // matrix <double> A = I && I&& X;
   // matrix <double> B = I && X && I;
   // matrix <double> C = X && I && I;

   // vector<double> vec1{ 0, 1, 2, 3, 4, 5, 6, 7 };
   // vector<double> vec2{ 0, 1, 2, 3, 4, 5, 6, 7 };
   // vector<double> vec3{ 0, 1, 2, 3, 4, 5, 6, 7 };

   // vec1 = A * vec1;
   // 
   // for (auto i : vec1)
   //     std::cout << i << std::endl;

   // cout << endl << endl;
   // vec2 = B * vec2;

   // for (auto i : vec2)
   //     std::cout << i << std::endl;

   // cout << endl << endl;
   // vec3 = C * vec3;

   // for (auto i : vec3)
   //     std::cout << i << std::endl;
    
    return 0;
}

