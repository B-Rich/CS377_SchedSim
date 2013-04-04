#include "CHeaders.h"
 
//Represent all possible process states.
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
	void checkUnderflow();			//Checks _bursts for emptiness to ensure that a null burst isn't deleted. 
	void addBurst(double burst); 	//Called by addCpuBurst and addIOBurst; actually adds the burst to the data structure 
	int _processID;
	double _RQArrivalTime; 		//the last time at which the process entered the ready queue
	double _CPUArrivalTime; 	//the time at which the process was last placed onto the cpu
	double _totalCPUtime;		//the total amount of time the process has spent on the cpu so far
	double _remainingCPUtime; 	//the total amount of time the process needs to spend on the cpu in order to terminate
	State _state;
	std::queue<std::pair<double,double>* > _bursts; //.first is the burst's total run time, .second is the burst's remaining run time. Alternates between CPU and I/O bursts
public:
	Process(int processID);
	void addCpuBurst(double burst); //calls addBurst with the appropriate type and increments CPU counters
	void addIoBurst(double burst);	//calls addBurst with the appropriate type
	void setRQArrivalTime(double newArrivalTime);	//sets the arrival time of the process to the CPU ready queue
	void preemptCurrentBurst(double time);	
	void deleteCurrentBurst();		//called every time a burst completes
	void setState(State state);
	void setCPUArrivalTime(double newArrivalTime);
	bool noBurstsRemaining();		//returns true if the process should terminate
	int getProcessID();
	int getRemainingBurstCount();
	double getRQArrivalTime();			//used for FCFS scheduling
	double getCPUArrivalTime();			//gets the arrival time of the process to the CPU device itself
	double getCurrentBurstTotal();		//gets the initial time of the current burst
	double getCurrentBurstRemaining();	//gets the remaining time of the current burst. Strictly <= currentBurstTotal
	double getTotalCpuTime();		//totalCpuTime is the sum of all bursts at the creation of the process
	double getRemainingCpuTime();	//remainingCpuTime is the sum of all currently remaining bursts of the process
	State getState();	
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
