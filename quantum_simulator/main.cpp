#include <iostream>
#include "quantum_simulatorV2.h"

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


    //Q_Sim q(vector<complex<double>>{0.3, -0.6, -0.1, -0.7, 0, 0.1, -0.2, 0});

    Q_Sim q("111");


    q.QFT();
    cout << q;
    return 0;
}