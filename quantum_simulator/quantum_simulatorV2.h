#pragma once
#include <vector>
#include <complex>
#include <cmath>
#include <random>
#include <bitset>
#include <algorithm>
#include <omp.h>

using namespace std;

const double Pi = 3.14159265358979323846;
const double Pi_ = 3.14;

double makeError(double phi, double disp)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, disp);
    double random = d(gen);
    return (phi + random);
}


template<class T>
vector<T> operator&&(const vector<T>& a, const vector<T>& b)
{
    vector<T> result(a.size() * b.size());
    for (int i = 0; i < a.size(); ++i)
    {
        for (int j = 0; j < b.size(); ++j)
        {
            result[i * b.size() + j] = a[i] * b[j];
        }
    }
    return result;
}

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
const vector<complex<double>> X_{ 0, 1, 1, 0 };
const vector<complex<double>> Y_{ 0, complex<double>(0, -1), complex<double>(0, 1), 0 };
const vector<complex<double>> Z_{ 1, 0, 0, -1 };

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

    void PrintMean() {
        for (size_t i = 0; i < vector_state_.size(); ++i) {
            if (vector_state_[i] != complex<double>(0, 0)) {
                cout << vector_state_[i] << " " << bitset<50>(i).to_string().substr(50 - count_of_qubits) << endl;
            }
        }
        cout << "\n";
    }

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
    Q_Sim(int size) : count_of_qubits((int)log2(size))
    {
        vector_state_.push_back(1);
        for (int i = 1; i < size; ++i)
            vector_state_.push_back(0);
    }
    Q_Sim(const string& qubits) : count_of_qubits(qubits.length())
    {
        std::bitset<32> bitset(qubits);
        for (int i = 0; i < bitset.to_ulong(); ++i)
            vector_state_.push_back(complex<double>(0, 0));
        vector_state_.push_back(complex<double>(1, 0));
        for (int i = bitset.to_ulong() + 1; i < pow(2, count_of_qubits); ++i)
            vector_state_.push_back(complex<double>(0, 0));
    }

    int get_size()
    {
        return vector_state_.size();
    }
    int get_count_of_qubits()
    {
        return count_of_qubits;
    }
    vector<complex<double>> get_vector_state()
    {
        return vector_state_;
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
#pragma omp parallel for
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
#pragma omp parallel for
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

    vector<int> Measure(int n)
    {
        vector<int> result(vector_state_.size(), 0);
        vector<complex<double>> result1(vector_state_.size(), complex<double>(0, 0));
        for (int i = 0; i < n; ++i)
        {
            result[To_Measure()]++;
        }
        /*int max_index = 0;
        int max = -1;
        for (int i = 0; i < result.size(); ++i)
        {
            if (result[i] > max)
            {
                max_index = i;
                max = result[i];
            }
        }*/
        //result1[max_index] = complex<double>(1, 0);
        return result;
    }

    Q_Sim Measure(const vector<int>& id_qubits, int n = 1000000) {
        // Проверка входных данных
        if (id_qubits.empty()) {
            throw invalid_argument("Empty qubit list for measurement");
        }

        // 1. Вычисляем вероятности состояний для измеряемых кубитов
        const int num_measured_states = 1 << id_qubits.size();
        vector<double> probabilities(num_measured_states, 0.0);

        for (int i = 0; i < vector_state_.size(); ++i) {
            int pos = 0;
            for (size_t j = 0; j < id_qubits.size(); ++j) {
                if ((i >> id_qubits[j]) & 1) {
                    pos |= (1 << j);
                }
            }
            probabilities[pos] += norm(vector_state_[i]);
        }

        // 2. Моделируем измерения (n раз)
        vector<int> measurement_counts(num_measured_states, 0);
        random_device rd;
        mt19937 gen(rd());
        discrete_distribution<> dist(probabilities.begin(), probabilities.end());
        for (int i = 0; i < n; ++i) {
            measurement_counts[dist(gen)]++;
        }

        // 3. Находим наиболее вероятное состояние
        int max_index = distance(
            measurement_counts.begin(),
            max_element(measurement_counts.begin(), measurement_counts.end())
        );

        // 4. Создаем коллапсированное состояние
        vector<complex<double>> collapsed_state(vector_state_.size(), 0.0);
        complex<double> norm_factor(0.0);

        // Сначала вычисляем нормировочный коэффициент
        for (int i = 0; i < vector_state_.size(); ++i) {
            bool match = true;
            for (size_t j = 0; j < id_qubits.size(); ++j) {
                if (((i >> id_qubits[j]) & 1) != ((max_index >> j) & 1)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                norm_factor += norm(vector_state_[i]);
            }
        }

        norm_factor = sqrt(norm_factor);

        // Затем заполняем коллапсированное состояние
        for (int i = 0; i < vector_state_.size(); ++i) {
            bool match = true;
            for (size_t j = 0; j < id_qubits.size(); ++j) {
                if (((i >> id_qubits[j]) & 1) != ((max_index >> j) & 1)) {
                    match = false;
                    break;
                }
            }
            if (match && norm_factor != 0.0) {
                collapsed_state[i] = vector_state_[i] / norm_factor;
            }
        }

        return Q_Sim(collapsed_state);
    }

    complex<double>& operator[](int i)
    {
        return vector_state_[i];
    }


    //Q_Sim Measure(const vector<int>& id_qubits, int n = 1000)
    //{
    //    int num_measured_qubits = id_qubits.size();
    //    int num_possible_states = (int)pow(2, num_measured_qubits);

    //    vector<int> counts(num_possible_states, 0); // Подсчитываем результаты измерений

    //    // Выполняем n измерений
    //    for (int k = 0; k < n; ++k) {
    //        // Создаем копию состояния для каждого измерения (чтобы не изменять исходное)
    //        Q_Sim temp_sim = *this; // Копируем текущий объект Q_Sim

    //        // Выполняем одно измерение на копии
    //        int measured_state = temp_sim.MeasureOneShot(id_qubits); // Используем вспомогательную функцию

    //        if (measured_state != -1) {
    //            counts[measured_state]++; // Увеличиваем счетчик для этого результата
    //        }
    //    }

    //    // Находим наиболее часто измеренное состояние
    //    int most_frequent_state = 0;
    //    for (int i = 1; i < num_possible_states; ++i) {
    //        if (counts[i] > counts[most_frequent_state]) {
    //            most_frequent_state = i;
    //        }
    //    }

    //    // Создаем состояние, соответствующее наиболее часто измеренному результату
    //    vector<complex<double>> collapsed_state(vector_state_.size(), complex<double>(0, 0));
    //    for (int i = 0; i < vector_state_.size(); ++i) {
    //        bool flag = true;
    //        for (int j = 0; j < num_measured_qubits; ++j) {
    //            flag = flag && (((i >> id_qubits[j]) & 1) == ((most_frequent_state >> j) & 1));
    //        }
    //        if (flag) {
    //            collapsed_state[i] = vector_state_[i];
    //        }
    //    }

    //    //// Нормируем состояние
    //    //double collapse_norm = 0.0;
    //    //for (int i = 0; i < vector_state_.size(); ++i) {
    //    //    collapse_norm += norm(collapsed_state[i]);
    //    //}

    //    //if (collapse_norm > 0.0) {
    //    //    for (int i = 0; i < vector_state_.size(); ++i) {
    //    //        collapsed_state[i] /= sqrt(collapse_norm);
    //    //    }
    //    //}

    //    return Q_Sim(collapsed_state);
    //}

    //// Вспомогательная функция для выполнения одного измерения и возврата результата
    //int MeasureOneShot(const vector<int>& id_qubits) {
    //    int num_measured_qubits = id_qubits.size();
    //    int num_possible_states = (int)pow(2, num_measured_qubits);

    //    vector<complex<double>> prob(num_possible_states, complex<double>(0, 0));

    //    // 1. Нормировка
    //    double norm_factor = 0.0;
    //    for (const auto& amplitude : vector_state_) {
    //        norm_factor += norm(amplitude);
    //    }

    //    // 2. Вычисление вероятностей
    //    for (int i = 0; i < vector_state_.size(); ++i) {
    //        int pos = 0;
    //        for (int j = 0; j < num_measured_qubits; ++j) {
    //            if ((i >> id_qubits[j]) & 1)
    //                pos += pow(2, j);
    //        }
    //        prob[pos] += norm(vector_state_[i]) / norm_factor; // Вероятности
    //    }

    //    // 3. Моделирование измерения
    //    double r = Generation(0.0, 1.0);
    //    double summ = 0.0;
    //    int measured_state = -1;
    //    for (int i = 0; i < num_possible_states; ++i) {
    //        summ += norm(prob[i]);
    //        if (summ >= r) {
    //            measured_state = i;
    //            break;
    //        }
    //    }
    //    return measured_state;
    //}


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
    void QuantumAdder() // count_of_qubits = 3*n + 1 (n - first number, n - second number, n+1 - result)
    {
        if (count_of_qubits % 3 != 1)
            return;

        int length = count_of_qubits / 3;

        for (int i = 0; i < length; ++i)
            QuantumFullAdder(count_of_qubits - length + i, count_of_qubits - 2 * length + i, i, i + 1);
    }

    /* string get_system_state()
    {
        int i;
        for (i = 0; i < vector_state_.size(); ++i)
        {
            if (vector_state_[i] == complex<double>(1, 0))
                break;
        }
        bitset<32> bin = i;
        string result = bin.to_string<char, std::char_traits<char>, std::allocator<char>>();

        return result.substr(result.length() - count_of_qubits, count_of_qubits);

    }*/
    std::string get_system_state() const {
        // Находим индекс состояния с максимальной вероятностью
        int max_index = 0;
        double max_probability = 0.0;

        for (int i = 0; i < vector_state_.size(); ++i) {
            double probability = std::norm(vector_state_[i]);
            if (probability > max_probability) {
                max_probability = probability;
                max_index = i;
            }
        }

        // Конвертируем индекс в бинарную строку
        std::bitset<32> bin(max_index);
        std::string binary_str = bin.to_string();

        // Возвращаем только значимые биты (по количеству кубитов)
        return binary_str.substr(binary_str.size() - count_of_qubits);
    }


    void QFT() {
        int n = count_of_qubits;
        for (int i = 0; i < n; ++i)
        {
            H(i);
            for (int j = i + 1; j < n; ++j)
            {
                double phase = Pi / pow(2, j - i);
                CPhase(j, i, phase);
            }
        }
        for (int i = 0; i < n / 2; ++i)
        {
            SWAP(i, n - i - 1);
        }
    }

    void QFT_Alex(size_t start, size_t end)
    {
        for (size_t i = end - 1; i >= start && i + 1 != 0; i--)
        {
            H(i);
            for (size_t j = i - 1; j >= start && j + 1 != 0; j--)
            {
                CPhase(j, i, Pi / (1 << (i - j)));
            }
        }
    }


    void IQFT_Alex(size_t start, size_t end, double error = 0)
    {


        for (size_t i = start; i < end; i++)
        {
            for (size_t j = start; j < i; j++)
            {
                CPhase(j, i, -Pi / (1 << (i - j)));
            }
            H(i);
        }
    }


    void IQFT() {
        int n = count_of_qubits;
        for (int i = 0; i < n / 2; ++i) {
            SWAP(i, n - i - 1);
        }

        for (int i = n - 1; i >= 0; --i) {
            for (int j = n - 1; j > i; --j) {
                double phase = -2 * Pi / pow(2, j - i + 1);
                CPhase(j, i, phase);
            }
            H(i);
        }
    }

    void QFT_Range(int start, int n, double error = 0) {
        for (int i = n - 1; i >= start; --i) {
            H(i);
            for (int j = i - 1; j >= start; --j) {
                double phase = 2 * Pi / pow(2, i - j + 1);
                CPhase(j, i, phase, error);
            }
        }

    }

    void IQFT_Range(int start, int n, double error = 0) {

        for (int i = start; i < n; ++i) {
            for (int j = start; j < i; ++j) {
                double phase = -2 * Pi / pow(2, i - j + 1);
                CPhase(j, i, phase, error);
            }

            H(i);
        }
    }
    void ReverseQubitOrder(int start, int n) {
        for (int i = 0; i < n / 2; ++i) {
            SWAP(start + i, start + n - 1 - i);
        }
    }


    void QFT_Adder(int a_start, int b_start, int n, double error = 0)
    {
        ReverseQubitOrder(a_start, n);
        ReverseQubitOrder(b_start, n);
        QFT_Range(b_start, b_start + n, error);

        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                double phase = Pi / (1 << (j - i));

                CPhase(a_start + (n - 1 - j), b_start + (n - 1 - i), phase, error);
            }
        }

        IQFT_Range(b_start, b_start + n, error);

    }
    void QFT_Adder_with_correction(int a_start, int b_start, int n, double error = 0) // only for 4-qubit system (hardcode)
    {

        Q_Sim a = Get_system_for_correction(*this, 3);

        copy_qubit(a, 0, 4, 8);
        copy_qubit(a, 1, 5, 9);
        copy_qubit(a, 2, 6, 10);
        copy_qubit(a, 3, 7, 11);

        a.QFT_Adder(0, 2, 2, error);
        a.QFT_Adder(4, 6, 2, error);
        a.QFT_Adder(8, 10, 2, error);


        qubit_flip_correction(a, 0, 4, 8);
        qubit_flip_correction(a, 1, 5, 9);
        qubit_flip_correction(a, 2, 6, 10);
        qubit_flip_correction(a, 3, 7, 11);



        Q_Sim b = Get_system_for_first_n_qubits(a, 4);
        this->vector_state_ = b.get_vector_state();
    }

    void CPhase(int controlQubit, int targetQubit, double phase, double error = 0)
    {
        if (error != 0) {
            phase = makeError(phase, error);
        }
        int size = static_cast<int>(vector_state_.size());
#pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            if ((i >> controlQubit) & 1)
            {
                if ((i >> targetQubit) & 1)
                {
                    vector_state_[i] *= complex<double>(cos(phase), sin(phase));
                }
            }
        }
    }

    void SWAP(int qubit1, int qubit2) {
        int size = static_cast<int>(vector_state_.size());
        for (int i = 0; i < size; ++i) {
            if (((i >> qubit1) & 1) != ((i >> qubit2) & 1)) {
                int newIndex = i ^ (1 << qubit1) ^ (1 << qubit2);
                std::swap(vector_state_[i], vector_state_[newIndex]);
            }
        }
    }


    void norm_Q() {
        // Вычисляем сумму квадратов модулей амплитуд
        double norm_squared = 0.0;
        for (const auto& amplitude : vector_state_) {
            norm_squared += std::norm(amplitude); // norm = real^2 + imag^2
        }

        // Если норма нулевая (все амплитуды нулевые), ничего не делаем
        if (norm_squared <= 0.0) return;

        // Вычисляем коэффициент нормализации
        const double scale = 1.0 / std::sqrt(norm_squared);
        const double epsilon = 1e-10; // Порог для обнуления

        // Нормализуем амплитуды и очищаем шум
        for (auto& amplitude : vector_state_) {
            // Нормализуем
            amplitude *= scale;

            // Очищаем шум в вещественной и мнимой частях
            if (std::abs(amplitude.real()) < epsilon) {
                amplitude.real(0.0);
            }
            if (std::abs(amplitude.imag()) < epsilon) {
                amplitude.imag(0.0);
            }
        }
    }


    void QFT_Sub(int a_start, int b_start, int n, double error = 0)
    {
        ReverseQubitOrder(a_start, n);
        ReverseQubitOrder(b_start, n);
        QFT_Range(b_start, b_start + n, error);

        for (int i = 0; i < n; ++i) {
            for (int j = i; j < n; ++j) {
                double phase = -Pi / (1 << (j - i));

                CPhase(a_start + (n - 1 - j), b_start + (n - 1 - i), phase, error);
            }
        }

        IQFT_Range(b_start, b_start + n, error);

    }

    void copy_qubit(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
    {
        q.CNOT(qubit, ancilla1);
        q.CNOT(qubit, ancilla2);
    }
    void qubit_flip_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
    {
        q.CNOT(qubit, ancilla1);
        q.CNOT(qubit, ancilla2);

        q.Toffoli(ancilla2, ancilla1, qubit);
    }
    void copy_sign(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
    {
        q.CNOT(qubit, ancilla1);
        q.CNOT(qubit, ancilla2);
        q.H(qubit);
        q.H(ancilla1);
        q.H(ancilla2);
    }
    void sign_flip_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
    {
        q.H(qubit);
        q.H(ancilla1);
        q.H(ancilla2);

        q.CNOT(qubit, ancilla1);
        q.CNOT(qubit, ancilla2);

        q.Toffoli(ancilla2, ancilla1, qubit);
    }
    void copy_qubit_and_sign(Q_Sim& q, int qubit, int ancilla1, int ancilla2,
        int ancilla3, int ancilla4, int ancilla5, int ancilla6, int ancilla7, int ancilla8)
    {
        q.CNOT(qubit, ancilla3);
        q.CNOT(qubit, ancilla6);

        q.H(qubit);
        q.H(ancilla3);
        q.H(ancilla6);



        q.CNOT(qubit, ancilla1);
        q.CNOT(ancilla3, ancilla4);
        q.CNOT(ancilla6, ancilla7);

        q.CNOT(qubit, ancilla2);
        q.CNOT(ancilla3, ancilla5);
        q.CNOT(ancilla6, ancilla8);
    }
    void shor_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2,
        int ancilla3, int ancilla4, int ancilla5, int ancilla6, int ancilla7, int ancilla8)
    {
        q.CNOT(qubit, ancilla1);
        q.CNOT(ancilla3, ancilla4);
        q.CNOT(ancilla6, ancilla7);

        q.CNOT(qubit, ancilla2);
        q.CNOT(ancilla3, ancilla5);
        q.CNOT(ancilla6, ancilla8);


        q.Toffoli(ancilla2, ancilla1, qubit);
        q.Toffoli(ancilla5, ancilla4, ancilla3);
        q.Toffoli(ancilla8, ancilla7, ancilla6);



        q.H(qubit);
        q.H(ancilla3);
        q.H(ancilla6);


        q.CNOT(qubit, ancilla3);
        q.CNOT(qubit, ancilla6);


        q.Toffoli(ancilla6, ancilla3, qubit);
    }
    Q_Sim Get_system_for_correction(Q_Sim& q, int repetition)
    {



        vector<complex<double>> res_vector = q.get_vector_state();
        vector<complex<double>> zero_qubit = { 1, 0 };
        for (int i = 0; i < q.get_count_of_qubits() * (repetition - 1); ++i)
        {
            res_vector = zero_qubit && res_vector;
        }



        return Q_Sim(res_vector);
    }

    Q_Sim Get_system_for_first_n_qubits(Q_Sim q, int n)
    {

        vector<complex<double>> res((1 << n), 0.0);


        for (int i = 0; i < res.size(); ++i)
        {
            for (int j = i; j < q.get_size(); j += (1 << n))
            {
                res[i] += q[j];
            }
        }


        double norma = 0.0;
        for (const auto& amplitude : res) {
            norma += norm(amplitude);
        }
        norma = sqrt(norma);

        for (auto& amplitude : res) {
            amplitude /= norma;
        }
        return Q_Sim(res);



    }
};





// flip qubit

//void copy_qubit(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
//{
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(qubit, ancilla2);
//}


//void qubit_flip_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
//{
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(qubit, ancilla2);
//
//    q.Toffoli(ancilla2, ancilla1, qubit);
//}


 //sign flip

//void copy_sign(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
//{
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(qubit, ancilla2);
//    q.H(qubit);
//    q.H(ancilla1);
//    q.H(ancilla2);
//}

//void sign_flip_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
//{
//    q.H(qubit);
//    q.H(ancilla1);
//    q.H(ancilla2);
//
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(qubit, ancilla2);
//
//    q.Toffoli(ancilla2, ancilla1, qubit);
//}


//void copy_qubit_and_sign(Q_Sim& q, int qubit, int ancilla1, int ancilla2,
//    int ancilla3, int ancilla4, int ancilla5, int ancilla6, int ancilla7, int ancilla8)
//{
//    q.CNOT(qubit, ancilla3);
//    q.CNOT(qubit, ancilla6);
//
//    q.H(qubit);
//    q.H(ancilla3);
//    q.H(ancilla6);
//
//
//
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(ancilla3, ancilla4);
//    q.CNOT(ancilla6, ancilla7);
//
//    q.CNOT(qubit, ancilla2);
//    q.CNOT(ancilla3, ancilla5);
//    q.CNOT(ancilla6, ancilla8);
//}

//void shor_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2,
//    int ancilla3, int ancilla4, int ancilla5, int ancilla6, int ancilla7, int ancilla8)
//{
//    q.CNOT(qubit, ancilla1);
//    q.CNOT(ancilla3, ancilla4);
//    q.CNOT(ancilla6, ancilla7);
//
//    q.CNOT(qubit, ancilla2);
//    q.CNOT(ancilla3, ancilla5);
//    q.CNOT(ancilla6, ancilla8);
//
//
//    q.Toffoli(ancilla2, ancilla1, qubit);
//    q.Toffoli(ancilla5, ancilla4, ancilla3);
//    q.Toffoli(ancilla8, ancilla7, ancilla6);
//
//
//
//    q.H(qubit);
//    q.H(ancilla3);
//    q.H(ancilla6);
//
//
//    q.CNOT(qubit, ancilla3);
//    q.CNOT(qubit, ancilla6);
//
//
//    q.Toffoli(ancilla6, ancilla3, qubit);
//}


//Q_Sim Get_system_for_correction(Q_Sim& q, int repetition)
//{
//
//    
//
//    vector<complex<double>> res_vector = q.get_vector_state();
//    vector<complex<double>> zero_qubit = { 1, 0 };
//    for (int i = 0; i < q.get_count_of_qubits() * (repetition - 1); ++i)
//    {
//        res_vector =   zero_qubit && res_vector;
//    }
//
//    
//
//    return Q_Sim(res_vector);
//}
//
//Q_Sim Get_system_for_first_n_qubits(Q_Sim q, int n)
//{
//
//    vector<complex<double>> res((1 << n), 0.0);
//
//
//    for (int i = 0; i < res.size(); ++i)
//    {
//        for (int j = i; j < q.get_size(); j += (1 << n))
//        {
//            res[i] += q[j];
//        }
//    }
//
//
//    double norma = 0.0;
//    for (const auto& amplitude : res) {
//        norma += norm(amplitude);
//    }
//    norma = sqrt(norma);
//
//    for (auto& amplitude : res) {
//        amplitude /= norma;
//    }
//    return Q_Sim(res);
//
//
//
//}



static double calculateRMSE(const std::vector<std::complex<double>>& ideal,
    const std::vector<std::vector<std::complex<double>>>& noisyStates) {
    double totalError = 0.0;
    int p = noisyStates.size();
    int n = ideal.size();

    for (const auto& noisy : noisyStates) {
        double sumSq = 0.0;
        for (int i = 0; i < n; ++i) {
            double diffReal = ideal[i].real() - noisy[i].real();
            double diffImag = ideal[i].imag() - noisy[i].imag();
            sumSq += diffReal * diffReal + diffImag * diffImag;
        }
        totalError += sumSq;
    }

    return std::sqrt(totalError / p);
}