#include "CSV.hpp"
#include "Exceptions.hpp"

using namespace  std;

csv::Reader::Reader() { File = NULL; WrittenEntryCount = 0; Row = 0; Column = 0; }
void csv::Reader::ReadBegin(ifstream* InFile) { File = InFile; WrittenEntryCount = 0; Row = 0; Column = 0; }
size_t csv::Reader::ReadEnd() { return WrittenEntryCount; }
bool csv::Reader::ReadEntry(vector<string>& Buffer)
{
	Buffer.clear();
	string Field;
	char c;

	while (*File >> noskipws >> c)
	{
		Column++;

		if (c == '"')
		{
			Field.clear();
			*File >> noskipws >> c;
			Column++;
			while (c != '"')
			{
				Field.push_back(c);
				*File >> noskipws >> c;
				Column++;
			}
			Buffer.push_back(Field);

			// End of file
			if (!(*File >> noskipws >> c))
			{
				WrittenEntryCount++;
				return true;
			}
			Column++;

			// End of entry
			if (c == '\n')
			{
				Column = 0;
				Row++;

				WrittenEntryCount++;
				return true;
			}

			if (c != '\n' && c != ',' && c != '\r')
				throw(InvalidCSVFormat(Row, Column));
			
		}
		else throw(InvalidCSVFormat(Row, Column));
	}

	if (Buffer.size() > 0)
	{
		WrittenEntryCount++;
		return true;
	}
	return false;
}

csv::Writer::Writer() { File = NULL; WrittenEntryCount = 0; }

void csv::Writer::WriteBegin(ofstream* OutFile)
{
	File = OutFile;
	WrittenEntryCount = 0;
}
size_t csv::Writer::WriteEnd()
{
	return WrittenEntryCount;
}

void csv::Writer::WriteEntry(const vector<string>& Entry)
{
	for (size_t i = 0; i < Entry.size(); i++)
	{
		*File << "\"" << Entry[i] << "\"";
		if (i != Entry.size() - 1)
			*File << ",";
	}
	WrittenEntryCount++;
}

void csv::Writer::WriteEntrySeparator()
{
	*File << "\n";
}