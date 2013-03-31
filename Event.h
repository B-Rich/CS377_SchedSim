enum Type
{
	ARRIVAL,
	CPU_DONE,
	IO_DONE,
	CANCELLED
};

class Event
{
private:
	Type _type;
	double _time;
public:
	Event(Type type, double time);
	Type getType();
	void setType(Type type);
	double getTime();
};

class CompareEvent
{
public:
	bool operator()(Event* e1, Event* e2);
};
