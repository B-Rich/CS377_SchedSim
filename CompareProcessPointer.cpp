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
	
	// SJF if there's not a tie
	//(total cpu time is the sum of all cpu bursts at the creation of the process.)
	if(p1->getTotalCpuTime() != p2->getTotalCpuTime()){	
		return p1->getTotalCpuTime() < p2->getTotalCpuTime();
	}
	else{// FCFS on a tie
		return p1->getRQArrivalTime() > p2->getRQArrivalTime();
	}
}

bool CompareProcessPointer::compareSRTF(Process* p1, Process* p2)
{
	if(p1 == NULL) { return false; }
	if(p2 == NULL) { return true; }
	//remaining cpu time is the sum of all cpu bursts that have not yet been executed, or executed completely due to preemption.
	if(p1->getRemainingCpuTime() != p2->getRemainingCpuTime()){
		return p1->getRemainingCpuTime() < p2->getRemainingCpuTime();
	}
	else{// FCFS on a tie
		return p1->getRQArrivalTime() > p2->getRQArrivalTime();
	}
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
