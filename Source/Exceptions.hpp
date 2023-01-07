#pragma once

using namespace std;

class Exception
{
	public:
		union
		{
			struct
			{
				unsigned long LowValue;
				unsigned long HighValue;
			};
			size_t Value;
			const char* Info;
		}Context;
		
		Exception() { Context.Value = 0; Context.Info = NULL; }
		Exception(size_t Val) { Context.Value = Val; }
		Exception(const char* Info) { Context.Info = Info; }

		Exception(unsigned long ValA, unsigned long ValB) 
		{ 
			Context.LowValue = ValA;
			Context.HighValue = ValB; 
		}


		virtual string What() = 0;
};

class UnknownError : public Exception
{
	public:
		virtual string What() { return string("An unknown error occured."); }
};

class FileError : public Exception
{
	public:
		FileError() : Exception() {}
		FileError(const char* Info) : Exception(Info) {}

		virtual string What() { return string("An error occured while operating on file: ") + Context.Info + "."; }
};

class InvalidCSVFormat : public Exception
{
	public:
		InvalidCSVFormat() : Exception() {}
		InvalidCSVFormat(unsigned long Row, unsigned long Column) : Exception(Row, Column) {}

		virtual string What()
		{
			return  "Invalid csv format at line: " + to_string(Context.LowValue) +
				" column:" + to_string(Context.HighValue) +
				" in file: " + Context.Info;
				
		}
};

class NotEnoughNames : public Exception
{
	public:
		NotEnoughNames() : Exception() {}
		NotEnoughNames(unsigned long Requested, unsigned long Maximum) : Exception(Requested, Maximum) {}

		virtual string What()
		{ 
			return string("Not enough name combinations. Requested: ") + to_string(Context.LowValue) +
				   " Maximum: " + to_string(Context.HighValue);
		}
};