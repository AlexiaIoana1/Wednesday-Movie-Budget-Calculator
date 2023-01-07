#include "Currency.hpp"

using namespace std;

void Currency::Convert(double Ratio)
{
	Value = ((unsigned long long) (((((double)Value) / 100) * Ratio) * 100));
}

string Currency::Format(const char DigitSepparator, const char DecimalSepparator, const string& Suffix)
{
	string ValueStr;
	unsigned long DigitCount = 0;
	unsigned long long Value = this->Value;
	unsigned long long Rev = 0;

	do
	{
		DigitCount++;
		Rev = Rev * 10 + Value % 10;
		Value = Value / 10;

	} while (Value > 0);

	for (unsigned long i = 0; i < DigitCount - 2; i++)
	{
		ValueStr.push_back('0' + Rev % 10);
		if ((DigitCount - 2 - i) % 3 == 0)
			ValueStr.push_back(DigitSepparator);

		Rev = Rev / 10;
	}
	ValueStr.push_back(DecimalSepparator);
	ValueStr.push_back('0' + Rev % 100 / 10);
	ValueStr.push_back('0' + Rev % 10);
	ValueStr.push_back(' ');
	ValueStr.append(Suffix);

	return ValueStr;
}