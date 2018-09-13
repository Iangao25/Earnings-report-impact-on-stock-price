#pragma once
#ifndef excelgraph_h
#define excelgraph_h

#include <new.h>
#include <iostream>
#include <string>
#include<vector>
#include <list>
#include <vector>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <map>

#include "ExcelDriver\excelmechanisms.hpp"
#include "ExcelDriver\VectorsAndMatrices\arraymechanisms.hpp"
#include "ExcelDriver\VectorsAndMatrices\vector.cpp"
#include "ExcelDriver\Geometry\range.cpp"
#include <iostream>


using namespace std;

class Convert {
private:
	Vector<double, long> yarr;//(60, 1);
	std::vector<double> Vec;// (60);
	Convert() {}
public:
	Convert(const vector<double> & Vec_) : yarr(60, 1) //(long NumberSteps?//(60, 1);??
	{
		Vec.resize(60);  //(60)??
		Vec = Vec_;
		for (long j = yarr.MinIndex(); j <= yarr.MaxIndex(); j++)
		{
			yarr[j] = Vec[j - yarr.MinIndex()];
		}
	}

	//testing:
	Vector<double, long> get_yarr()
	{
		return(yarr);
	}
};
void Plot(vector<vector<double>> CAAR3)
{
	double h = 1;
	Vector<double, long> xarr(60, 1);

	xarr[xarr.MinIndex()] = -29.0; //-30??
	for (long i = xarr.MinIndex() + 1; i <= xarr.MaxIndex(); i++)
	{
		xarr[i] = xarr[i - 1] + h;
	}
	Convert myConvert1(CAAR3[0]);
	Convert myConvert2(CAAR3[1]);
	Convert myConvert3(CAAR3[2]);
	list<Vector<double, long>> myList = { myConvert1.get_yarr(), myConvert2.get_yarr(),myConvert3.get_yarr() };
	list<string> text = { "beat", "meet", "miss" };
	printInExcel(xarr, text, myList, "CAAR");
	getchar();
}


#endif // !excelgraph_h
