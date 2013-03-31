#include "Headers.h"

//This is the discrete event simulator. 
//It's basically the entrypoint into everything in this file.
void SchedSim::DES()
{
	Event* currEvent = 0; 

	while(!_eventHeap.empty())
	{
		currEvent	= _eventHeap.top();
		_eventHeap.pop();
		_time = currEvent->getTime();
		
		switch(currEvent->getType())
		{
			case ARRIVAL	:
				processArrivalEvent();
				break;
			case CPU_DONE	:
				processCpuDoneEvent();
				break;
			case IO_DONE	:
				processIoDoneEvent();
				break;
			case CANCELLED:
				break;
			default:
				printf("we done goofed in SchedSim::DES. \n");
		}

		delete currEvent;
	}

printf("_eventHeap.size() = %d\n", (int)_eventHeap.size());
printf("_IOQueue->size() = %d\n", (int)_IOQueue.size());
printf("_readyQueue->size() = %d\n", (int)_readyQueue.size());
	printStatistics();
}

//returns NULL if a file read error occurs
// or _remainingProcesses = 0
Process* SchedSim::getNextProcess()
{
	if(_remainingProcesses == 0)
	{
		return NULL;
	}

	unsigned char byte;
	unsigned char* twoNminusOneBytes;
	double nextProcessTime;
	int numberOfCpuBursts;
	int twoNminusOne;
	size_t result;

	result = fread(&byte, 1, 1, _dataFile);
	if(result != 1)
	{
		handleFileReadError();
		return NULL;
	}
	nextProcessTime = byte / 10.0;

	if(DEBUG_BINARY)
	{
		printf("nextProcessTime = %f\n", nextProcessTime);
	}

	result = fread(&byte, 1, 1, _dataFile);
	if(result != 1)
	{
		handleFileReadError();
		return NULL;
	}
	numberOfCpuBursts = byte % _maxCPUBursts + 1;

	if(DEBUG_BINARY)
	{
		printf("numberOfCpuBursts = %d\n", numberOfCpuBursts);
	}

	twoNminusOne = 2 * numberOfCpuBursts - 1;
	twoNminusOneBytes = new unsigned char[twoNminusOne];
	result = fread(twoNminusOneBytes, 1,
		twoNminusOne, _dataFile);
	if(result != (size_t)twoNminusOne)
	{
		handleFileReadError();
		delete twoNminusOneBytes;
		return NULL;
	}

	//OK to create the next arrival event since file
	//	file read was successful
	createEventArrival(_time + nextProcessTime);

	Process* process = new Process(_nextProcessID);
	_nextProcessID++;

	//hack to create CPU bursts from the first n
	//	bytes and IO bursts from the last n-1 bytes
	//	yet have them interleaved in Process _burst
	//for(int i = 0; i < twoNminusOne; i++)
	for(int i = 0; i < numberOfCpuBursts; i++)
	{
		//process->addBurst(twoNminusOneBytes[i] / 25.6);
		process->addCpuBurst(twoNminusOneBytes[i] / 25.6);

		if(i < numberOfCpuBursts - 1)
		{
			process->addIoBurst(twoNminusOneBytes[i + numberOfCpuBursts] / 25.6);
		}

		if(DEBUG_BINARY)
		{
			printf("cpu byte[%d] = %u\n", i, twoNminusOneBytes[i]);
			if(i < numberOfCpuBursts - 1)
			{
				printf("io byte[%d] = %u\n", i, twoNminusOneBytes[i + numberOfCpuBursts]);
			}

			printf("cpu burst[%d] = %f\n", i, twoNminusOneBytes[i] / 25.6);
			if(i < numberOfCpuBursts - 1)
			{
				printf("io burst [%d] = %f\n", i, twoNminusOneBytes[i + numberOfCpuBursts] / 25.6);
			}
		}
	}

	delete twoNminusOneBytes;

	_remainingProcesses--;

	return process;
}

void SchedSim::handleFileReadError()
{
	printf("Handling file read error\n");
	if(feof(_dataFile))
	{
		printf("Error end of file\n");
	}
	else if(ferror(_dataFile))
	{
		printf("Error reading file\n");
	}
	exit(1);
}

void SchedSim::addDataToProcessTable(Process* process)
{
	ProcessData* processData = new ProcessData(process->getProcessID(),_time);
	processData->setCompletionTime(_time);
	processData->setTotalCpuTime(process->getTotalCpuTime());
	_processTable.insert
		(
			std::pair<int,ProcessData*>
				(
					process->getProcessID(),
					processData
				)
		);
}

void SchedSim::addProcessToEmptyCPU(Process* process)
{
	process->setState(RUNNING);
	_CPUDev->setProcessAndEvent
		(
			process,
			createEventCPUDone
				(
					_time +
					process->getCurrentBurstRemaining()
				)
		);
	process->setCPUArrivalTime(_time);
}

void SchedSim::preemptProcessOnCpuWithGivenProcess(Process* process)
{
	//old process
	Process* oldProcess = _CPUDev->getProcess();
	Event* oldEvent = _CPUDev->getEvent();

	oldProcess->preemptCurrentBurst(_time - oldProcess->getCPUArrivalTime()); 
	placeProcessOnReadyQueue(oldProcess);

	oldEvent->setType(CANCELLED);
	
	//new process
	addProcessToEmptyCPU(process);
}

void SchedSim::placeProcessOnReadyQueue(Process* process)
{
	process->setState(READY);
	process->setRQArrivalTime(_time);
	_readyQueue.push(process);
}

void SchedSim::processArrivalEvent() 
{
	Process* process = getNextProcess();

	if(process == NULL || process->noBurstsRemaining())
	{
		return;
	}

	addDataToProcessTable(process);

	if(DEBUG_ARRIVAL)
	{
		printf("processing arrival event\n");
		printf("process ID = %d\n", process->getProcessID());
	}

	//If CPU empty: get next process, set state to 'RUNNING', place on CPU device, create CPUEvent
	//			else: attempt to preempt executing process; if not successful, just place process on ready queue
	
	if(!_CPUDev->hasProcessAndEvent())
	{
		addProcessToEmptyCPU(process);
	}
	else if(processShouldPreemptProcessOnDevice(process, _CPUDev))
	{
		preemptProcessOnCpuWithGivenProcess(process);
	}
	else
	{
		placeProcessOnReadyQueue(process);
	}
}

bool SchedSim::processShouldPreemptProcessOnDevice(Process* process, Device* device)
{
	return _compareProc(process, device->getProcess());
}

void SchedSim::processCpuDoneEvent()
{
	//TODO: Check whether the old process is now doing I/O or simply used its entire quanta. 
	
	Process* oldProcess = _CPUDev->getProcess();
	oldProcess->deleteCurrentBurst();

	if(DEBUG_CPU_DONE)
	{
		printf("*****processing cpu done event\n");
		printf("process ID = %d\n", oldProcess->getProcessID());
	}

	//if old process has bursts remaining 
	//  and 'IODevice' doesn't have a Process and Event,
	//	then add the process to the IODevice.
	//	else, set the old process' state to waiting and put it on the I/O queue.
	if(!oldProcess->noBurstsRemaining())
	{
		if(!_IODev->hasProcessAndEvent())
		{
			addProcessToEmptyIO(oldProcess);
		}
		else
		{
			oldProcess->setState(WAITING); 	
			_IOQueue.push(oldProcess);		
		}
	}
	else //the old process is terminated because it has no bursts remaining.
	{
printf("terminating process\n");
		oldProcess->setState(TERMINATED);
		//update data in _processTable
		_processTable[oldProcess->getProcessID()]->setCompletionTime(_time);
		delete oldProcess;
	}
	
	//new process (going on now-empty CPU device)
	if(!_readyQueue.empty())
	{
		Process* newProcess = _readyQueue.top();
		_readyQueue.pop();
		addProcessToEmptyCPU(newProcess);
	}
	else
	{
		_CPUDev->setProcessAndEvent(NULL, NULL);
	}
}

void SchedSim::processIoDoneEvent()
{
	Process* process = _IODev->getProcess();
	process->deleteCurrentBurst();

	if(DEBUG_IO_DONE)
	{
		printf("*****processing io done event\n");
		printf("process ID = %d\n", process->getProcessID());
		printf("_IOQueue.size() = %d\n", (int)_IOQueue.size());
	}

	/*
	if(processShouldPreemptProcessOnDevice(process, _CPUDev))
	{
		preemptProcessOnCpuWithGivenProcess(process);
	}
	else
	{
		if(!_CPUDev->hasProcessAndEvent())
		{
			addProcessToEmptyCPU(process);
		}
		else
		{
			placeProcessOnReadyQueue(process);
		}
	}
	*/
	
	if(_CPUDev->hasProcessAndEvent())
	{
		if(processShouldPreemptProcessOnDevice(process, _CPUDev))
		{
			preemptProcessOnCpuWithGivenProcess(process);
		}
		else
		{
			placeProcessOnReadyQueue(process);
		}
	}

	if(!_IOQueue.empty())
	{
		Process* newProcess = _IOQueue.front();
		_IOQueue.pop();
		addProcessToEmptyIO(newProcess);
	}
	else
	{
		_IODev->setProcessAndEvent(NULL, NULL);
	}
}

//Adds a process to the IO device. Called only when the IO device is empty and there is nothing in the queue.
void SchedSim::addProcessToEmptyIO(Process* process)
{
	process->setState(IO);
	_IODev->setProcessAndEvent
		(
			process,
			createEventIODone
				(
					_time +
					process->getCurrentBurstRemaining()
				)
		);
}

Event* SchedSim::createEventArrival(double time)
{
	return createEvent(ARRIVAL, time);
}


Event* SchedSim::createEventCPUDone(double time)
{
	return createEvent(CPU_DONE, time);
}

Event* SchedSim::createEventIODone(double time)
{
	return createEvent(IO_DONE, time);
}

Event* SchedSim::createEvent(Type type, double time)
{
	Event* event = new Event(type, time);
	_eventHeap.push(event);
	return event;
}

SchedSim::SchedSim(int maxProcesses, int maxCPUBursts, Algorithm algorithm, int quantum, FILE* dataFile)
: _maxProcesses(maxProcesses),_remainingProcesses(maxProcesses),_maxCPUBursts(maxCPUBursts),_quantum(quantum),
	_IODev(new Device()),_CPUDev(new Device()),_eventHeap(),_readyQueue(algorithm),_IOQueue(),
	_processTable(),_nextProcessID(0),_dataFile(dataFile),
	_compareProc(algorithm) //removed _time(0.0), moved to bottom of this file.
{
	_algorithm = algorithm;
	createEventArrival(_time);
}

Algorithm SchedSim::getAlgorithm()
{
	return _algorithm;
}

double SchedSim::getCurrentTime()
{
	return _time;
}

void SchedSim::printStatistics()
{
	if(!_processTable.empty())
	{
		std::map<int,ProcessData*>::iterator it;
		double complete = 0.0;
		double length = 0.0;
		double wait = 0.0;
		int numberOfProcesses = (int)_processTable.size();
		double totalWait = 0.0;
		double totalComplete = 0.0;

		printf("ProcessID\tLength\t\tArrival\t\tCompletion\tWait\t\tComplete\n");

		for(it = _processTable.begin(); it != _processTable.end(); it++)
		{
			complete = it->second->getCompletionTime() -
				it->second->getArrivalTime();
			length = it->second->getTotalCpuTime();
			wait = complete - length;

			printf("%d\t\t%f\t%f\t%f\t%f\t%f\n",
				it->first,
				length,
				it->second->getArrivalTime(),
				it->second->getCompletionTime(),
				wait,
				complete);

			totalWait += wait;
			totalComplete += complete;

			delete it->second;
		}

		printf("Average\t\t\t\t\t\t\t\t%f\t%f\n",
			totalWait / numberOfProcesses,
			totalComplete / numberOfProcesses);
	}
}

Algorithm SchedSim::_algorithm = FCFS; //default
double SchedSim::_time = 0.0; //initialization of static variable
