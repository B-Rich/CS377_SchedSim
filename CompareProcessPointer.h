#include "CHeaders.h"

class CompareProcessPointer
{
private:
	bool (*_comparator)(Process* p1, Process* p2);
	static bool compareFCFS(Process* p1, Process* p2);
	static bool compareSJF(Process* p1, Process* p2);
	static bool compareSRTF(Process* p1, Process* p2);
public:
	CompareProcessPointer(Algorithm algorithm);
	bool operator()(Process* p1, Process* p2);
};
