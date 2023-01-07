#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "Source/Instance.hpp"

#define LOG_FILE "Logs.log"
#define IN_FILE "InputDatabase.csv"

#define CSV_CAST "OutputDatabase.csv"

#define CSV_COSTRO "CostsRO.csv"
#define CSV_COSTEN "CostsEN.csv"

#define CSV_MENUEN "FoodMenuEN.csv"
#define CSV_MENURO "FoodMenuRO.csv"


int main()
{
	vector<string> Args;
	Args.push_back(string(LOG_FILE));
	Args.push_back(string(IN_FILE));
	Args.push_back(string(CSV_CAST));
	Args.push_back(string(CSV_COSTEN));
	Args.push_back(string(CSV_COSTRO));
	Args.push_back(string(CSV_MENUEN));
	Args.push_back(string(CSV_MENURO));

	Instance Inst;
	int InstReturnValue = 0;
	
	try
	{
		InstReturnValue = Inst.Run(Args);
	}
	catch (Exception& ex)
	{
		ofstream Log(LOG_FILE, ios_base::app);
		Log << ex.What();
	}

	return InstReturnValue;
}

