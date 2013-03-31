#include "Headers.h"

Process::Process(int processID)
:	_processID(processID),_state(NEW),
	_RQArrivalTime(0.0),_CPUArrivalTime(0.0),
	_totalCPUtime(0.0),_remainingCPUtime(0.0),
	_bursts()
{
}

double Process::getRQArrivalTime(){ //arrival time is the last time at which the process entered the ready queue.
	return _RQArrivalTime;
}

void Process::setRQArrivalTime(double newArrivalTime){
	_RQArrivalTime = newArrivalTime;
}

double Process::getCPUArrivalTime(){ //arrival time is the last time at which the process entered the ready queue.
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

double Process::getCurrentBurstRemaining() //current size of current burst; is only =/= '.first' if the process has been preempted.
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
	_bursts.push(thePair); //TODO: check this for segfaults
	_totalCPUtime += burst;
	_remainingCPUtime += burst;
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

void Process::checkUnderflow()
{
	if(noBurstsRemaining())
	{
		throw QueueUnderflowException(); 
	}
}

bool CompareProcess::operator()(Process* p1, Process* p2)
{
	//TODO: Ensure that _bursts.front().first() of the process currently on the CPU isn't popped until after the process finishes its burst.
	//TODO: Ensure that new Process objects come with _CPUArrivalTime initialized to _time. This is used to calculate how long the current burst has been running.
	switch(SchedSim::getAlgorithm()){
		case FCFS:
		if(p1->getRQArrivalTime() < p2->getRQArrivalTime() ){
			return true;
		}
		else{
			return false;
		}
		break;
	
		case SJF:
		if(p1->getCurrentBurstTotal() < p2->getCurrentBurstTotal() ){
			return true;
		}
		else{ //special case of SJF: use FCFS if the next CPU bursts of the processes are the same length
			if(p1->getCurrentBurstTotal() == p2->getCurrentBurstTotal() ){
				if(p1->getRQArrivalTime() < p2->getRQArrivalTime() ){ //FCFS
					return true;
				}
				else{
					return false;
				}
			}
			return false;
		}
		break;
		
		case SRTF:
		if(
			p1->getCurrentBurstRemaining() - ( SchedSim::getCurrentTime() - p1->getCPUArrivalTime() ) 
			< 
			p2->getCurrentBurstRemaining() - ( SchedSim::getCurrentTime() - p2->getCPUArrivalTime() ) 
		){
		return true;
		}
		else{
			return false;
		}
		break;
		
		default:
			printf("we done goofed in Process.cpp. \n");	
	}
	return -1;
}

const char* QueueUnderflowException::what() const throw()
{
	return "queue underflow";
}
