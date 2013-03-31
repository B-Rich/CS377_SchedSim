#include "CHeaders.h"
 
enum State
{
	NEW,
	READY,
	RUNNING,
	IO,
	WAITING,
	TERMINATED
};

class Process
{
private:
	int _processID;
	State _state;
	double _RQArrivalTime; //arrival time is the last time at which the process entered the ready queue.
	double _CPUArrivalTime; // is the time at which the process was last placed onto the cpu.
	double _totalCPUtime;
	double _remainingCPUtime;
	std::queue<std::pair<double,double>* > _bursts; //.first is the burst's total run time, .second is the burst's remaining run time.
	void checkUnderflow();
	void addBurst(double burst);
public:
	Process(int processID);
	int getProcessID();
	double getRQArrivalTime();
	void setRQArrivalTime(double newArrivalTime);
	double getCPUArrivalTime();
	void setCPUArrivalTime(double newArrivalTime);
	State getState();
	void setState(State state);
	double getCurrentBurstTotal();
	double getCurrentBurstRemaining();
	void preemptCurrentBurst(double time);
	void deleteCurrentBurst();
	int getRemainingBurstCount();
	bool noBurstsRemaining();
	void addCpuBurst(double burst);
	void addIoBurst(double burst);
	double getTotalCpuTime();
	double getRemainingCpuTime();
};

class CompareProcess
{
public:
	bool operator()(Process* p1, Process* p2);
};

class QueueUnderflowException : public std::exception
{
	virtual const char* what() const throw();
};
