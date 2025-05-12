#include <iostream>
#include "quantum_simulatorV2.h"
#include "q_error.h"
#include <thread>
#include <string>
#include <fstream>
using namespace std;


template<class T>
vector<T> operator&&(const vector<T>& a, const vector<T>& b)
{
    vector<T> result(a.size() * b.size());
    for (int i = 0; i < a.size(); ++i)
    {
        for (int j = 0; j < b.size(); ++j)
        {
            result[i*b.size() + j] = a[i] * b[j];
        }
    }
    return result;
}



void measurement_res_to_file(Q_Sim& q, string file_name, int n)
{   

    vector<int> res = q.Measure(n);

    std::ofstream out;
    out.open(file_name);
    if (out.is_open())
    {   
        for (auto& i : res)
            out << i << std::endl;
    }
    out.close();
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


    //Q_Sim q("000000001"); // 000000111
    //
    //copy_qubit_and_sign(q, 0, 1, 2, 3, 4, 5, 6, 7, 8);

    ////ошибка
    //q.X(0);
    ////
    //shor_correction(q, 0, 1, 2, 3, 4, 5, 6, 7, 8);
    //cout << q;
    ////cout << q.get_system_state();


    /*Q_Sim q("000");

    q.X(0);

    q.QFT_Alex(0, 2);


    cout << q << endl;*/


    //Q_Sim a("011001");


    //a.QFT_Adder(0, 3, 3);
    ////a.SWAP(0, 1);
    //a.update_vector();
    //cout << a;
    
    //cout << makeError(1, 0.1);

    Q_Sim q(vector<complex<double>>{0.8, 0.6});
    //Q_Sim q("001");
    
    /*vector<int> res = q.Measure(10000);

    for (auto& i : res)
        cout << i << " ";*/


    measurement_res_to_file(q, "results.txt", 10000);




    /*Q_Sim a("000");

    a.X(0);

    a.QFT_Range(0, 3);*/

 
    return 0;
}