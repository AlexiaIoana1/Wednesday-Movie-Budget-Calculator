#include "Instance.hpp"

using namespace std;

const char* SoupNamesEN[] = {"Tomato soup", "Vegetable cream soup", "Potato soup", "Carrot soup"};
const char* SoupNamesRO[] = {"Supa de Rosii", "Supa-crema de legume", "Supa de cartofi", "Supa de morcovi"};

const char* MainDishNamesVeganEN[] = {"Salad", "Vegan Pasta", "Bruschetta", "Teriyaki crispy tofu"};
const char* MainDishNamesVeganRO[] = {"Salata", "Paste vegane", "Bruschete", "Teriyaki-tofu prajit"};

const char* MainDishNamesFishEN[] = {"Smoked salmon with potatoes", "Smoked trout", "Carp", "Grilled pike with vegetables"};
const char* MainDishNamesFishRO[] = {"Somon afumat cu cartofi", "Pastrav afumat", "Crap", "Stiuca la gratar cu legume"};

const char* MainDishNamesEN[] = {"Schnitzel", "Beef steak", "Burger", "Pizza"};
const char* MainDishNamesRO[] = {"Snitel","Friptura de vita", "Burger", "Pizza"};

const char* DesertNamesEN[] = {"Tiramisu", "Chocolate Cake", "Pancakes", "Cookies"};
const char* DesertNamesRO[] = {"Tiramisu", "Tort de ciocolata", "Clatite", "Fursecuri"};

//Log record

int Instance::Run(vector<string>& Args)
{
	if (Args.size() != 7)
		throw(UnknownError());

    //File doesn't open

	ofstream Log(Args[0]);
	if (!Log.good())
		throw(FileError(Args[0].c_str()));

	size_t Aux;
	this->Args = Args;

	Log << "Reading entries from: " << Args[1] << "\n";
	Aux = ReadEntries();
	Log << "Successfully read " << Aux << " entries.\n\n";

	Log << "Generating " << EXTRA_PERSON_COUNT << " extra enties." << "\n";
	GenerateExtraEntries();
	Log << "Successfully generated " << EXTRA_PERSON_COUNT << " entries.\n\n";

	Log << "Writing " << Persons.size() << " entries to: " << Args[2] << "\n";
	Aux = PrintEntries();
	Log << "Entries written: " << Aux << "\n\n";

	CalculateCosts();

	Log << "Writing costs to " << Args[3] << "\n";
	Aux = PrintCostsEN();
	Log << "Wrote: " << Aux << " entries.\n";

	Log << "Writing costs to " << Args[4] << "\n";
	Aux = PrintCostsRO();
	Log << "Wrote: " << Aux << " entries.\n";

	RollDishes();

	Log << "Writing menus to " << Args[5] << "\n";
	Aux = PrintMenuEN();
	Log << "Wrote: " << Aux << " entries.\n";

	Log << "Writing menus to " << Args[6] << "\n";
	Aux = PrintMenuRO();
	Log << "Wrote: " << Aux << " entries.\n";

	return 0;
}

//Read Entries from initial CSV

size_t Instance::ReadEntries()
{
	ifstream In(Args[1]);
	if (!In.good())
		throw(FileError(Args[1].c_str()));

	csv::Reader Reader;
	vector<string> Entry;

	Reader.ReadBegin(&In);
	Person p;
	while (Reader.ReadEntry(Entry))
	{
		p.SetName(Entry[0]);
		p.SetRole(Entry[1]);

		Persons.push_back(p);
	}
	return Reader.ReadEnd();
}

//Generate 132 new people

void Instance::GenerateExtraEntries()
{
	Person p;
	p.SetRole(string("Figurant"));

	for (size_t i = 0; i < EXTRA_PERSON_COUNT; i++)
	{
		p.SetNameRandom(); 
		Persons.push_back(p);
	}
}

// Generate diet and monster-type random

void Instance::GenerateEntryData()
{
	for (size_t i = 0; i < Persons.size(); i++)
	{
		Persons[i].SetMonsterType(rand() % 5);
		Persons[i].SetDietType(rand() % 3);
	}
}


//Write in file - new extra people

size_t Instance::PrintEntries()
{
	ofstream OutFile(Args[2]);
	if (!OutFile.good())
		throw(FileError(Args[2].c_str()));

	csv::Writer Writer;
	vector<string> Entry;

	Writer.WriteBegin(&OutFile);
	for (size_t i = 0; i < Persons.size(); i++)
	{
		Entry.clear();
		Entry.push_back(Persons[i].GetName());
		Entry.push_back(Persons[i].GetRole());
		Writer.WriteEntry(Entry);

		if (i != Persons.size() - 1)
			Writer.WriteEntrySeparator();
	}
	return Writer.WriteEnd();
}

//Calculate the cost:

void Instance::CalculateCosts()
{
	// Bus
	BusCost.Value = ((unsigned long)Persons.size() / BUS_CAPACITY + 1) * BUS_COST;
	BusCost.Convert(RATIO_RON_USD);

	// Makeup
	DailyMakeupCost.Value = 0;
	for (size_t i = 0; i < Persons.size(); i++)
	{
		switch (Persons[i].GetMonsterType())
		{
		case MONSTER_VAMPIRE:	DailyMakeupCost.Value += MONSTER_VAMPIRE_COST;	break;
		case MONSTER_WEREWOLF:	DailyMakeupCost.Value += MONSTER_WEREWOLF_COST;	break;
		case MONSTER_MERMAID:	DailyMakeupCost.Value += MONSTER_MERMAID_COST;	break;
		case MONSTER_SEER:		DailyMakeupCost.Value += MONSTER_SEER_COST;		break;
		case MONSTER_HUMAN:		DailyMakeupCost.Value += MONSTER_HUMAN_COST;	break;
		}
	}
	DailyMakeupCost.Convert(RATIO_RON_USD);

	// Food
	DailyFoodCost.Value = 0;
	for (size_t i = 0; i < Persons.size(); i++)
	{
		switch (Persons[i].GetDietType())
		{
		case DIET_EVERYTHING:	DailyFoodCost.Value += DIET_EVERYTHING_COST;	break;
		case DIET_FISH:			DailyFoodCost.Value += DIET_FISH_COST;			break;
		case DIET_VEGAN:		DailyFoodCost.Value += DIET_VEGAN_COST;			break;
		}
	}
	DailyFoodCost.Convert(RATIO_RON_USD);

	// Drinks
	unsigned long long TotalWaterMl = WATER_CONSUMPTION * Persons.size();
	unsigned long long TotalCoffeeMl = COFFEE_CONSUMPTION * Persons.size();
	unsigned long long TotalJuiceMl = JUICE_CONSUMPTION * Persons.size();

	unsigned long long TotalWaterUnits = TotalWaterMl / WATER_UNIT + (TotalWaterMl % WATER_UNIT > 0);
	unsigned long long TotalCoffeeUnits = TotalCoffeeMl / COFFEE_UNIT + (TotalCoffeeMl % COFFEE_UNIT > 0);
	unsigned long long TotalJuiceUnits = TotalJuiceMl / JUICE_UNIT + (TotalJuiceMl % JUICE_UNIT > 0);

	DailyDrinksCost.Value = TotalWaterUnits * WATER_COST + TotalCoffeeUnits * COFFEE_COST + TotalJuiceUnits * JUICE_COST;
	DailyDrinksCost.Convert(RATIO_RON_USD);

	// Hotel
	DailyHotel2Cost.Value = (Persons.size() - EXTRA_PERSON_COUNT) * HOTEL_ROOM_2_COST;
	DailyHotel2Cost.Convert(RATIO_RON_USD);

	DailyHotel3Cost.Value = EXTRA_PERSON_COUNT * HOTEL_ROOM_3_COST;
	DailyHotel3Cost.Convert(RATIO_RON_USD);

	// Castle Rent
	Rent30Days.Value = CASTLE_RENT_COST * 30 * (100 - (30 / 10) * CASTLE_REST_DISCOUNT) / 100;
	Rent30Days.Convert(RATIO_RON_USD);

	Rent45Days.Value = CASTLE_RENT_COST * 45 * (100 - (45 / 10) * CASTLE_REST_DISCOUNT) / 100;
	Rent45Days.Convert(RATIO_RON_USD);

	Rent60Days.Value = CASTLE_RENT_COST * 60 * (100 - (60 / 10) * CASTLE_REST_DISCOUNT) / 100;
	Rent60Days.Convert(RATIO_RON_USD);

	Rent100Days.Value = CASTLE_RENT_COST * 100 * (100 - (100 / 10) * CASTLE_REST_DISCOUNT) / 100;
	Rent100Days.Convert(RATIO_RON_USD);

	// Total
	Total30Days.Value = Rent30Days.Value + BusCost.Value + (DailyHotel2Cost.Value + DailyDrinksCost.Value + DailyFoodCost.Value + DailyMakeupCost.Value) * 30;
	Total45Days.Value = Rent45Days.Value + BusCost.Value + (DailyHotel2Cost.Value + DailyDrinksCost.Value + DailyFoodCost.Value + DailyMakeupCost.Value) * 45;
	Total60Days.Value = Rent60Days.Value + BusCost.Value + (DailyHotel2Cost.Value + DailyDrinksCost.Value + DailyFoodCost.Value + DailyMakeupCost.Value) * 60;
	Total100Days.Value = Rent100Days.Value + BusCost.Value + (DailyHotel2Cost.Value + DailyDrinksCost.Value + DailyFoodCost.Value + DailyMakeupCost.Value) * 100;
}

//Print costs in EN

size_t Instance::PrintCostsEN()
{
	ofstream OutFileEN(Args[3]);
	if (!OutFileEN.good())
		throw(FileError(Args[3].c_str()));

	csv::Writer WriterEN;
	vector<string> Entry;
	Currency Aux;

	WriterEN.WriteBegin(&OutFileEN);
	Entry.push_back(string("Number of Days"));
	Entry.push_back(string("Transport"));
	Entry.push_back(string("Accommodation"));
	Entry.push_back(string("Makeup"));
	Entry.push_back(string("Food & Drinks"));
	Entry.push_back(string("Castle Rent"));
	Entry.push_back(string("Total"));
	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();
	Entry.clear();

    //For 30 days

	Entry.push_back(string("30"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent30Days.Format(',', '.', " $"));
	Entry.push_back(Total30Days.Format(',', '.', " $"));

	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();
	Entry.clear();

    //For 45 days

	Entry.push_back(string("45"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent45Days.Format(',', '.', " $"));
	Entry.push_back(Total45Days.Format(',', '.', " $"));

	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();
	Entry.clear();

    //For 60 days

	Entry.push_back(string("60"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent60Days.Format(',', '.', " $"));
	Entry.push_back(Total60Days.Format(',', '.', " $"));

	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();
	Entry.clear();

    //For 100 days

	Entry.push_back(string("100"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent100Days.Format(',', '.', " $"));
	Entry.push_back(Total100Days.Format(',', '.', " $"));

	WriterEN.WriteEntry(Entry);

	return WriterEN.WriteEnd();
}

//Print costs in RO

size_t Instance::PrintCostsRO()
{
	ofstream OutFileRO(Args[4]);
	if (!OutFileRO.good())
		throw(FileError(Args[4].c_str()));

	csv::Writer WriterRO;
	vector<string> Entry;
	Currency Aux;

	WriterRO.WriteBegin(&OutFileRO);
	Entry.push_back(string("Numar de zile"));
	Entry.push_back(string("Transport"));
	Entry.push_back(string("Cazare"));
	Entry.push_back(string("Machiaj"));
	Entry.push_back(string("Mancare si Apa"));
	Entry.push_back(string("Inchiriere spatiu"));
	Entry.push_back(string("Total"));
	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();
	Entry.clear();

    //For 30 days

	Entry.push_back(string("30"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 30;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent30Days.Format(',', '.', " $"));
	Entry.push_back(Total30Days.Format(',', '.', " $"));

	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();
	Entry.clear();

    //For 45 days

	Entry.push_back(string("45"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 45;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent45Days.Format(',', '.', " $"));
	Entry.push_back(Total45Days.Format(',', '.', " $"));

	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();
	Entry.clear();

    //For 60 days

	Entry.push_back(string("60"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 60;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent60Days.Format(',', '.', " $"));
	Entry.push_back(Total60Days.Format(',', '.', " $"));

	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();
	Entry.clear();

    //For 100 days

	Entry.push_back(string("100"));
	Entry.push_back(BusCost.Format(',', '.', " $"));

	Aux.Value = (DailyHotel2Cost.Value + DailyHotel3Cost.Value) * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = DailyMakeupCost.Value * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Aux.Value = (DailyFoodCost.Value + DailyDrinksCost.Value) * 100;
	Entry.push_back(Aux.Format(',', '.', " $"));

	Entry.push_back(Rent100Days.Format(',', '.', " $"));
	Entry.push_back(Total100Days.Format(',', '.', " $"));

	WriterRO.WriteEntry(Entry);

	return WriterRO.WriteEnd();
}

//Menu creation for 3 days

void Instance::RollDishes()
{
	SoupID[0] = rand() % (sizeof(SoupNamesEN) / sizeof(char*));
	SoupID[1] = (SoupID[0] + 1) % (sizeof(SoupNamesEN) / sizeof(char*));
	SoupID[2] = (SoupID[0] + 2) % (sizeof(SoupNamesEN) / sizeof(char*));

	MainDishID[0] = rand() % (sizeof(MainDishNamesEN) / sizeof(char*));
	MainDishID[1] = (MainDishID[0] + 1) % (sizeof(MainDishNamesEN) / sizeof(char*));
	MainDishID[2] = (MainDishID[0] + 2) % (sizeof(MainDishNamesEN) / sizeof(char*));

	MainDishIDVegan[0] = rand() % (sizeof(MainDishNamesVeganEN) / sizeof(char*));
	MainDishIDVegan[1] = (MainDishIDVegan[0] + 1) % (sizeof(MainDishNamesVeganEN) / sizeof(char*));
	MainDishIDVegan[2] = (MainDishIDVegan[0] + 2) % (sizeof(MainDishNamesVeganEN) / sizeof(char*));

	MainDishIDFish[0] = rand() % (sizeof(MainDishNamesFishEN) / sizeof(char*));
	MainDishIDFish[1] = (MainDishIDFish[0] + 1) % (sizeof(MainDishNamesFishEN) / sizeof(char*));
	MainDishIDFish[2] = (MainDishIDFish[0] + 2) % (sizeof(MainDishNamesFishEN) / sizeof(char*)); 

	DesertID[0] = rand() % (sizeof(DesertNamesEN) / sizeof(char*));
	DesertID[1] = (DesertID[0] + 1) % (sizeof(DesertNamesEN) / sizeof(char*));
	DesertID[2] = (DesertID[0] + 2) % (sizeof(DesertNamesEN) / sizeof(char*));
}

//Print Menu in EN

size_t Instance::PrintMenuEN()
{
	ofstream OutFileEN(Args[5]);
	if (!OutFileEN.good())
		throw(FileError(Args[5].c_str()));

	csv::Writer WriterEN;
	WriterEN.WriteBegin(&OutFileEN);
	
	vector<string> Entry;

	Entry.push_back(string("Soup"));
	Entry.push_back(string("Main Dish"));
	Entry.push_back(string("Main Dish Vegan"));
	Entry.push_back(string("Main Dish Fish"));
	Entry.push_back(string("Dessert"));
	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesEN[SoupID[0]]));
	Entry.push_back(string(MainDishNamesEN[MainDishID[0]]));
	Entry.push_back(string(MainDishNamesVeganEN[MainDishIDVegan[0]]));
	Entry.push_back(string(MainDishNamesFishEN[MainDishIDFish[0]]));
	Entry.push_back(string(DesertNamesEN[DesertID[0]]));
	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesEN[SoupID[1]]));
	Entry.push_back(string(MainDishNamesEN[MainDishID[1]]));
	Entry.push_back(string(MainDishNamesVeganEN[MainDishIDVegan[1]]));
	Entry.push_back(string(MainDishNamesFishEN[MainDishIDFish[1]]));
	Entry.push_back(string(DesertNamesEN[DesertID[1]]));
	WriterEN.WriteEntry(Entry);
	WriterEN.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesEN[SoupID[2]]));
	Entry.push_back(string(MainDishNamesEN[MainDishID[2]]));
	Entry.push_back(string(MainDishNamesVeganEN[MainDishIDVegan[2]]));
	Entry.push_back(string(MainDishNamesFishEN[MainDishIDFish[2]]));
	Entry.push_back(string(DesertNamesEN[DesertID[2]]));
	WriterEN.WriteEntry(Entry);

	return WriterEN.WriteEnd();
}

//Print Menu in RO

size_t Instance::PrintMenuRO()
{
	ofstream OutFileRO(Args[6]);
	if (!OutFileRO.good())
		throw(FileError(Args[6].c_str()));

	csv::Writer WriterRO;
	WriterRO.WriteBegin(&OutFileRO);

	vector<string> Entry;

	Entry.push_back(string("Supa"));
	Entry.push_back(string("Fel principal"));
	Entry.push_back(string("Fel principal vegetarian"));
	Entry.push_back(string("Fel principal flexitarian"));
	Entry.push_back(string("Desert"));
	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesRO[SoupID[0]]));
	Entry.push_back(string(MainDishNamesRO[MainDishID[0]]));
	Entry.push_back(string(MainDishNamesVeganRO[MainDishIDVegan[0]]));
	Entry.push_back(string(MainDishNamesFishRO[MainDishIDFish[0]]));
	Entry.push_back(string(DesertNamesRO[DesertID[0]]));
	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesRO[SoupID[1]]));
	Entry.push_back(string(MainDishNamesRO[MainDishID[1]]));
	Entry.push_back(string(MainDishNamesVeganRO[MainDishIDVegan[1]]));
	Entry.push_back(string(MainDishNamesFishRO[MainDishIDFish[1]]));
	Entry.push_back(string(DesertNamesRO[DesertID[1]]));
	WriterRO.WriteEntry(Entry);
	WriterRO.WriteEntrySeparator();

	Entry.clear();
	Entry.push_back(string(SoupNamesRO[SoupID[2]]));
	Entry.push_back(string(MainDishNamesRO[MainDishID[2]]));
	Entry.push_back(string(MainDishNamesVeganRO[MainDishIDVegan[2]]));
	Entry.push_back(string(MainDishNamesFishRO[MainDishIDFish[2]]));
	Entry.push_back(string(DesertNamesRO[DesertID[2]]));
	WriterRO.WriteEntry(Entry);

	return WriterRO.WriteEnd();
}