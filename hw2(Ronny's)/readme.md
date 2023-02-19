# Assignment 2

Milestone 0: CMake and Unit Testing filled out. 

Milestone 1: PCB file loading and First Come First Served. 

Milestone 2: Shortest Job First, Shortest Remaining Time First, Round Robin, and analysis of algorithms. 

Note: 
You can manually copy the time analysis from console and paste it to this readme file, but directly output from your program is strongly recommended.     
---------------------------------------------------------------------------
Add your scheduling algorithm analysis below this line in a readable format. 
---------------------------------------------------------------------------

[ronnyim12345@selene build]$ time ./analysis pcb.bin FCFS

Average Waiting Time: 16.00
Average Turnaround Time: 28.50
Total Run Time: 50.00

real    0m0.007s
user    0m0.001s
sys     0m0.006s
[ronnyim12345@selene build]$ time ./analysis pcb.bin SJF

Average Waiting Time: 14.75
Average Turnaround Time: 27.25
Total Run Time: 50.00

real    0m0.023s
user    0m0.001s
sys     0m0.008s
[ronnyim12345@selene build]$ time ./analysis pcb.bin P

Average Waiting Time: 16.00
Average Turnaround Time: 28.50
Total Run Time: 50.00

real    0m0.006s
user    0m0.001s
sys     0m0.004s
[ronnyim12345@selene build]$ time ./analysis pcb.bin RR 5

Average Waiting Time: 19.75
Average Turnaround Time: 32.25
Total Run Time: 50.00

real    0m0.006s
user    0m0.001s
sys     0m0.005s
[ronnyim12345@selene build]$ time ./analysis pcb.bin SRTF

Average Waiting Time: 11.75
Average Turnaround Time: 24.25
Total Run Time: 50.00

real    0m0.024s
user    0m0.000s
sys     0m0.009s


