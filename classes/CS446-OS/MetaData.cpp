/*
This file is the implementation file of the MetaData class.
This file will hold all of the implementations of the functions
necessary to operate on the meta data acquired in the program.
*/
#include "MetaData.h"
#include <iostream>

// constructors
MetaData::MetaData()
	{
	// set default
	processNum = 100;
	cycleTime = 0;
	processRunTime = 0;
	processType = ' ';
	operation = " ";
	state = 0;
	interruptID = 0;
	}

MetaData::~MetaData()
	{
	// reset default
	processNum = 0;
	cycleTime = 0;
	processRunTime = 0;
	processType = ' ';
	operation = "";
	state = 0;
	interruptID = 0;
	}

// set data
void MetaData::setProcessNum( const int &num){
	processNum = num;}

void MetaData::setCycleTime( const int &num ){
	cycleTime = num;}

void MetaData::setProcessRunTime( const int &num ){
	processRunTime = num;}

void MetaData::setProcessType( const char &type ){
	processType = type;}

void MetaData::setOperation( const std::string &op ){
	operation = op;}

void MetaData::setState( const int &newState ){
	state = newState;}

void MetaData::setInterruptID( const int &newID ){
	interruptID = newID;}

// get data
int MetaData::getProcessNum(  ) const{
	return processNum;}

int MetaData::getCycleTime(  ) const{
	return cycleTime;}

int MetaData::getProcessRunTime(  ) const{
	return processRunTime;}

char MetaData::getProcessType(  ) const{
	return processType;}

std::string MetaData::getOperation(  ) const{
	return operation;}

int MetaData::getState(  ) const{
	return state;}

int MetaData::getInterruptID(  ) const{
	return interruptID;}

// other functions
void MetaData::Reset() 
{
	processNum = 0;
	cycleTime = 0;
	processRunTime = 0;
	processType = ' ';
	operation = "";
}

// overloaded < operator
MetaData MetaData::operator<(const MetaData &m)
{
	if( this->processRunTime < m.processRunTime )
		return *this;
	else
		return m;
}

// debugging
void MetaData::print() const
{
	std::cout << "MetaData info: " << std::endl;
	std::cout << "ProcessNum: " << processNum << std::endl;
	std::cout << "cycleTime: " << cycleTime << std::endl;
	std::cout << "processRunTime: " << processRunTime << std::endl;
	std::cout << "processType: " << processType << std::endl;
	std::cout << "operation: " << operation << std::endl;
}
