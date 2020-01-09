/*/////////////////////////////////////////////////////////////////
Filename: Sim03.cpp

Note:  

Description: This program is a simulation of an operating system 
where we extract infomation from a configuration file and running 
operations from meta-data file. Threads are created to 
handle input/output operations. Visual output of results are available 
via the Monitor, File, or Both. Output displays each process being 
handled in real time as well as precision in milliseconds.

Version: 5.34673213.234t67245 // aka fuck this assignment

Author: 750730
Date: 11/13/15


// Program Help
This program is ran through the use of a file passed by the 
command-line. Simulation runs in conjuction with meta-data 
files generated separately. Please use "make" to compile all files.
Run program by typing "./Sim03 config_file_name_here".
/////////////////////////////////////////////////////////////////*/


// include files
#include <iostream>		// console I/O
#include <fstream>		// file I/O
#include <string>		// for string data structure
#include <list> 		// for "queueing" processes
#include <queue> 		// for queueing interrupts
#include <algorithm>	// for reducing strings to lowercase
#include <pthread.h> 	// for threading
#include <unistd.h> 	// for usleep
#include <iomanip>		// for precision
#include "SimpleTimer.h"// timing class
#include "ConfigClass.h"// for processing configuration file
#include "Journal.h"	// for collecting data to output to file later
#include "MetaData.h" 	// for processing meta data file

using namespace std;

// global variables
SimpleTimer TIMER;			// professor's timer class
double GLOBAL_RUN_TIME = 0;	// variable for keeping track of run times
int GLOBAL_INTERRUPT_COUNTER = 1;
int TOTAL_NUMBER_OF_PROGRAMS = 0;
list<int> interrupts;
pthread_t * threadArr;

/*//////////////////////////////////////////////////////////////

					Function Prototypes

//////////////////////////////////////////////////////////////*/

/*
Function Name: checkArgs
Description: checks the number of arguments on the command line
Parameters: integer indicating the number of arguments on the command line
Return: none
*/
void checkArgs( int );

/*
Function Name: analyzeConfig
Description: processes information in the provided configuration file
Parameters: file passed via command line to be opened
			class named ConfigClass that will store all info in the file
Return: none
*/
void analyzeConfig( const string &, 
				 	ConfigClass & );

/*
Function Name: extractData
Description: processes information from the metadata file specified in the config file
Parameters: MetaData 			 :: class to store all relevant information temporarily
			list<list<MetaData> >:: a list of lists of MetaData (for dynamically acquiring all processes)
			list<MetaData>       :: a list of metaData (for temporarily acquiring programs to be passed to upper list )
			ConfigClass  		 ::	for opening file to extract metadata info from
Return: none
*/
void extractData( MetaData &, 
				  list<list<MetaData> > &, 
				  list<MetaData> &, 
				  const ConfigClass & );

/*
Function Name: getNumCycles
Description: the number of cycles stored within a parameter string is extracted returned
Parameters: string that is extracted from file
Return: integer specifying the number of cycle times
*/
int getNumCycles( string );


/*
Function Name: performValidityTests
Description: processes string to determine if a validity error occurs
Parameters: string to be analyzed
			file operator to pull the string from
Return: integer that specifies what condition occurs
*/
int performValidityTests( string &, ifstream & );

/*
Function Name: getOperation
Description: extracts the name of an operation from a provided string
Parameters: string to be analyzed
Return: string with operation name
*/
string getOperation( string dummyStr );

/*
Function Name: calcTotalProgramRunTime
Description: processes string to determine if a validity error occurs
Parameters: list<MetaData>:: list of class MetaData where operations are stored
			ConfigClass	  :: ConfigClass where #s of cycles are stored
Return: integer that specifies the runtime for a particular set of operations
*/
int calcTotalProgramRunTime( const list<MetaData> &, 
				 const ConfigClass & );

/*
Function Name: convertToCases
Description: does a comparison on a string and returns an int based on it
Parameters: string to be analyzed
Return: integer that specifies what string was sent
*/
int convertToCases( const string& );

/*
Function Name: listSort
Description: sorts a given list
Parameters: list<list<MetaData> >:: list of lists that contain MetaData class objects for sorting
Return: none
Post: given list is sorted
*/
void listSort( list<list<MetaData> > &omegaList );

/*
Function Name: executeProcessing
Description: executes the processing of information dependent on which job type is specified
Parameters: list<list<MetaData> >:: list containing all data to processing
			ConfigClass			 :: class containing information necessary for operations
			Journal				 :: class containing a list of strings for file output
Return: none
*/
void executeProcessing( list<list<MetaData> > &,
					    const ConfigClass &,
					    Journal & );
/*
Function Name: getElapsedTime
Description: updates the GLOBAL_RUN_TIME variable
Parameters: none
Return: none
*/
void getElapsedTime();

/*
Function Name: executeThread
Description: executes an additional thread that waits a specified amount of time then dies
Parameters: void*:: this is really a double inside the function
Return: none
*/
void * executeThread( void * param );

/*
Function Name: outputToFile
Description: outputs data to a file
Parameters: Journal	   :: class that stores all information to send to file
			ConfigClass:: contains the file name to output to
Return: none
*/
void outputToFile( Journal notepad, 
				   const ConfigClass &configInfo );


/*
Function Name: performValidityTests
Description: processes string to determine if a validity error occurs
Parameters: ConfigInfo    :: contains information on how long operations take
			list<MetaData>:: list of operations for getting cycle #s
Return: integer that specifies what condition occurs
*/
double calcWaitTime( const ConfigClass& configInfo, 
	  				 list<MetaData> metaList );

void handleInterrupts( list<list<MetaData> > &omegaList, list<list<MetaData> > &waiting, list<MetaData> &running, int flag );

void printToScreen( const MetaData & );

//void findAndSet( list<MetaData> &waiting, list<MetaData> &running, const int &ID );

void executeFIFO( list<list<MetaData> > &omegaList, 
				  const ConfigClass &configInfo,
				  Journal &notepad );


/*/////////////////////////////////////////////////////////////

						Main Program

/////////////////////////////////////////////////////////////*/
int main(int argc, char const *argv[])
{
	// variables
	ConfigClass configInfo;			// for storing information related to configuration file
	MetaData metaInfo;				// for storing information related to meta data file
	list<list<MetaData> > omegaList;// ultimate list for dynamically storing # of programs + operations
	list<MetaData> metaList;		// list for storing operations
	Journal notepad;				// for storing data related to file output
	cout.precision(6);				// set precision of a cout for integers

	// check if arguments are valid
	checkArgs( argc );

	// process configuration file
	analyzeConfig( argv[1], configInfo );

	// process metadata
	extractData( metaInfo, omegaList, metaList, configInfo );

	// set the number of threads to allocate for
	threadArr = new pthread_t[TOTAL_NUMBER_OF_PROGRAMS];

	// execute program
	switch( convertToCases( configInfo.getSchedulingType() ) )
	{
		// FIFO
		case 5:
			// execute cycles in order
			executeFIFO( omegaList, configInfo, notepad );
			break;

		default:
			// improper 
			cout << "Error in execution. Quitting.";
			return 1;
			break;
	}
	// write to file if necessary
	if( configInfo.getWhereToLog() == "Both" || 
		configInfo.getWhereToLog() == "File" )
	{
		outputToFile( notepad, configInfo );
	}
// finish
return 0;
}

/*//////////////////////////////////////////////////////////////

					Function Implementations

//////////////////////////////////////////////////////////////*/
void checkArgs( int args )
{
	// check arguments
	if( args < 2 )
	{
		// file not provided
		cout << "ERROR: File not provided. Please provide a file via the command line. Quitting." << endl;
		exit(1);
	}
	else if( args > 2 )
	{
		// too many arguments
		cout << "Please provide just ONE argument via the command line. Quitting." << endl;
		exit(1);
	}
}

void analyzeConfig( const string &config_File, 
					ConfigClass &configInfo )
{
	// variables
	ifstream fin;
	string dummyStr = config_File;
	int dummyInt;

	// open file
	fin.open( dummyStr.c_str() );
	
	// check to make sure data is good
	if( fin.peek() == EOF || !fin.good() )
	{
		// notify user about lack of data
		cout << "Error: config file does not contain data or name is invalid. Quitting." << endl;
		exit(1);
	}

	// data is available so continue
	
	// get first superfluous string
	getline( fin, dummyStr );

	// get version 
	getline( fin, dummyStr );

	// get the file name for opening the meta data file
	fin >> dummyStr >> dummyStr >> dummyStr;
	configInfo.setFilePath( dummyStr );

	// get scheduling type
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr;
	configInfo.setSchedulingType( dummyStr );

	fin >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setQuantum( dummyInt );

	// get the processor cycling time
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setProc( dummyInt );

	// get the monitor cycling time
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setMonitor( dummyInt );

	// get the hard drive cycling time
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setHDD( dummyInt );

	// get the printer cycling time
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setPrinter( dummyInt );

	// get the keyboard cycling time
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr >> dummyInt;
	configInfo.setKeyboard( dummyInt );

	// get the log storage location (both or file or monitor)
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr; 
	configInfo.setWhereToLog( dummyStr );

	// get the log path (what file to store)
	fin >> dummyStr >> dummyStr >> dummyStr >> dummyStr;
	configInfo.setLogPath( dummyStr );

	// no need to get last line so close
	fin.clear();
	fin.close();
}

void extractData( MetaData &metaInfo, 
				  list<list<MetaData> > &omegaList, 
				  list<MetaData> &metaList,
				  const ConfigClass &configInfo )
{
	// variables
	ifstream fin;		// for file I/O
	string dummyStr;	// for acquiring data
	int index = 1;		// for setting process number
	int condition = 0; 	// for determining what actions to take
	double runtime = 0;
	list<MetaData>::iterator metaIterator = metaList.begin();

	// open the data file
	dummyStr = configInfo.getFilePath();

	fin.open( dummyStr.c_str() );

	// check validity of data
	if( fin.peek() == EOF || !fin.good() )
	{
		// notify user about lack of data
		cout << "Error: meta data file does not contain data or name is invalid. Quitting." << endl;
		exit(1);
	}

	// begin acquiring data
	
	// acquire useless line of text
	getline( fin, dummyStr );

	// acquire next two lines as they are always S start and A start
	fin >> dummyStr >> dummyStr;

	// set process number to 1 
	metaInfo.setProcessNum( 1 );

	// loop to acquire data
	while( fin.good() )
	{
		// collect data
		fin >> dummyStr;

		// perform validity tests
		condition = performValidityTests( dummyStr, fin );

		// check if new process beginning
		if( condition == 2 )
		{
			// new process beginning
			metaIterator = metaList.begin();

			// calculate process runtime 
			runtime = calcTotalProgramRunTime( metaList, configInfo );
			
			// set runtimes
			while( metaIterator != metaList.end() )
			{
				metaIterator->setProcessRunTime( runtime );
				metaIterator++;
			}

			// push the completed metaInfo into the list 
			omegaList.push_back( metaList );

			// reset list
			metaList.clear();

			// reset meta info
			metaInfo.Reset();

			// this is A(start) or S(end)
			fin >> dummyStr; 

			// check if it is s(end)
			if( dummyStr == "S(end)0." )
				break;

			// set process num for new run
			metaInfo.setProcessNum( ++index );
		}
		else
		{
			// string was valid so collect data
			metaInfo.setProcessType( dummyStr[0] );
			metaInfo.setCycleTime( getNumCycles( dummyStr ) );
			metaInfo.setOperation( getOperation( dummyStr ) );
			metaInfo.setProcessNum( index );

			// store operation
			metaList.push_back( metaInfo );
		}
		// begin overwriting metaInfo
		metaInfo.Reset();
	}

	fin.clear();
	fin.close();
}

int performValidityTests( string &dummyStr, ifstream &fin )
{
	// variables
	string dummyStr2;

	// check for "hard" to append following string
	if( strstr( dummyStr.c_str(), "hard" ) )
		{
		// found string "hard drive" inside of collected string

		// get and append next string containing "drive" into previous string
		fin >> dummyStr2;
		dummyStr = dummyStr + " " + dummyStr2;

		return 1;
		}

	if( strstr( dummyStr.c_str(), "end" ) )
		{
		// found string "end" inside of collected string

		// increment number of programs total
		TOTAL_NUMBER_OF_PROGRAMS++;

		// set up for new round of processes to begin

		return 2;
		}

	// none of the special conditions occured
	return 0;
}

int getNumCycles( string dummyStr )
{
	//variables
	int length = dummyStr.length();
	int strIndex = 0;
	int index = 0;
	int cycleNum = 0;
	int num1 = 0;
	int num2 = 0;
	char temp;

	// analyze string and extract number
	for (strIndex = 0; strIndex < length; strIndex++)
	{
		// check for identifier
		if( dummyStr[strIndex+1] == ')' )
		{
			// set normal index for counting
			index = strIndex+2;

			// one number is present
			if( (index+3 > length) || dummyStr[index+1] == ';' )
			{
				num1 = dummyStr[index] - '0';	
				return cycleNum = num1;
			}
			// two numbers are present 
			else
			{
				// the cycle time is two digits
				temp = dummyStr[index++];
				num1 = temp - '0';

				// multiple number by 10
				num1 *= 10;

				// get second number
				temp = dummyStr[index];
				num2 = temp - '0';

				return cycleNum = num1+num2;
			}
		}

	}
	// return regardless
	return cycleNum;
}

string getOperation( string dummyStr )
{
	// variables
	string subStr;
	int start = 5;
	int run = 3;
	int keyboard = 8;
	int monitor = 7;
	int printer = 7;
	int end = 3;

	// use if statements to acquire substring from string
	
	if( strstr( dummyStr.c_str(), "start" ) )
		subStr = dummyStr.substr( 2, start );

	else if( strstr( dummyStr.c_str(), "run" ) )
		subStr = dummyStr.substr( 2, run );

	else if( strstr( dummyStr.c_str(), "keyboard" ) )
		subStr = dummyStr.substr( 2, keyboard );

	else if( strstr( dummyStr.c_str(), "monitor" ) )
		subStr = dummyStr.substr( 2, monitor );

	else if( strstr( dummyStr.c_str(), "printer" ) )
		subStr = dummyStr.substr( 2, printer );

	else if( strstr( dummyStr.c_str(), "end" ) )
		subStr = dummyStr.substr( 2, end );
	else // it was hard drive
		subStr = "hard drive";

	return subStr;	
}

int calcTotalProgramRunTime( const list<MetaData> &metaList, 
				 const ConfigClass &configInfo )
{
	// variables
	int totalRunTime = 0;
	list<MetaData>::const_iterator listEnd = metaList.end();
	list<MetaData>::const_iterator listerator = metaList.begin();

	// iterate through metaList to get all operations
	while( listerator != listEnd )
	{
		// switch statements
		switch( convertToCases(listerator->getOperation()) )
		{
			case 0:
				// multiply number of cycles for hard drive by hard drive access time		
				totalRunTime += (listerator->getCycleTime() * configInfo.getHDD());
				break;
				
			case 1:
				// multiply number of cycles for processing by processing run time
				totalRunTime += (listerator->getCycleTime() * configInfo.getProc());
				break;

			case 2:
				// multiply number of cycles for monitor by monitor access time
				totalRunTime += (listerator->getCycleTime() * configInfo.getMonitor());
				break;

			case 3:
				// multiply number of cycles for keyboard by keyboard access time
				totalRunTime += (listerator->getCycleTime() * configInfo.getKeyboard());
				break;

			case 4:
				// multiply number of cycles for printer by printer access time
				totalRunTime += (listerator->getCycleTime() * configInfo.getPrinter());
				break;

			default:
				// do nothing
				break;

		}
		// increment to next item in list
		listerator++;
	}
	// return the total runtime for the program
	return totalRunTime;
}

int convertToCases( const string& operation )
{
	// check which string it is
	if( operation == "hard drive" )
		return 0;
	else if( operation == "run" )
		return 1;
	else if( operation == "monitor" )
		return 2;
	else if( operation == "keyboard" )
		return 3;
	else if( operation == "printer" )
		return 4;
	else if( operation == "FIFO-P" )
		return 5;
	else if( operation == "SRTF-P" )
		return 7;
	else if( operation == "Monitor" )
		return 8;
	else if( operation == "File" )
		return 9;
	else if( operation == "Both" )
		return 10;
	else
		return -1;
}

void getElapsedTime()
{
	// variables
	char* temp = new char[20];

	// stop the timer
	TIMER.stop();

	// get elapsed time
	TIMER.getElapsedTime( temp );

	// increment the global running time
	GLOBAL_RUN_TIME += atof( temp );

	// restart timer
	TIMER.start();
	// delete
	delete[] temp;
}

void outputToFile( Journal notepad, 
				   const ConfigClass &configInfo )
{
	// variables
	ofstream fout;
	
	// open file
	fout.open( configInfo.getLogPath().c_str() );
	
	// make sure notepad is not empty	
	if( !notepad.storage.empty() )
	{
		list<string>::iterator iterator = notepad.storage.begin();
		while( iterator != notepad.storage.end() )
		{
			// get data
			fout << *iterator << endl;
			iterator++;
		}
	}
	fout.close();
}

void * executeThread( void * param )
{
	// initialize variables
	double timeToExecute = 0.0;
	int temp = 0;

	// change parameter typing to double
	timeToExecute = *(double *)param;

	// create interrupt
	temp = GLOBAL_INTERRUPT_COUNTER++;

	// sleep
	usleep(timeToExecute);

	// successfully slept, push interrupt ID onto queue
	interrupts.push_back( temp );

	// exit thread
	pthread_exit(0);
}

double calcWaitTime( const ConfigClass& configInfo, 
					 list<MetaData> metaList )
{
	// variables
	list<MetaData>::iterator iterator = metaList.begin();
	switch( convertToCases( iterator->getOperation() ) ) 
	{
		// hard drive
		case 0:
			return configInfo.getHDD() 
				   * iterator->getCycleTime() 
				   * 1000;
			break;

		// monitor
		case 2:
			return configInfo.getMonitor() 
				   * iterator->getCycleTime() 
				   * 1000;
			break;

		// keyboard
		case 3:
			return configInfo.getKeyboard() 
				   * iterator->getCycleTime() 
				   * 1000;
			break;

		// printer
		case 4:
			return configInfo.getPrinter() 
				   * iterator->getCycleTime() 
				   * 1000;
			break;

		default:
			return 0.0;
			break;
	} 
}

void printToScreen( const MetaData &data )
{
	cout << fixed << GLOBAL_RUN_TIME << " - Process " << data.getProcessNum() << ": " << data.getOperation();

	if( data.getProcessType() == 'I' )
		cout << " input -";
	else if( data.getProcessType() == 'O' )
		cout << " output -";
	else
		cout << " processing -";

	if( data.getState() == 1 )
		cout << " continue " << endl;
	else if( data.getState() == 0 )
		cout << " start " << endl;
	else
		cout << " completed" << endl;
}

int calcOperationRunTime( const MetaData &data, const int &procRun )
{
	return (procRun * data.getCycleTime());
}

void handleInterrupts( list<list<MetaData> > &omegaList, list<list<MetaData> > &waiting, list<MetaData> &running, int flag )
{
	// iterator to waiting list
	list<list<MetaData> >::iterator waitingItr = waiting.begin();

	//list<MetaData> *newList = new list<MetaData>;

	// push currently running process onto ready queue
	omegaList.push_back( running );

	// loop while interrupts available
	while( !interrupts.empty() )
	{
		// compare the interupt IDs
		if( interrupts.front() == waitingItr->front().getInterruptID() )
		{
			// found so pop from interrupt queue
			interrupts.pop_front();

			// put out to the screen
			getElapsedTime();
			if( flag == 0 || flag == 2 )
			{
				cout << fixed << GLOBAL_RUN_TIME << " - Interrupt: Process " << waitingItr->front().getProcessNum() << ": " << waitingItr->front().getOperation();
				if( waitingItr->front().getProcessType() == 'I' )
					cout << " input completed" << endl;
				else
					cout << " output completed" << endl;
			}

			// set waiting program into running
			running = (*waitingItr);

			// pop front process
			running.pop_front();

			// reload running into ready
			omegaList.push_front( running );

			// clear portion of waiting list
			(*waitingItr).clear();

			// quit
			return;
		}
		else
			waitingItr++;
	}
}

void executeFIFO( list<list<MetaData> > &omegaList, 
				  const ConfigClass &configInfo,
				  Journal &notepad )
{
	// variables
	int flag = 50; // for determing whether to print to screen or file or both
	int quantum = 0;
	double timeToExecute = 0;
	list<list<MetaData> > waiting;
	list<MetaData> running;
	list<MetaData> *newList = new list<MetaData>;
	list<MetaData>::iterator runningItr = running.begin();

	// start timer
	TIMER.start();
	getElapsedTime();
	//temp2 = GLOBAL_RUN_TIME;

	// determine what to set flag to
	switch( convertToCases( configInfo.getWhereToLog() ) )
	{
		// to monitor
		case 8:
			flag = 0;
			break;

		// to file
		case 9:
			flag = 1;
			break;

		// to both
		case 10:
			flag = 2;
			break;

		default:
			break;
	}

	// initial output
	if( flag == 0 || flag == 2 )
	{
		// output
		cout << fixed << GLOBAL_RUN_TIME 
			 << " - Simulator program starting" << endl;
		getElapsedTime();
		//temp = GLOBAL_RUN_TIME;
		cout << fixed << GLOBAL_RUN_TIME
			 << " - OS: preparing all processes " << endl;
		getElapsedTime();
		cout << fixed << GLOBAL_RUN_TIME
			 << " - OS: selecting next process" << endl;
	}

	// calculate the wait time for the problem
	timeToExecute = calcWaitTime( configInfo, omegaList.front() );

	// while omega list is not empty
	while( !omegaList.empty() || !running.empty() )
	{
		// put first operation into running queue
		running = omegaList.front();

		omegaList.pop_front();

		// print intial process to screen
		if( flag == 0 || flag == 2 )
			printToScreen( running.front() );

		// while current program still has valid operations to execute
		while( !running.empty() || !waiting.empty() )	
		{
			// get operations

			// if operation is I/O
			if( (running.front().getProcessType() == 'I') ||
				(running.front().getProcessType() == 'O') )
			{
				// load interruptID 
				runningItr = running.begin();
				while( runningItr != running.end() )
					{
						runningItr->setInterruptID( GLOBAL_INTERRUPT_COUNTER );
						runningItr++;
					}

				// update user
				getElapsedTime();
				cout << fixed << GLOBAL_RUN_TIME << " - Process " << running.front().getProcessNum() << ": block for " << running.front().getOperation();
				if( running.front().getProcessType() == 'I' )
					cout << " input" << endl;
				else
					cout << " output" << endl;
				// thread
				pthread_create( &threadArr[running.front().getProcessNum() - 1] , NULL, executeThread, (void *)&timeToExecute );

				// load temp list with running list
				(*newList) = running;

				// push running onto waiting
				waiting.push_back( (*newList) );

				// delete running
				running.clear();

				if( !omegaList.empty() )
				{
					running = omegaList.front();

					// delete from ready
					omegaList.pop_front();

					// update user
					getElapsedTime();
					cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process" << endl;
					
					if( !running.empty() )
						printToScreen( running.front() );				
				}

				// clear temp list
				(*newList).clear();
			}
			// else if operation is a processing action
			else if( running.front().getProcessType() == 'P' )
			{
				// while quantum not reached
				while( quantum < configInfo.getQuantum() )
				{
					// interrupts present
					if( !interrupts.empty() )
					{
						// find interrupt in waiting queue via interrupt ID and load all of them back into running
						handleInterrupts( omegaList, waiting, running, flag );

						// set new operations to perform
						if( !omegaList.empty() )
						{
							running = omegaList.front();					
							
							omegaList.pop_front();

							if( !running.empty() )
							{
								// update user
								getElapsedTime();
								cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process" << endl;
								printToScreen( running.front() );				
							}
						}

						// break out of processing
						break;
					}
					// interrupt not present
					else
					{
						// sleep for 1 processor cycle
						usleep( 30000 );

						// increment quantum counter
						quantum++;

						// calculate new run time
						running.front().setCycleTime( running.front().getCycleTime() - 1 );

						// update runtime
						running.front().setProcessRunTime( calcOperationRunTime( running.front(), configInfo.getProc() ) );

						// if process complete
						if( running.front().getProcessRunTime() == 0 )
						{
							// finishes shit
							getElapsedTime();
							running.front().setState(2);

							// print to screen
							printToScreen( running.front() );

							// pop process
							running.pop_front();
							
							// reset quantum number
							quantum = 0;

							getElapsedTime();
							if( !running.empty() )
								cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process" << endl;

							getElapsedTime();
							if( !running.empty() )
								printToScreen( running.front() );

							break;
						}
						// else if quantum reached
						else if( quantum == configInfo.getQuantum() )
						{
							// update
							getElapsedTime();

							// inform of quantum time out
							cout << fixed << GLOBAL_RUN_TIME << " - Interrupt: Process " << running.front().getProcessNum() << ": quantum time out" << endl;

							// inform user of selecting next process
							getElapsedTime();
							cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process" << endl;

							// reset quantum
							quantum = 0;

							// set new state
							running.front().setState(1);

							// set to omegaList only if operations present
							if( !omegaList.empty() )
							{
								// store list
								(*newList) = running;

								// delete list
								running.clear();

								// set running
								running = omegaList.front();

								// pop front
								omegaList.pop_front();

								// load program back into omegaList
								omegaList.push_back( (*newList ) );

								// print to screen
								printToScreen( running.front() );
							}
						        // omegaList is empty (only 1 program left so just use running again)


							// print to screen
							if( flag == 0 || flag == 2 )
							{
								getElapsedTime();
								printToScreen( running.front() );
							}
							break;
						}
					}
				}
			}
			else if( running.empty() && !waiting.empty() )
			{
				//report
				getElapsedTime();
				cout << fixed << GLOBAL_RUN_TIME << " - OS: Idle" << endl;

				// continue idling until interrupt queue has an interrupt
				while( interrupts.empty() )
					{

					}

				// find interrupt in waiting queue via interrupt ID and load all of them back into running
				handleInterrupts( omegaList, waiting, running, flag );

				// set new operations to perform
				if( !omegaList.empty() )
				{
					// update user
					getElapsedTime();
					cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process" << endl;
cout << "\n\nhere?" << endl;
					running = omegaList.front();					
					
					omegaList.pop_front();

					if( !running.empty() )
						printToScreen( running.front() );				
				}

				// break out of processing
				break;
			}
		}

		// update
		getElapsedTime();
		if( flag == 0 || flag == 2 )
			cout << fixed << GLOBAL_RUN_TIME << " - OS: removing process" << endl;
		else
		{
			notepad.appendToString( to_string( GLOBAL_RUN_TIME ) );
			notepad.appendToString( " - OS: removing process" );
			notepad.push();
			notepad.resetString();
		}
		// prepare for next loop
		running.clear();

		if( !omegaList.empty() && !waiting.empty() ) // update user
		{
			getElapsedTime();
			cout << fixed << GLOBAL_RUN_TIME << " - OS: selecting next process " << endl;
	cout << "\n\nhere2" << endl;	
		}
		// quit program otherwise
	}

	// update time and print
	getElapsedTime();
	if( flag == 0 || flag == 2 )
	{
		// output
		cout << fixed << GLOBAL_RUN_TIME 
			 << " - Simulator program ending" << endl;
	}
	else
	{
		// output
		notepad.appendToString( to_string( GLOBAL_RUN_TIME ) );
		notepad.appendToString( " - Simulator program ending" );
		notepad.push();
		notepad.resetString();
	}
}
