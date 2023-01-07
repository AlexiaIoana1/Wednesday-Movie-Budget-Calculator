#include "Person.hpp"
#include <iostream>

using namespace std;

vector<pair<size_t, size_t>> Person::UsedNames;

// For name generation
const char* FirstNames[] = { "Andrei", "Nicolae", "Stukov", "Camelia", "Sergiu", "Alexandra",
							"Cristina", "Connor", "Osas", "Jamal", "Anna", "Maria", "Jhonny" };

const char* LastNames[] = { "Popescu", "Doe", "Krastrov", "Windrunner", "Panait", "Sandu", "Basla",
						   "Ciobanu", "Smith", "Clark", "Campbell", "Sidorov", "Weber", "Meyer" };

Person::Person()
{
	MonsterType = 0;
	DietType = 0;
}

void Person::SetName(const string & Name)
{
	FullName = Name;
}
void Person::SetNameRandom()
{
	pair<size_t, size_t> NameID;
	NameID.first = rand() % (sizeof(FirstNames) / sizeof(char*));
	NameID.second = rand() % (sizeof(LastNames) / sizeof(char*));
	bool FoundName = false;
	size_t NameRerolls = 0;

	if (UsedNames.size() == (sizeof(FirstNames) / sizeof(char*)) * (sizeof(LastNames) / sizeof(char*)))
		throw(NotEnoughNames());

	while (!FoundName)
	{
		if (UsedNames.size() == 0)
			FoundName = true;
		else
		{
			bool bDuplicate = false;
			for (size_t i = 0; i < UsedNames.size(); i++)
				if (NameID.first == UsedNames[i].first && NameID.second == UsedNames[i].second)
				{
					bDuplicate = true;
					break;
				}

			if (!bDuplicate)
				FoundName = true;
		}
				
		if (FoundName)
			break;

		NameRerolls++;
		NameID.second++;
		if (NameID.second >= (sizeof(LastNames) / sizeof(char*)))
		{
			NameID.second = (NameID.second) % (sizeof(LastNames) / sizeof(char*));
			NameID.first = (NameID.first + 1) % (sizeof(FirstNames) / sizeof(char*));
		}
	}

	UsedNames.push_back(NameID);
	FullName = FirstNames[NameID.first];
	FullName.append(" ");
	FullName.append(LastNames[NameID.second]);
}
string Person::GetName()
{
	return FullName;
}

void Person::SetRole(const string& r)
{
	Role = r;
}
string Person::GetRole()
{
	return Role;
}

void Person::SetMonsterType(unsigned short type)
{
	MonsterType = type;
}
unsigned short Person::GetMonsterType()
{
	return MonsterType;
}

void Person::SetDietType(unsigned short type)
{
	DietType = type;
}
unsigned short Person::GetDietType()
{
	return DietType;
}