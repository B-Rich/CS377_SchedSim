#include "Headers.h"

Process::Process(int processID)
	//initializer list
:	_bursts(),
	_processID(processID),
	_RQArrivalTime(0.0),
	_CPUArrivalTime(0.0),
	_totalCPUtime(0.0),
	_remainingCPUtime(0.0),
	_state(NEW)
{
}

double Process::getRQArrivalTime(){ //arrival time is the last time at which the process entered the ready queue.
	return _RQArrivalTime;
}

void Process::setRQArrivalTime(double newArrivalTime){
	_RQArrivalTime = newArrivalTime;
}

double Process::getCPUArrivalTime(){ 
	return _CPUArrivalTime;
}

void Process::setCPUArrivalTime(double newArrivalTime){
	_CPUArrivalTime = newArrivalTime;
}


int Process::getProcessID()
{
	return _processID;
}

State Process::getState()
{
	return _state;
}

void Process::setState(State state)
{
	_state = state;
}

double Process::getCurrentBurstTotal() //initial size of current burst
{
	if(DEBUG_PROCESS)
	{
		printf("Process::getCurrentBurstTotal()\n");
	}
	checkUnderflow();
	return _bursts.front()->first;
}

double Process::getCurrentBurstRemaining() //current size of current burst; is only =/= '_bursts.first' if the process has been preempted.
{
	if(DEBUG_PROCESS)
	{
		printf("Process::getCurrentBurstRemaining()\n");
	}
	checkUnderflow();
	return _bursts.front()->second;
}

void Process::addBurst(double burst)
{
	std::pair<double,double>* thePair = new std::pair<double,double>(burst,burst);
	_bursts.push(thePair); 
}

void Process::addCpuBurst(double burst)
{
	_totalCPUtime += burst;
	_remainingCPUtime += burst;
	addBurst(burst);
}

void Process::addIoBurst(double burst)
{
	addBurst(burst);
}

void Process::preemptCurrentBurst(double ranTime)
{
	if(DEBUG_PROCESS)
	{
		printf("Process::preemptCurrentBurst()\n");
	}
	checkUnderflow();
	_bursts.front()->second -= ranTime;
	_remainingCPUtime -= ranTime;
}

void Process::deleteCurrentBurst() //basically like moving forward one burst
{
	if(DEBUG_PROCESS)
	{
		printf("Process::deleteCurrentBurst()\n");
	}
	checkUnderflow();
	std::pair<double,double>* thePair = _bursts.front();
	_bursts.pop();
	_remainingCPUtime -= thePair->second;
	delete thePair;
}

int Process::getRemainingBurstCount()
{
	return _bursts.size();
}

bool Process::noBurstsRemaining()
{
	return _bursts.empty();
}

double Process::getTotalCpuTime()
{
	return _totalCPUtime;
}

double Process::getRemainingCpuTime()
{
	return _remainingCPUtime;
}

void Process::checkUnderflow()
{
	if(noBurstsRemaining())
	{
		throw QueueUnderflowException(); 
	}
}

const char* QueueUnderflowException::what() const throw()
{
	return "queue underflow";
}
