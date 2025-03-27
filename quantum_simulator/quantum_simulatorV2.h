#pragma once
#include <vector>
#include <complex>
#include <cmath>
#include <random>




using namespace std;

double Generation(double a, double b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // Создание равномерного распределения в диапазоне [0, 1)
    std::uniform_real_distribution<double> dist(a, b);
    // Генерация случайной величины
    double r = dist(gen);
    return r;
}

const vector<complex<double>> I_{ 1, 0, 0, 1 };
const vector<complex<double>> H_{ sqrt(2) / 2, sqrt(2) / 2 ,sqrt(2) / 2, -sqrt(2) / 2 };
const vector<complex<double>> X_{ 0, 1, 1, 0};
const vector<complex<double>> Y_{ 0, complex<double>(0, -1), complex<double>(0, 1), 0};
const vector<complex<double>> Z_{ 1, 0, 0, -1};

class Q_Sim
{
    std::vector<complex<double>> vector_state_;
    int count_of_qubits;
    void U(const vector<complex<double>>& U_, int n)
    {
        for (int i = 0; i < vector_state_.size();)
        {
            auto tmp = vector_state_[i];

            vector_state_[i] = U_[0] * vector_state_[i] + U_[1] * vector_state_[i + (int)pow(2, n)];
            vector_state_[i + (int)pow(2, n)] = U_[2] * tmp + U_[3] * vector_state_[i + (int)pow(2, n)];
            if (((i + 1) % (int)pow(2, n)) == 0)
                i += ((int)pow(2, n) + 1);
            else
                ++i;
        }
    }

    vector<complex<double>> Ry_(double a)
    {
        return vector<complex<double>>{
            cos(a / 2), -sin(a / 2),
                sin(a / 2), cos(a / 2)
        };
    }
    vector<complex<double>> Rz_(double a)
    {
        return vector<complex<double>>{
            complex<double>(cos(-a / 2), sin(-a / 2)), 0,
                0, complex<double>(cos(a / 2), sin(a / 2))
        };
    }
public:

    Q_Sim() : count_of_qubits(1)
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
    Q_Sim(int size)
    {
        vector_state_.push_back(1);
        for (int i = 1; i < size; ++i)
            vector_state_.push_back(0);
    }
    void H(int n)
    {
        U(H_, n);
    }

    void X(int n)
    {
        U(X_, n);
    }

    void Y(int n)
    {
        U(Y_, n);
    }

    void Z(int n)
    {
        U(Z_, n);
    }
    void Ry(double a, int n)
    {
        U(Ry_(a), n);
    }
    void Rz(double a, int n)
    {
        U(Rz_(a), n);
    }



    void CNOT(int controlQubit, int targetQubit)
    {

        int size = static_cast<int>(vector_state_.size());
        vector<complex<double>> result(size);

        for (int i = 0; i < size; ++i) {
            // Проверяем, установлен ли управляющий кубит в |1⟩
            if ((i >> controlQubit) & 1) {
                int newIndex = i ^ (1 << targetQubit);// Инвертируем целевой кубит
                result[i] = vector_state_[newIndex];
            }
            else {
                result[i] = vector_state_[i];
            }
        }
        vector_state_ = result;
    }
    
    void CZ(int controlQubit, int targetQubit)
    {
        int size = static_cast<int>(vector_state_.size());
        vector<complex<double>> result(size);

        for (int i = 0; i < size; ++i) {
            // Проверяем, установлены ли управляющий и целевой кубиты в |1⟩
            if (((i >> controlQubit) & 1) && ((i >> targetQubit) & 1)) {
                // Применяем фазовый сдвиг (-1) к состоянию
                if ((vector_state_[i] != complex<double>(0, 0))) {
                    result[i] = vector_state_[i] * -1.0;
                }
            }
            else {
                result[i] = vector_state_[i];
            }
        }
        vector_state_ = result;
    }

    void Toffoli(int controlQubit1, int controlQubit2, int targetQubit)
    {

        int size = static_cast<int>(vector_state_.size());
        vector<complex<double>> result(size);

        for (int i = 0; i < size; ++i) {
            // Проверяем, установлен ли управляющий кубит в |1⟩
            if (((i >> controlQubit1) & 1) && ((i >> controlQubit2) & 1)) {
                int newIndex = i ^ (1 << targetQubit);// Инвертируем целевой кубит
                result[i] = vector_state_[newIndex];
            }
            else {
                result[i] = vector_state_[i];
            }
        }
        vector_state_ = result;
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

    int To_Measure() {
        vector<complex<double>> result(vector_state_.size(), complex<double>(0, 0));
        double summ = 0.0;
        double r = Generation(0.0, 1.0);
        int i;
        for (i = 0; i < vector_state_.size(); ++i) {
            summ += norm(vector_state_[i]);
            if (summ >= r) {
                result[i] = complex<double>(1, 0);
                break;
            }
        }
        return i;
        //return result;
    }

    vector<complex<double>> Measure(int n)
    {
        vector<int> result(vector_state_.size(), 0);
        vector<complex<double>> result1(vector_state_.size(), complex<double>(0, 0));
        for (int i = 0; i < n; ++i)
        {
            result[To_Measure()]++;
        }
        int max_index = 0;
        int max = -1;
        for (int i = 0; i < result.size(); ++i)
        {
            if (result[i] > max)
            {
                max_index = i;
                max = result[i];
            }
        }
        result1[max_index] = complex<double>(1, 0);
        return result1;
    }
    complex<double>& operator[](int i)
    {
        return vector_state_[i];
    }


    void QuantumFullAdder(int A, int B, int Cin, int Cout)
    {   
        Toffoli(A, B, Cout);
        CNOT(A, B);
        Toffoli(B, Cin, Cout);
        CNOT(B, Cin);
        CNOT(A, B);

        // A -> A
        // B -> B
        // Cin -> Sum
        // Cout -> Cout
    }
};
