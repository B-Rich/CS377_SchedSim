CXX = g++
#CXXFLAGS = -g -Wall
CXXFLAGS = -ggdb -Wall

all:	SchedSim

SchedSim:		Main.o SchedSim.o Event.o Device.o ProcessData.o Process.o CompareProcessPointer.o
	$(CXX) $(CXXFLAGS) Main.o SchedSim.o Event.o Device.o ProcessData.o Process.o CompareProcessPointer.o -o $@

Main.o:	Main.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c Main.cpp -o $@

SchedSim.o:	SchedSim.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c SchedSim.cpp -o $@

Event.o:	Event.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c Event.cpp -o $@

Process.o:	Process.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c Process.cpp -o $@

Device.o:	Device.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c Device.cpp -o $@

ProcessData.o:	ProcessData.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c ProcessData.cpp -o $@

CompareProcessPointer.o:	CompareProcessPointer.cpp Headers.h CHeaders.h
	$(CXX) $(CXXFLAGS) -c CompareProcessPointer.cpp -o $@

Headers.h:	ProcessData.h Event.h ProcessData.h CompareProcessPointer.h Device.h SchedSim.h
	touch Headers.h

PriorityQueueTest:		PriorityQueueTest.cpp
	$(CXX) $(CXXFLAGS) PriorityQueueTest.cpp -o $@

#csapp.o:	csapp.c csapp.h
#	$(CXX) $(CXXFLAGS) -c csapp.c -o $@

clean:
	rm -fv *.o SchedSim
#	rm -fv *.o SchedSim src.zip

zip:
	echo "***Deleting***" &&\
	rm -Rfv src.zip src &&\
	echo "***Creating src directory***" &&\
	mkdir -v src &&\
	cp -vt src *.cpp *.h Makefile pwd pwd.new01 pwd.new02\
		uniform frontload increasing &&\
	echo "***Creating zip***" &&\
	zip -r src.zip src && echo "***Testing***" &&\
	zip -Tv src.zip
