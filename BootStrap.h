#pragma once
#ifndef BootStrap_h
#define BootStrap_h
#include <map>
#include <vector>
#include <string>
#include "stockclass.h"
#include <iostream>

//BootStrap 100 times and output the result matrix, spending less than 300ms
vector <Matrixd > BootStrap(vector<stockclass> threeGroup, vector<vector<string> > stockgroupname) {
	vector <Matrixd > matrix_;
	matrix_.resize(3);

	for (int k = 0; k < 3; k++) {
		Matrixd aar;
		Matrixd caar;
		Vectord AARave(60, 0.0);
		Vectord CAARave(60, 0.0);

		stockclass selectedGroup;
		vector<vector <string> > result;
		result.resize(100);
		for (int j = 0; j < 100; j++) {
			int N = stockgroupname[k].size();
			vector <int> randvector;
			for (int i = 0; i < 40; i++) {
				result[j].push_back(stockgroupname[k][rand() % N]);
			}
			selectedGroup.setdata(threeGroup[k].Select<Map>(result[j], threeGroup[k].stock), 
				                                 threeGroup[k].Select<Map>(result[j], threeGroup[k].SPY));
			aar.push_back(selectedGroup.AAR());
			caar.push_back(selectedGroup.CAAR(aar.back()));
		}
		for (int i = 0; i < 100; i++) {
			AARave = AARave + aar[i];
			CAARave = CAARave + caar[i];
		}
		AARave = AARave / 100;
		CAARave = CAARave / 100;
		matrix_[k].push_back(AARave);
		matrix_[k].push_back(CAARave);
	}
	return matrix_;
}

#endif // !BootStrap_h
