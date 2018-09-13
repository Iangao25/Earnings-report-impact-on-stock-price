#pragma once
#ifndef overloadoperator_h
#define overloadoperator_h

#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>


using namespace std;
typedef vector<double> Vectord;
typedef vector<Vectord > Matrixd;
typedef map<string, Vectord> Map;


// overload operator +, -, / for Vectord and Matrixd
inline Vectord operator+(const Vectord& v1, const Vectord& v2) {
	if (v1.size() != v2.size()) {
		cout << "the dimensions of vectors are not correspond" << endl;
	}
	else {
		Vectord result;
		for (int i = 0; i < v1.size(); i++)
			result.push_back(v1[i] + v2[i]);

		return result;
	}
}
inline Vectord operator-(const Vectord& v1, const Vectord& v2) {
	if (v1.size() != v2.size()) {
		cout << "the dimensions of vectors are not correspond" << endl;
	}
	else {
		Vectord result;
		for (int i = 0; i < v1.size(); i++)
			result.push_back(v1[i] - v2[i]);

		return result;
	}
}
inline Vectord operator/(const Vectord& v, const double& a) {
	Vectord result;
	for (int i = 0; i < v.size(); i++)
		result.push_back(v[i] / a);

	return result;
}
inline Matrixd operator-(const Matrixd& X1, const Matrixd& X2) {
	if (X1.size() != X2.size()) {
		cout << "the dimensions of matrices are not correspond" << endl;
	}
	else {
		Matrixd result;
		for (int i = 0; i < X1.size(); i++)
			result.push_back(X1.at(i) - X2.at(i));

		return result;
	}
}
#endif // !overloadoperator_h
