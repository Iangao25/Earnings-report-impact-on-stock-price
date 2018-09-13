#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include "stockclass.h"
#include "excelgraph.h"

using namespace std;

string lowerToUpper(string stkname) {
	string res;
	for (int i = 0; i < stkname.size(); ++i) {
		if ('a' <= stkname[i] && stkname[i] <= 'z')
			res.push_back(stkname[i] - 32);
		else if ('A' <= stkname[i] && stkname[i] <= 'Z')
			res.push_back(stkname[i]);
		else {
			cout << "It's not a valid stock ticker."; break;
		}
	}
	return res;
}

// menu 2 function
void output(stockclass A, string stkname) {
		cout << "Announcement date:  " << A.releasedate[stkname] << endl;
		cout << "EPS Estimate:  " << A.EPSEarning[stkname][0] << "\tActual EPS:  " << A.EPSEarning[stkname][1] << endl;
		cout << "Historical prices:  " << endl;
		for (int i = 0; i < A.stock[stkname].size(); ++i) {
			cout << A.stock[stkname][i] << "\t";
			if (i % 9 == 8) cout << endl;
		}
}

//menu 4 function
void displayCAARgraph(vector<Matrixd> resultmatrix) {
	Matrixd CAAR_;
	for (int i = 0; i < 3; i++)
		CAAR_.push_back(resultmatrix[i][1]);
	Plot(CAAR_);
}
// menu function execution function
void command(int cmdnum_, stockclass beat, stockclass meet, stockclass miss, vector<Matrixd> ResultMatrix) {
	string stkname;
	int groupnum;
	switch (cmdnum_) {
	case 1:
		if (beat.stock.size() == beat.SPY.size() && beat.stock.size() > 40 &&
			meet.stock.size() == meet.SPY.size() && meet.stock.size() > 40 &&
			miss.stock.size() == miss.SPY.size() && miss.stock.size() > 40)
			cout << "Retrieving historical data succeeds." << endl;
		break;
	case 2:
		//cout << "Plese input the group number:\t";
		//cin >> groupnum;
		cout << "Plese input the stock ticker:\t";
		cin >> stkname;
		stkname = lowerToUpper(stkname);
		if (beat.stock.find(stkname) != beat.stock.end()) {
			cout << stkname << "is in group beat." << endl;
			output(beat, stkname);
		}
		else if (meet.stock.find(stkname) != meet.stock.end()) {
			cout << stkname << " is in group meet." << endl;
			output(meet, stkname);
		}
		else if (miss.stock.find(stkname) != miss.stock.end()) {
			cout << stkname << " is in group miss." << endl;
			output(miss, stkname);
		}
		else
			cout << stkname << " is not in any group." << endl;
		break;
	case 3:
		int choice;
		cout << "Plese input the group num (input 1 for beat, 2 for meet or 3 for miss):\t";
		cin >> groupnum;
		cout << "AAR (input 0) or CAAR (input 1)? \t";
		cin >> choice;
		for (int i = 0; i < 60; ++i) {
			cout << ResultMatrix[groupnum - 1][choice][i] << "\t";
			if (i % 10 == 9) cout << endl;
		}
		break;
	case 4:
		displayCAARgraph(ResultMatrix);
		break;
	case 5:
		exit(0);
	default:
		cout << "The input is not correct." << endl; break;
	}
}

void setoffMenu(stockclass beat, stockclass meet, stockclass miss, vector<Matrixd> ResultMatrix) {
	cout << "Menu: " << endl;
	cout << "1: Retrieve historical price data for stocks" << endl;
	cout << "2: Pull information of a stock from one group" << endl;	//including stk name, EPS estimate, actual EPS, announcement date, stk prices
	cout << "3: Show AAR or CAAR for one group" << endl;
	cout << "4: Show the Excel graph with CAAR for all 3 groups" << endl;
	cout << "5: Exit program" << endl << endl << endl;
	cout << "Input your choice:" << endl;
	int cmdnum;
	cin >> cmdnum;
	command(cmdnum, beat, meet, miss, ResultMatrix);
	cout << endl << "Any other choice?" << endl;
	setoffMenu(beat, meet, miss, ResultMatrix);
}