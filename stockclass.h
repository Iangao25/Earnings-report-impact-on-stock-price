	#pragma once
#ifndef stockclass_h
#define stockclass_h
#include"overloadoperator.h"
#include <iostream>


class stockclass
{

public:
	map <string, Vectord > stock;
	map <string, Vectord > SPY;
	map <string, string> releasedate;
	map <string, Vectord > EPSEarning;

	void setdata(Map stock_, Map spy_, map <string, string> release, Map EPS) {
		stock = stock_;
		SPY = spy_;
		releasedate = release;
		EPSEarning = EPS;
	}
	void setdata(Map stock_, Map spy_) {
		stock = stock_;
		SPY = spy_;
	}

	//categorize into three groups  (1st row beat,  2nd meet,  3rd miss)
	vector<vector<string> > categorize(double rate) {
		vector<vector<string> > result;
		result.resize(3);
		for (Map::iterator iter = EPSEarning.begin(); iter != EPSEarning.end(); iter++) {
			if (iter->second[1] > (iter->second[0] * (1.0 + rate)))
				result[0].push_back(iter->first);
			else if (iter->second[1] < (iter->second[0] * (1.0 + rate)) && iter->second[1] > (iter->second[0] * (1.0 - rate)))
				result[1].push_back(iter->first);
			else
				result[2].push_back(iter->first);
		}
		return result;
	}


	//choose stock data according to stockname, using function template
	template <typename T>
	T Select(vector<string> stockname, T obj) {
		T selected;
		for (vector<string>::const_iterator iter = stockname.begin(); iter != stockname.end(); iter++) {
			selected[*iter] = obj[*iter];
		}
		return selected;
	}

	Vectord AAR() {
		Matrixd Rstock, RSPY;
		Rstock.resize(stock.size());
		RSPY.resize(SPY.size());
		Vectord result;
		int k = 0;
		for (Map::iterator iter = stock.begin(); iter != stock.end(); iter++) {
			for (int i = 0; i < 60; i++) {
				Rstock[k].push_back((iter->second[i + 1] - iter->second[i]) / iter->second[i]);
			}
			k++;
		}
		int k_ = 0;
		for (Map::iterator iter = SPY.begin(); iter != SPY.end(); iter++) {
			for (int i = 0; i < 60; i++) {
				RSPY[k_].push_back((iter->second[i + 1] - iter->second[i]) / iter->second[i]);
			}
			k_++;
		}
		Matrixd temp = Rstock - RSPY;
		int N = temp.size();
		for (int j = 0; j < temp[0].size(); j++) {
			double sum = 0;
			for (int i = 0; i < N; i++) {
				sum += temp[i][j];
			}
			sum /= N;
			result.push_back(sum);
		}

		return result;
	}


	Vectord CAAR(Vectord AAR) {
		Vectord result;
		result.push_back(AAR[0]);
		for (int i = 1; i < AAR.size(); i++) {
			result.push_back(result.back() + AAR[i]);
		}

		return result;
	}


	stockclass();
	~stockclass();
};

#endif // !stockclass_h


