Here are three sample binaries to use with SchedSim.
These inputs assume that maxProcesses and maxCPUbursts are set high enough to accomidate everything.
Don't forget to test your program on arbitrary large binaries as well!

Here is how they should look in memory:

uniform -

Process # 1
time of next arrival: 3.0
number of CPU bursts: 8
size of CPU bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]
size of I/O bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]
Process # 2
time of next arrival: 3.0
number of CPU bursts: 8
size of CPU bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]
size of I/O bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]
Process # 3
time of next arrival: 3.0
number of CPU bursts: 8
size of CPU bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]
size of I/O bursts: [2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625, 2.2265625]

frontload -

Process # 1
time of next arrival: 1.0
number of CPU bursts: 4
size of CPU bursts: [7.96875, 7.96875, 7.96875, 7.96875]
size of I/O bursts: [0.0390625, 0.0390625, 0.0390625]
Process # 2
time of next arrival: 1.0
number of CPU bursts: 8
size of CPU bursts: [0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375]
size of I/O bursts: [0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375]
Process # 3
time of next arrival: 1.0
number of CPU bursts: 8
size of CPU bursts: [0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375]
size of I/O bursts: [0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375, 0.234375]

increasing -

Process # 1
time of next arrival: 4.0
number of CPU bursts: 4
size of CPU bursts: [0.9765625, 0.9765625, 1.9921875, 3.984375]
size of I/O bursts: [0.9765625, 1.9921875, 3.984375]
Process # 2
time of next arrival: 8.0
number of CPU bursts: 4
size of CPU bursts: [1.9921875, 1.9921875, 3.984375, 7.96875]
size of I/O bursts: [1.9921875, 3.984375, 7.96875]
Process # 3
time of next arrival: 8.0
number of CPU bursts: 4
size of CPU bursts: [3.984375, 3.984375, 7.96875, 9.8828125]
size of I/O bursts: [3.984375, 7.96875, 9.8828125]

