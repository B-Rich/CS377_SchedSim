#include "CHeaders.h"

class SchedSim
{
private:
	int _maxProcesses;
	int _remainingProcesses;
	int _maxCPUBursts;
	static Algorithm _algorithm;
	int _quantum;
	Device* _IODev;
	Device* _CPUDev;
	std::priority_queue<Event*, std::vector<Event*>, CompareEvent> _eventHeap;
	std::priority_queue<Process*, std::vector<Process*>, CompareProcess> _oldReadyQueue;
	std::priority_queue<Process*, std::vector<Process*>, CompareProcessPointer> _readyQueue;
	std::queue<Process*> _IOQueue;
	static double _time;	//made static
	std::map<int, ProcessData*> _processTable;
	int _nextProcessID;
	FILE* _dataFile;
	CompareProcess _oldCompareProc;
	CompareProcessPointer _compareProc;

	void processArrivalEvent();

	void processCpuDoneEvent();

	void processIoDoneEvent();

	Event* createEventArrival(double time);

	Event* createEventCPUDone(double time);

	Event* createEventIODone(double time);

	Event* createEvent(Type type, double time);

	Process* getNextProcess();

	void handleFileReadError();

	bool processShouldPreemptProcessOnDevice(Process* process, Device* device);

	void printStatistics();

	void addDataToProcessTable(Process* process);

	void addProcessToEmptyCPU(Process* process);

	void preemptProcessOnCpuWithGivenProcess(Process* process);

	void placeProcessOnReadyQueue(Process* process);

	void addProcessToEmptyIO(Process* process);
public:
	SchedSim(int maxProcesses, int maxCPUBursts, Algorithm algorithm, int quantum, FILE* dataFile);

	static Algorithm getAlgorithm();
	
	static double getCurrentTime(); //added

	void DES();
};
