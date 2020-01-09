/*
This is the header file for the class that will store all 
data associated with what processes have been run in the overall program.
This data will be output to a file before exiting the program.
*/

// compiler guards
#ifndef __JOURNAL_H_
#define __JOURNAL_H_

// include files
#include <string> 	// for data storage
#include <list> 	// for data storage
// definition
class Journal
{
	public:

		// basic constructor/destructor
		Journal();
		~Journal();

		// operations
		void appendToString( const std::string &);
		void push();
		std::string get();
		void resetString();

		// public member functions
		std::list<std::string> storage;

	private:
		std::string storageString;

};

// end compiler guards
#endif