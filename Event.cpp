#include "Headers.h"

Event::Event(Type type, double time)
:_type(type),_time(time)
{
}

Type Event::getType()
{
	return _type;
}

void Event::setType(Type type)
{
	_type = type;
}

double Event::getTime() //"time" refers to the absolute time at which the event is scheduled for
{
	return _time;
}

bool CompareEvent::operator()(Event* e1, Event* e2) 
{
	return e1->getTime() > e2->getTime();
}
