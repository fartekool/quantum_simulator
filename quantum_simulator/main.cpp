#include <iostream>
#include "quantum_simulatorV2.h"
#include <thread>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;


//template<class T>
//vector<T> operator&&(const vector<T>& a, const vector<T>& b)
//{
//    vector<T> result(a.size() * b.size());
//    for (int i = 0; i < a.size(); ++i)
//    {
//        for (int j = 0; j < b.size(); ++j)
//        {
//            result[i*b.size() + j] = a[i] * b[j];
//        }
//    }
//    return result;
//}



vector<vector<complex<double>>> Create_vector_noise(Q_Sim& ideal, double disp,int count,bool correction){
    vector<vector<complex<double>>> vector_noises;
    for(int i=0;i<count;++i){
        Q_Sim noise_vector(ideal.get_vector_state());
        if(!correction)
            noise_vector.QFT_Adder(0, 2, 2,disp);
        else
            noise_vector.QFT_Adder_with_correction(0, 2, 2, disp);
        vector_noises.push_back(noise_vector.get_vector_state());
    }
    return vector_noises;
}




void RMSE_res_to_file(Q_Sim& ideal, string file_name, double max_disp, double step, int count, bool correction)
{   
    Q_Sim ideal_sum(ideal.get_vector_state());
    ideal_sum.QFT_Adder(0, 2, 2);

    vector<pair<double,double>> x_y;
    for(double disp=0;disp <= max_disp;disp += step){
        double RMSE_calculate =0;
        x_y.push_back({disp,calculateRMSE(ideal_sum.get_vector_state(), Create_vector_noise(ideal, disp, count, correction))});
        cout << disp << endl;
    }

    std::ofstream out;
    out.open(file_name);
    if (out.is_open())
    {
        for (auto& coord : x_y)
            out << coord.first <<  "        " << coord.second << std::endl;
    }

    out.close();
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


    // 1. Формируем команду для запуска Python скрипта
    std::string command = "py -3 histogram.py";

    // 2. Запускаем Python скрипт
    system(command.c_str());

}

void RMSE_graph(Q_Sim& q, string file_name1, string file_name2, double max_disp, double step, int count)
{
    RMSE_res_to_file(q, file_name1, max_disp, step, count, 0);
    RMSE_res_to_file(q, file_name2, max_disp, step, count, 1);

    std::string command = "py -3 RMSE.py";

    system(command.c_str());
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


    /*Q_Sim q("0101"); 
    */
    //copy_qubit(q, 0, 1, 2);

    //ошибка
    //q.X(0);
    //
    //qubit_flip_correction(q, 0, 1, 2);
    /*q.QFT_Adder(0, 2, 2, 0.1);
    cout << q;*/
    //cout << q.get_system_state();


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

    //Q_Sim q(vector<complex<double>>{0.5, 0.5, 0.5, 0.5});
    //q.H(0);
    //q.Ry(1, Pi / 5); // makeError(Pi / 5, 0.01)
    /*cout << q;*/
    //Q_Sim q("001");
    
    /*vector<int> res = q.Measure(10000);

    for (auto& i : res)
        cout << i << " ";*/


    //measurement_res_to_file(q, "results.txt", 10000);

    /*Q_Sim q(vector<complex<double>>{0.8, 0.6});
    Q_Sim a = Get_system_for_correction(q, 3); 
    
    Q_Sim b = Get_system_for_first_n_qubits(a, 1);

    cout << b;*/
















    //Q_Sim q(vector<complex<double>>{0.8, 0.3, 0.4, 0.33166});
    ////Q_Sim q("11");

    //
    //Q_Sim a = Get_system_for_correction(q, 9);
    //////cout << a;
    //copy_qubit_and_sign(a, 0, 2, 3, 4, 5, 6, 7, 8, 9);
    //copy_qubit_and_sign(a, 1, 10, 11, 12, 13, 14, 15, 16, 17);
    ////// ошибка
    ////
    //a.Z(0);
    //a.X(0);
    //a.Z(1);
    //a.X(1);
    ////
    //shor_correction(a, 0, 2, 3, 4, 5, 6, 7, 8, 9);
    //shor_correction(a, 1, 10, 11, 12, 13, 14, 15, 16, 17);
    //Q_Sim res = Get_system_for_first_n_qubits(a, 2);
    //measurement_res_to_file(res, "results.txt", 20000);
    //cout << res;


    Q_Sim q("0101");


    
    // БЕЗ КОРРЕКЦИИ ОШИБКИ
    //q.QFT_Adder(0, 2, 2, 0.5);




    


    // С коррекцией ошибки

    /*q.QFT_Adder_with_correction(0, 2, 2, 0.1);


    measurement_res_to_file(q, "results.txt", 20000);*/
    RMSE_graph(q, "coord.txt", "coord_correction.txt", 10, 0.5, 1000);

    /*vector<vector<complex<double>>> fjiaooh;
    fjiaooh.push_back(q.get_vector_state());

    cout << calculateRMSE(q.get_vector_state(), fjiaooh);*/
    return 0;
}