/*
This is the header file for the class that
will be storing all of the info regarding
the metadata file and willrun the various processes.
*/

// compiler guards
#ifndef __METADATA_H_
#define __METADATA_H_

// include files
#include <string> 	// for data storage

// definition
class MetaData
{
	public:

		// basic constructor/destructor
		MetaData();
		~MetaData();

		// set data
		void setProcessNum( const int & );
		void setCycleTime( const int & );
		void setProcessRunTime( const int & );
		//void setNumOperations( const int & );
		void setProcessType( const char & );
		void setOperation( const std::string & );
		void setState( const int & );
		void setInterruptID( const int& );

		// get data
		int getProcessNum(  ) const;
		int getCycleTime(  ) const;
		int getProcessRunTime(  ) const;
		//int getNumOperations(  ) const;
		char getProcessType(  ) const;
		std::string getOperation(  ) const;
		int getState() const;
		int getInterruptID() const;

		// other functions
		void Reset();
		MetaData operator<(const MetaData &);

		// for debuggin
		void print() const;

	private:
		int processNum;
		int cycleTime;
		int processRunTime;
		char processType;
		std::string operation;
		int state;
		int interruptID;
};

// end compiler guards
#endif