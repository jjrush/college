/*
This is the header file for the class that will store all of the
data related to the configuration file. The function naming should
leave little room for erroneous interpretation.
*/

// compiler guards
#ifndef __CONFIGCLASS_H_
#define __CONFIGCLASS_H_

#include <string> 

// definition
class ConfigClass
{
	public:

		// basic
		ConfigClass();
		~ConfigClass();

		// set
		void setProc( const int );
		void setKeyboard( const int ); 
		void setHDD( const int );
		void setPrinter( const int );
		void setMonitor( const int );
		void setQuantum( const int );

		void setFilePath( const std::string );
		void setLogPath( const std::string );
		void setWhereToLog( const std::string );
		void setSchedulingType( const std::string );

		// get
		int getProc( ) const;
		int getKeyboard( ) const; 
		int getHDD( ) const;
		int getPrinter( ) const;
		int getMonitor( ) const;
		int getQuantum( ) const;

		std::string getFilePath( ) const;
		std::string getLogPath( ) const;
		std::string getWhereToLog( ) const;
		std::string getSchedulingType( ) const;

	private:
		int processor;
		int keyboard;
		int hardDrive;
		int printer;
		int monitor;
		int quantum;
		std::string filePath;
		std::string logPath;
		std::string whereToLogTo;
		std::string scheduleType;
};

// end compiler guards
#endif