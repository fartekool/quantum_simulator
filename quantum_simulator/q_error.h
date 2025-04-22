#pragma once
#include <random>




double makeError(double phi, double disp) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> d(0, disp);
	double random = d(gen);
	return (phi + random);
}


// flip qubit

void copy_qubit(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
{
	q.CNOT(qubit, ancilla1);
	q.CNOT(qubit, ancilla2);
}


void qubit_flip_correction(Q_Sim& q, int qubit, int ancilla1, int ancilla2)
{
	q.CNOT(0, 1);
	q.CNOT(0, 2);

	q.Toffoli(2, 1, 0);
}


// sign flip

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

	q.CNOT(0, 1);
	q.CNOT(0, 2);

	q.Toffoli(2, 1, 0);
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