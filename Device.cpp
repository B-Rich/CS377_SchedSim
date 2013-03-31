#include "Headers.h"

bool Device::hasProcessAndEvent()
{
	return _process != NULL && _event != NULL;
}

Process* Device::getProcess()
{
	return _process;
}

Event* Device::getEvent()
{
	return _event;
}

void Device::setProcessAndEvent(Process* process, Event* event)
{
	_process = process;
	_event = event;
}
