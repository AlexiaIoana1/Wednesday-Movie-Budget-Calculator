#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "Constants.hpp"

#include "Person.hpp"
#include "CSV.hpp"
#include "Currency.hpp"

using namespace std;

class Instance
{
	private:

		/** File Paths
		*  Expected 7 files.
		*	0. Log File
		*	1. Input File
		*	2. Output for all cast members.
		*	3. Output for costs EN.
		*	4. Output for costs RO.
		*	5. Output for menu EN.
		*	6. Output for menu RO.
		*/

		vector<string> Args;

		/* Full cast */
		vector<Person> Persons;

		/* Costs */
		Currency BusCost;

		Currency DailyMakeupCost;
		Currency DailyFoodCost;
		Currency DailyDrinksCost;

		Currency DailyHotel2Cost;
		Currency DailyHotel3Cost;

		Currency Rent30Days;
		Currency Rent45Days;
		Currency Rent60Days;
		Currency Rent100Days;

		Currency Total30Days;
		Currency Total45Days;
		Currency Total60Days;
		Currency Total100Days;

		size_t SoupID[3];
		size_t MainDishID[3];
		size_t MainDishIDVegan[3];
		size_t MainDishIDFish[3];
		size_t DesertID[3];

	private:
		/* Helper Function */
		size_t ReadEntries();
		void GenerateExtraEntries();
		void GenerateEntryData();

		size_t PrintEntries();
		void CalculateCosts();
		size_t PrintCostsEN();
		size_t PrintCostsRO();
		void RollDishes();
		size_t PrintMenuEN();
		size_t PrintMenuRO();

	public:
		int Run(vector<string>& Args);
};