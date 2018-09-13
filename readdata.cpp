#include "stdafx.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include<map>
#include <vector>
#include <string>
#include "readdata.h"
using namespace std;

void data(map<string, string> &map1, map<string, vector<double>>&map2, map<string, vector<string>> &map3)
{
	
	vector<double> EPS;
	vector<string> startend;
	EPS.resize(2);
	startend.resize(2);
	ifstream ifs("D:\\Documents\\Visual Studio 2017\\Projects\\Financial Computing\\Final\\newFinalproject\\EarningBook_.csv");


	string line;
	getline(ifs, line);
	while (getline(ifs, line))
	{
		
		istringstream ss_line(line);
		string item = "";
		getline(ss_line, item, ',');
		string symbol = item;
		getline(ss_line, item, ',');
		string report_date = item;
		
		getline(ss_line, item, ',');
		double actual = atof(item.c_str());
		getline(ss_line, item, ',');
		double estimate = atof(item.c_str());
		EPS[0]=estimate;EPS[1]=actual;
		getline(ss_line, item, ',');
		string start_date = item;
		getline(ss_line, item, ',');
		string end_date = item;
		startend[0] = start_date; startend[1] = end_date;
		map1.insert(pair<string, string>(symbol, report_date));
		map2.insert(pair<string, vector<double>>(symbol, EPS));
		map3.insert(pair<string, vector<string >>(symbol, startend));
	}
	


}