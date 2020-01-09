/*
Description: This file contains the class necessary to analyze 
the config file provided and store the requisite components.
*/

#include "ConfigClass.h"// for reference to header

// constructor
ConfigClass::ConfigClass()
	{
		processor = 0;
		keyboard = 0;
		hardDrive = 0;
		printer = 0;
		monitor = 0;
		filePath = "";
		logPath = "";
		whereToLogTo = "";
		scheduleType = "";
	}

// destructor
ConfigClass::~ConfigClass()
	{
		processor = 0;
		keyboard = 0;
		hardDrive = 0;
		printer = 0;
		monitor = 0;
		filePath = "";
		logPath = "";
		whereToLogTo = "";
		scheduleType = "";
	}

// setting functions
void ConfigClass::setProc( int number ){
	processor = number; }

void ConfigClass::setKeyboard( int number ){
	keyboard = number; }

void ConfigClass::setHDD( int number ){
	hardDrive = number; }

void ConfigClass::setPrinter( int number ){
	printer = number; }

void ConfigClass::setMonitor( int number ){
	monitor = number; }

void ConfigClass::setQuantum( int number ){
	quantum = number; }

void ConfigClass::setFilePath( const std::string setPath ){
	filePath = setPath; }

void ConfigClass::setLogPath( const std::string setLocation ){
	logPath = setLocation; }

void ConfigClass::setWhereToLog( const std::string setMethod ){
	whereToLogTo = setMethod; }

void ConfigClass::setSchedulingType( const std::string setType ){
	scheduleType = setType; }

// getting functions
int ConfigClass::getProc( ) const{ 
	return processor; }

int ConfigClass::getKeyboard( ) const{
	return keyboard; }

int ConfigClass::getHDD( ) const{
	return hardDrive; }

int ConfigClass::getPrinter( ) const{
	return printer; }

int ConfigClass::getMonitor( ) const{
	return monitor; }

int ConfigClass::getQuantum( ) const{
	return quantum; }
	
std::string ConfigClass::getFilePath( ) const{
	return filePath; }

std::string ConfigClass::getLogPath(  ) const{
	return logPath; }
	
std::string ConfigClass::getWhereToLog(  ) const{
	return whereToLogTo; }

std::string ConfigClass::getSchedulingType( ) const{
	return scheduleType; }