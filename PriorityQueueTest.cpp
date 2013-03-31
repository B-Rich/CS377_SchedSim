#include <iostream>
#include <queue>
#include <vector>

class Thing
{
private:
	int _num;
public:
	Thing(int num)
	:	_num(num)
	{
	}
	int getNum() const
	{
		return _num;
	}
	void setNum(int num)
	{
		_num = num;
	}
};

//note: "similar to a heap where only the max heap
//	element can be retrieved"
class CompareThingPointer
{
public:
	bool operator()(const Thing* t1, const Thing* t2) const
	{
		return t1->getNum() > t2->getNum();
	}
};

int main(int argc, char** argv)
{
	std::priority_queue<Thing*, std::vector<Thing*>, CompareThingPointer> pq, qp;
	Thing* things[5];
	Thing* t;

	things[0] = new Thing(3);
	things[1] = new Thing(7);
	things[2] = new Thing(1);
	things[3] = new Thing(2);
	things[4] = new Thing(5);

	for(int i = 0; i < 5; i++)
	{
		pq.push(things[i]);
	}

	for(int i = 0; i < 5; i++)
	{
		t = pq.top();
		pq.pop();
		std::cout << t->getNum() << std::endl;
		t->setNum(5 - i);
		qp.push(t);
		t->setNum(20 + i);
		pq.push(t);
	}

	for(int i = 0; i < 5; i++)
	{
		t = qp.top();
		qp.pop();
		std::cout << t->getNum() << std::endl;
	}

	for(int i = 0; i < 5; i++)
	{
		t = pq.top();
		pq.pop();
		std::cout << t->getNum() << std::endl;
	}

	for(int i = 0; i < 5; i++)
	{
		delete things[i];
	}

	return 0;
}
