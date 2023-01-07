#pragma once
#include <string>
#include <vector>
#include "Exceptions.hpp"

using namespace std;

class Person
{
	private:
		string FullName;
		string Role;

		unsigned short MonsterType;
		unsigned short DietType;

		static vector<pair<size_t, size_t>> UsedNames;
	public:
		Person();

		void SetName(const string&);
		void SetNameRandom();
		string GetName();

		void SetRole(const string&);
		string GetRole();

		void SetMonsterType(unsigned short);
		unsigned short GetMonsterType();

		void SetDietType(unsigned short);
		unsigned short GetDietType();
};

