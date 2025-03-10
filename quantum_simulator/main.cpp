#include <iostream>
#include "quantum_simulatorV2.h"

using namespace std;




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


    /*string sum = Adder(Adder("10", "10"), Adder("1101", "1100"));

    cout << sum;*/

    /*vector<complex<double>> A{ 1, 0, 0 };
    vector<complex<double>> B{ 2, 3, 0 };

    vector<complex<double>> C = A && B;

    for (auto i : C)
        cout << i << ' ';*/
    Q_Sim q(vector<complex<double>>{0.8, 0.6});
    vector<complex<double>> A =  q.Measure(100);


    for (auto i : A)
        cout << i << endl;
    cout << endl;

    return 0;
}