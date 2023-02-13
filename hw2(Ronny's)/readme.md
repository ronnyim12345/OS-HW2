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

Average Waiting Time: 17.50
Average Turnaround Time: 30.00
Total Run Time: 50.00

real    0m0.008s
user    0m0.001s
sys     0m0.006s
[ronnyim12345@selene build]$ time ./analysis pcb.bin SJF

Average Waiting Time: 12.50
Average Turnaround Time: 25.00
Total Run Time: 50.00

real    0m0.021s
user    0m0.002s
sys     0m0.007s
[ronnyim12345@selene build]$ time ./analysis pcb.bin P

Average Waiting Time: 17.50
Average Turnaround Time: 30.00
Total Run Time: 50.00

real    0m0.006s
user    0m0.002s
sys     0m0.004s
[ronnyim12345@selene build]$ time ./analysis pcb.bin RR 5

Average Waiting Time: 21.25
Average Turnaround Time: 33.75
Total Run Time: 50.00

real    0m0.006s
user    0m0.003s
sys     0m0.003s
[ronnyim12345@selene build]$ time ./analysis pcb.bin SRTF

Average Waiting Time: 13.25
Average Turnaround Time: 25.75
Total Run Time: 50.00

real    0m0.006s
user    0m0.001s
sys     0m0.005s

