#pragma once
#include <string>

using namespace std;

#define RATIO_RON_USD ((double)0.2146893)
#define RATIO_USD_RON ((double)4.6579000)

class Currency
{
	public:
		unsigned long long Value;

		void Convert(double Ratio);
		string Format(const char DigitSepparator, const char DecimalSepparator, const string& Suffix);
};