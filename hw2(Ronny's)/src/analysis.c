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

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        
        return EXIT_FAILURE;
    }


    //PERFORMS A FIRST COME FIRST CPU SCHEDULING
    if(strcmp(argv[2], "FCFS") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]);
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        first_come_first_serve(d, r);
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time);
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);
        printf("Total Run Time: %.2f\n", (float)r->total_run_time);
        
    }
    
    
    //PERFORMS A SHORTEST JOB FIRST CPU SCHEDULING
    else if(strcmp(argv[2], "SJF") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]);
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        shortest_job_first(d, r);
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time);
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);
        printf("Total Run Time: %.2f\n", (float)r->total_run_time);
        
    }
    
    
    //PERFORMS A PRIORITY CPU SCHEDULING
    else if(strcmp(argv[2], "P") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]);
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        priority(d, r);
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time);
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);
        printf("Total Run Time: %.2f\n", (float)r->total_run_time);
        
    }
    
    
    //PERFORMS A PRIORITY CPU SCHEDULING
    else if(strcmp(argv[2], "RR") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]);
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        round_robin(d, r, atoi(argv[3]));
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time);
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);
        printf("Total Run Time: %.2f\n", (float)r->total_run_time);
        
    }
    
    
    //PERFORMS A SHORTEST REMAINING TIME FIRST CPU SCHEDULING
    else if(strcmp(argv[2], "SRTF") == 0){
        
        dyn_array_t *d = load_process_control_blocks(argv[1]);
        ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
        shortest_remaining_time_first(d, r);
        
        
        printf("\nAverage Waiting Time: %.2f\n", (float)r->average_waiting_time);
        printf("Average Turnaround Time: %.2f\n", (float)r->average_turnaround_time);
        printf("Total Run Time: %.2f\n", (float)r->total_run_time);
        
    }
    
    

    return EXIT_SUCCESS;
}
