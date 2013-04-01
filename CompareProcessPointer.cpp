#include "Headers.h"

bool CompareProcessPointer::compareFCFS(Process* p1, Process* p2)
{
	//Since priority queue creates a max heap, negate
	//	inequality to form a min heap.
	if(p1 == NULL) { return false; }
	if(p2 == NULL) { return true; }
	return p1->getRQArrivalTime() > p2->getRQArrivalTime();
}

bool CompareProcessPointer::compareSJF(Process* p1, Process* p2)
{
	if(p1 == NULL) { return false; }
	if(p2 == NULL) { return true; }
	return p1->getTotalCpuTime() < p2->getTotalCpuTime();
}

bool CompareProcessPointer::compareSRTF(Process* p1, Process* p2)
{
	return true;
}

CompareProcessPointer::CompareProcessPointer(Algorithm algorithm)
{
	switch(algorithm)
	{
		case FCFS:
			_comparator = &compareFCFS;
			break;
		case SJF:
			_comparator = &compareSJF;
			break;
		case SRTF:
			_comparator = &compareSRTF;
			break;
	}
}

bool CompareProcessPointer::operator()(Process* p1, Process* p2)
{
	return _comparator(p1, p2);
}
