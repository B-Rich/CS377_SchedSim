#include "CHeaders.h"

class SchedSim
{
private:
	int _nextProcessID;
	int _maxProcesses;
	int _remainingProcesses;
	int _maxCPUBursts;
	int _quantum;
	static Algorithm _algorithm;
	static double _time;
	CompareProcessPointer _compareProc; //Custom comparator for processes. Behavior varies based on algorithm.
	FILE* _dataFile;
	Device* _IODev;
	Device* _CPUDev;
	std::priority_queue<Event*, std::vector<Event*>, CompareEvent> _eventHeap; //holds events in sorted order of time.
	std::priority_queue<Process*, std::vector<Process*>, CompareProcessPointer> _readyQueue; //processes waiting for CPU
	std::queue<Process*> _IOQueue;	//processes waiting for I/O
	std::map<int, ProcessData*> _processTable;

	void processArrivalEvent(); //processes "process arrival" events

	void processCpuDoneEvent(); //processes "cpu done" events (cpu burst completion)

	void processIoDoneEvent();	//processes "I/O done" events (I/O burst completion)

	void handleFileReadError();

	void printStatistics();

	void addDataToProcessTable(Process* process);

	void addProcessToEmptyCPU(Process* process);

	void preemptProcessOnCpuWithGivenProcess(Process* process);

	void placeProcessOnReadyQueue(Process* process);

	void addProcessToEmptyIO(Process* process);
	
	bool processShouldPreemptProcessOnDevice(Process* process, Device* device);
	
	Event* createEventArrival(double time);

	Event* createEventCPUDone(double time);

	Event* createEventIODone(double time);

	Event* createEvent(Type type, double time);

	Process* getNextProcess();
	
public:
	SchedSim(int maxProcesses, int maxCPUBursts, Algorithm algorithm, int quantum, FILE* dataFile);

	static Algorithm getAlgorithm();
	
	static double getCurrentTime(); //added

	void DES();
};
