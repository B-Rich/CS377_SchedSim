#include "Headers.h"

bool Device::hasProcessAndEvent()
{
	return _process != NULL && _event != NULL;
}

Process* Device::getProcess()
{
	return _process;
}

/*The event stored on the device.
 *(Storing the event is necessary because the IO and CPU
 *	devices hold IO and CPU 'done' events, respectively.)
 */
Event* Device::getEvent()
{
	return _event;
}

void Device::setProcessAndEvent(Process* process, Event* event)
{
	_process = process;
	_event = event;
}
