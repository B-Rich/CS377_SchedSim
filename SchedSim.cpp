#include "Headers.h"

//Constructor for SchedSim objects.
SchedSim::SchedSim(int maxProcesses, int maxCPUBursts, Algorithm algorithm, int quantum, FILE* dataFile)
	//initializer list
: 	_nextProcessID(0),
	_maxProcesses(maxProcesses),
	_remainingProcesses(maxProcesses),
	_maxCPUBursts(maxCPUBursts),
	_quantum(quantum),
	_compareProc(algorithm),
	_dataFile(dataFile),
	_IODev(new Device()),
	_CPUDev(new Device()),
	_eventHeap(),
	_readyQueue(algorithm),
	_IOQueue(),
	_processTable()
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

		printf("ProcessID\tLength\t\tArrival\t\tCompletion\tWait\t\tTotal Time\n");

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

//This is the discrete event simulator. 
//It's the "consumer" of events, and drives the simulation forward.
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

//printf("_eventHeap.size() = %d\n", (int)_eventHeap.size());
//printf("_IOQueue->size() = %d\n", (int)_IOQueue.size());
//printf("_readyQueue->size() = %d\n", (int)_readyQueue.size());
	printStatistics();
}


//returns NULL if a file read error occurs or if _remainingProcesses == 0
Process* SchedSim::getNextProcess() //reads the next process in from the file and creates the corresponding arrival event.
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
	for(int i = 0; i < numberOfCpuBursts; i++)
	{
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

//prints out an error message in response to file read errors.
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

//Adds the data from the process to the process table at the end of the process's lifetime. 
// The process can then be safely deleted.
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

//adds a process to the CPU device when the cpu device is empty.
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

//pre-empts the process on the CPU with a new process.
//note: this is not conditional. The conditional checking (whether it SHOULD pre-empt the old process) is done 
// in "shouldPreemptProcessOnDevice".
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

//puts a process on the ready queue for the CPU device.
void SchedSim::placeProcessOnReadyQueue(Process* process)
{
	process->setState(READY);
	process->setRQArrivalTime(_time);
	_readyQueue.push(process);
}


void SchedSim::processArrivalEvent() 
{
	Process* process = getNextProcess();
	
	//ignore null processes and processes with no bursts
	if(process == NULL || process->noBurstsRemaining())
	{
		return;
	}

	addDataToProcessTable(process); //for the collection of data.

	//for debugging purposes only.
	if(DEBUG_ARRIVAL)
	{
		printf("processing arrival event\n");
		printf("process ID = %d\n", process->getProcessID());
	}

	/*If CPU empty: get next process, set state to 'RUNNING', place on CPU device, create CPUEvent
	*	else: attempt to preempt executing process; if not successful, just place process on ready queue
	*/
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

//Logic for handling all cases of CPU burst completion. CPU Preemption is not considered part of this category.
void SchedSim::processCpuDoneEvent()
{	
	Process* oldProcess = _CPUDev->getProcess(); 	//the process currently on the CPU device
	oldProcess->deleteCurrentBurst();				//the last CPU burst of the process

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
//printf("terminating process\n");
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

//for handling IO burst completions. Again, this method does not handle preemptions;
// 		preemptions are handled by shouldPreemptProcessOnDevice.
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
	
	if(_CPUDev->hasProcessAndEvent()) //CPU device has a process on it
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
	else{ //CPU device doesn't have a process on it
		addProcessToEmptyCPU(process);
	}

	if(!_IOQueue.empty()) // add a process to the I/O device if one is waiting.
	{
		Process* newProcess = _IOQueue.front();
		_IOQueue.pop();
		addProcessToEmptyIO(newProcess);
	}
	else
	{
		_IODev->setProcessAndEvent(NULL, NULL); //if no processes are waiting for the I/O device, put a "null" process on it.
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

//Called by the three methods below to create an actual event in the _eventHeap 
Event* SchedSim::createEvent(Type type, double time)
{
	Event* event = new Event(type, time);
	_eventHeap.push(event);
	return event;
}
//calls createEvent
Event* SchedSim::createEventArrival(double time)
{
	return createEvent(ARRIVAL, time);
}
//calls createEvent
Event* SchedSim::createEventCPUDone(double time)
{
	return createEvent(CPU_DONE, time);
}
//calls createEvent
Event* SchedSim::createEventIODone(double time)
{
	return createEvent(IO_DONE, time);
}
