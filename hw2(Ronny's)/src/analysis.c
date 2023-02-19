#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"
#include "process_scheduling.c"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"
#define SRTF "SRTF"

//==================================================
//Author: Ronny Im, Matthew Bergquist, Alisha Crow
//Date: 02/19/2023
//Class: CIS 520
//Semester: Spring 2023
//Assignment: Project 2
//==================================================

/*
* main - the main function for the program
* @argc: the number of command line arguments
* @argv: the command line arguments
*
* Returns: EXIT_SUCCESS if the program runs successfully, EXIT_FAILURE otherwise
*/
int main(int argc, char **argv) 
{
    if (argc < 3) // checks if the user entered the correct number of arguments
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]); // prints the correct usage of the program
        
        return EXIT_FAILURE; // exits the program
    }


    //PERFORMS A FIRST COME FIRST CPU SCHEDULING
    if(strcmp(argv[2], "FCFS") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]); // loads the process control blocks from the file
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t)); // creates a ScheduleResult_t struct
        first_come_first_serve(d, r); // performs the first come first serve scheduling
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time); // prints the average waiting time
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time); // prints the average turnaround time
        printf("Total Run Time: %.2f\n", (float)r->total_run_time); // prints the total run time
        
    }
    
    
    //PERFORMS A SHORTEST JOB FIRST CPU SCHEDULING
    else if(strcmp(argv[2], "SJF") == 0){ 
        
        dyn_array_t *d = load_process_control_blocks(argv[1]); // loads the process control blocks from the file
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t)); // creates a ScheduleResult_t struct
        shortest_job_first(d, r); // performs the shortest job first scheduling

        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time); // prints the average waiting time
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time); // prints the average turnaround time
        printf("Total Run Time: %.2f\n", (float)r->total_run_time); // prints the total run time
        
    }
    
    
    //PERFORMS A PRIORITY CPU SCHEDULING
    else if(strcmp(argv[2], "P") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]); // loads the process control blocks from the file
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t)); // creates a ScheduleResult_t struct
        priority(d, r); // performs the priority scheduling
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time); // prints the average waiting time
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time); // prints the average turnaround time
        printf("Total Run Time: %.2f\n", (float)r->total_run_time); // prints the total run time
        
    }
    
    
    //PERFORMS A PRIORITY CPU SCHEDULING
    else if(strcmp(argv[2], "RR") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]); // loads the process control blocks from the file
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t)); // creates a ScheduleResult_t struct
        round_robin(d, r, atoi(argv[3])); // performs the round robin scheduling
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time); // prints the average waiting time
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time); // prints the average turnaround time
        printf("Total Run Time: %.2f\n", (float)r->total_run_time); // prints the total run time
        
    }
    
    
    //PERFORMS A SHORTEST REMAINING TIME FIRST CPU SCHEDULING
    else if(strcmp(argv[2], "SRTF") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]); // loads the process control blocks from the file
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t)); // creates a ScheduleResult_t struct
        shortest_remaining_time_first(d, r); // performs the shortest remaining time first scheduling
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time); // prints the average waiting time
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);   // prints the average turnaround time
        printf("Total Run Time: %.2f\n", (float)r->total_run_time); // prints the total run time
        
    }
    
    

    return EXIT_SUCCESS; // exits the program
}
