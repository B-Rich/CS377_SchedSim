class Device
{
private:
	Process* _process;
	Event* _event;
public:
	bool hasProcessAndEvent();
	Process* getProcess();
	Event* getEvent();
	void setProcessAndEvent(Process* process, Event* event);
};
