#include "Headers.h"

ProcessData::ProcessData(int processID, double arrivalTime)
:	_processID(0),_arrivalTime(arrivalTime),
	_completionTime(0.0)
{
}

int ProcessData::getProcessID()
{
	return _processID;
}

double ProcessData::getArrivalTime()
{
	return _arrivalTime;
}

void ProcessData::setCompletionTime(double completionTime)
{
	_completionTime = completionTime;
}

double ProcessData::getCompletionTime()
{
	return _completionTime;
}
