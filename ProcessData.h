//added because I can't think of a way to tie
//process arrival time with completion time
class ProcessData
{
private:
	int _processID;
	double _arrivalTime;
	double _completionTime;
public:
	ProcessData(int processID, double arrivalTime);
	int getProcessID();
	double getArrivalTime();
	void setCompletionTime(double completionTime);
	double getCompletionTime();
};
