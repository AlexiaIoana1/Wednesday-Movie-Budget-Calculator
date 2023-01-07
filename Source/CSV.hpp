#pragma once
#include <fstream>
#include <string>
#include <vector>

using namespace std;

namespace csv
{
	class Reader
	{
		private:
			ifstream* File;
			size_t WrittenEntryCount;
			
			unsigned long Row;
			unsigned long Column;

		public:
			Reader();
			void ReadBegin(ifstream*);
			bool ReadEntry(vector<string>&);
			size_t ReadEnd();
	};

	class Writer
	{
		private:
			ofstream* File;
			size_t WrittenEntryCount;

		public:
			Writer();
			void WriteBegin(ofstream*);
			void WriteEntry(const vector<string>&);
			void WriteEntrySeparator();
			size_t WriteEnd();
	};
}