//added because I can't think of a way to tie
//process arrival time with completion time
class ProcessData
{
private:
	int _processID;
	double _arrivalTime;
	double _completionTime;
	double _totalCpuTime;
public:
	ProcessData(int processID, double arrivalTime);
	int getProcessID();
	double getArrivalTime();
	double getCompletionTime();
	void setCompletionTime(double completionTime);
	double getTotalCpuTime();
	void setTotalCpuTime(double totalCpuTime);
};
