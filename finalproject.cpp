// finalproject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <vector>
#include <string>
#include "stockclass.h"
#include "download.h"
#include "readdata.h"
#include "Menu.h"
#include "BootStrap.h"
#include <iostream>

using namespace std;

int main()
{
	map<string, string> releasedate; map<string, vector<double>> EPS; map<string, vector<string>> map3;
	map<string, vector<double>> stock_, spy_;
	data(releasedate, EPS, map3);
	yahoo(map3, stock_, spy_);

	stockclass allstock;
	allstock.setdata(stock_, spy_, releasedate, EPS);
	vector <vector<string> > category;
	category = allstock.categorize(0.05);
	stockclass beat, meet, miss;
	beat.setdata(allstock.Select<Map>(category[0], allstock.stock), 
		                 allstock.Select<Map>(category[0], allstock.SPY), 
		                 allstock.Select<map <string, string>>(category[0], allstock.releasedate), 
		                 allstock.Select<Map>(category[0], allstock.EPSEarning)  );
	meet.setdata(allstock.Select<Map>(category[1], allstock.stock), allstock.Select<Map>(category[1], allstock.SPY),
		allstock.Select<map <string, string>>(category[1], allstock.releasedate), allstock.Select<Map>(category[1], allstock.EPSEarning));
	miss.setdata(allstock.Select<Map>(category[2], allstock.stock), allstock.Select<Map>(category[2], allstock.SPY),
		allstock.Select<map <string, string>>(category[2], allstock.releasedate), allstock.Select<Map>(category[2], allstock.EPSEarning));

	vector<stockclass> allstockgroup;
	allstockgroup.push_back(beat);
	allstockgroup.push_back(meet);
	allstockgroup.push_back(miss);
	vector<Matrixd> ResultMatrix;
	ResultMatrix = BootStrap(allstockgroup, category);

	Matrixd CAAR_;
	for (int i = 0; i < 3; i++) {
		CAAR_.push_back(ResultMatrix[i][1]);
	}

	setoffMenu(beat, meet, miss, ResultMatrix);

	cout << endl;

	system("PAUSE");

    return 0;
}

