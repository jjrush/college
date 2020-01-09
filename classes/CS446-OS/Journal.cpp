/*
This is the implementation file for the class that will store all 
data associated with what processes have been run in the overall program.
This data will be output to a file before exiting the program.
*/
// definition

#include "Journal.h"
#include <list>
#include <fstream>
// basic constructor/destructor
Journal::Journal()
{
	storageString = "";
}
Journal::~Journal()
{
	storageString = "";
	// delete storage
	storage.clear();
}

// add to storage
void Journal::appendToString( const std::string &newString)
{
	storageString += newString;
}

void Journal::push()
{
	storage.push_back(storageString);
}

void Journal::resetString()
{
	storageString = "";
}
