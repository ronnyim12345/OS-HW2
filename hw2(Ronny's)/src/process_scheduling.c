#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/types.h>


#include "dyn_array.h"
#include "dyn_array.c"
#include "processing_scheduling.h"

//==================================================
//Author: Ronny Im, Matthew Bergquist, Alisha Crow
//Date: 02/19/2023
//Class: CIS 520
//Semester: Spring 2023
//Assignment: Project 2
//==================================================

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}


/*
* first_come_first_serve
* Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
* @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
* @param result used for first come first served stat tracking \ref ScheduleResult_t
* @return true if function ran successful else false for an error
*/
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue; // DYNAMIC ARRAY
    int s = ready_queue->size; // SIZE OF DYNAMIC ARRAY
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1; // FIRST POINTER
    ProcessControlBlock_t *p2; // SECOND POINTER
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t)); // EXTRACTION HOLDER
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->arrival > p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS GREATER THAN SECOND POINTER'S ARRIVAL TIME
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        } 
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        waiting_time[i] = 0; // SET WAITING TIME TO ZERO
        burst_time[i] = 0; // SET BURST TIME TO ZERO
    }
    
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p; 
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        
        p = dyn_array_at(d, i); // TRAVELING POINTER
        
        waiting_time[i] = universal_timer; // SET WAITING TIME TO UNIVERSAL TIMER
        burst_timer = 0; // SET BURST TIMER TO ZERO
        
        while(p->remaining_burst_time != 0){ // WHILE BURST TIME IS NOT ZERO
            virtual_cpu(p); // PERFORMS CPU SCHEDULING
            universal_timer++; // INCREASE UNIVERSAL TIMER
            burst_timer++; // INCREASE BURST TIMER
        }
        
        burst_time[i] = burst_timer; // SET BURST TIME TO BURST TIMER
    }
    
    
    //SUBTRACTS ARRIVAL TIMES FROM WAITING TIMES
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER
        waiting_time[i] -= p1->arrival; // SUBTRACTS ARRIVAL TIME FROM WAITING TIME
    }
    
    
    //CREATE AVERAGE WAITING TIME VARIABLE
    float average_waiting_time = 0; 
   
   
    //CALCULATE AVERAGE WAITING TIME
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        average_waiting_time += waiting_time[i]; // ADDS WAITING TIME TO AVERAGE WAITING TIME
    }
    
    
    //CALCULATE AVERAGE WAITING TIME
    average_waiting_time /= s; // DIVIDES AVERAGE WAITING TIME BY SIZE OF DYNAMIC ARRAY
    
    
    //CREATE AVERAGE TURNAROUND TIME VARIABLE
    float average_turnaround_time = 0;
    
    
    //CALCULATE AVERAGE TURNAROUND TIME
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        average_turnaround_time += waiting_time[i]+burst_time[i]; // ADDS WAITING TIME AND BURST TIME TO AVERAGE TURNAROUND TIME
    }
    
    //CALCULATE AVERAGE TURNAROUND TIME
    average_turnaround_time /= s; // DIVIDES AVERAGE TURNAROUND TIME BY SIZE OF DYNAMIC ARRAY
    
    
    //STORE ALL THE RESULTS
    result->average_waiting_time = average_waiting_time;   
    result->average_turnaround_time = average_turnaround_time;  
    result->total_run_time = universal_timer; 
    
    
    free(ex); // FREE EXTRACTION HOLDER
    
    
    return true; // RETURN TRUE
}

/*
* shortest_job_first
* Runs the Shortest Job First Process Scheduling algorithm over the incoming ready_queue
* @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
* @param result used for shortest job first stat tracking \ref ScheduleResult_t
* @return true if function ran successful else false for an error
*/
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue; // DYNAMIC ARRAY
    int s = ready_queue->size; // SIZE OF DYNAMIC ARRAY
    int s_initial = ready_queue->size; // SIZE OF DYNAMIC ARRAY
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1; // FIRST POINTER
    ProcessControlBlock_t *p2; // SECOND POINTER
     
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p; 
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t)); // EXTRACTION HOLDER
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME, THEN BURST
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->arrival > p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS GREATER THAN SECOND POINTER'S ARRIVAL TIME
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        }
        else if(p1->arrival == p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS EQUAL TO SECOND POINTER'S ARRIVAL TIME
            
            if(p1->remaining_burst_time > p2->remaining_burst_time){ // IF FIRST POINTER'S BURST TIME IS GREATER THAN SECOND POINTER'S BURST TIME
                dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
                dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
                
                i = -1; // RESET LOOP
            }
            
        }
    }
    
    
    //SETS UP ALL ID NUMBERS
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p = dyn_array_at(d, i); // TRAVELING POINTER
        p->ID = i; // SET ID NUMBER
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0; // UNIVERSAL TIMER
    float waiting_time[s]; // WAITING TIME
    float burst_time[s]; // BURST TIME
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        waiting_time[i] = 0; // SET WAITING TIME TO ZERO
        burst_time[i] = 0; // SET BURST TIME TO ZERO
    }
    
    
    //SUBTRACTS ARRIVAL TIMES FROM WAITING TIMES
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER
        waiting_time[i] -= p1->arrival; // SUBTRACTS ARRIVAL TIME FROM WAITING TIME
    }
    
    
    //CPU SCHEDULES THE FIRST ONE (HAPPENS REGARDLESS)
    p = dyn_array_at(d, 0);
    
    while(p->remaining_burst_time != 0){ // LOOP UNTIL BURST TIME IS ZERO
        virtual_cpu(p); // PERFORMS VIRTUAL CPU
        universal_timer++; // INCREASES UNIVERSAL TIMER
        burst_time[p->ID]++; // INCREASES BURST TIME
    }
    
    
    //REMOVES IT FROM THE DYN ARRAY
    dyn_array_extract(d, 0, ex); // EXTRACTS FIRST POINTER
    
    
    //STORE THE NEW SIZE
    s = ready_queue->size; // STORES NEW SIZE OF DYNAMIC ARRAY
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF BURST
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->remaining_burst_time > p2->remaining_burst_time){ // IF FIRST POINTER'S BURST TIME IS GREATER THAN SECOND POINTER'S BURST TIME
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        }
        
    }

    
    //CPU SCHEDULES THE REST OF THE DATA
    while(s != 0){ // LOOP UNTIL SIZE IS ZERO
        
        //PERFORMS THE NEXT CPU SCHEDULING BY BURST
        for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
            p = dyn_array_at(d, i); // TRAVELING POINTER
            
            if(p->arrival <= universal_timer){ // IF ARRIVAL TIME IS LESS THAN OR EQUAL TO UNIVERSAL TIMER
                
                waiting_time[p->ID] += universal_timer; // ADDS UNIVERSAL TIMER TO WAITING TIME
                
                while(p->remaining_burst_time != 0){ // LOOP UNTIL BURST TIME IS ZERO
                    virtual_cpu(p);  // PERFORMS VIRTUAL CPU
                    universal_timer++; // INCREASES UNIVERSAL TIMER
                    burst_time[p->ID]++; // INCREASES BURST TIME
                }
                
                dyn_array_extract(d, i, ex); // EXTRACTS TRAVELING POINTER
                s = ready_queue->size;        //store the new size
                
                i = s; // SETS LOOP TO END
            } 
            
        }
    
    }
    
    
    //CREATE AVERAGE WAITING TIME VARIABLE
    float average_waiting_time = 0;
   
   
    //CALCULATE AVERAGE WAITING TIME
    for(int i=0; i < s_initial; i++){ // LOOP THROUGH ALL DATA
        average_waiting_time += waiting_time[i]; // ADDS WAITING TIME TO AVERAGE WAITING TIME
    }
    
    
    //CALCULATE AVERAGE WAITING TIME
    average_waiting_time /= s_initial; // DIVIDES AVERAGE WAITING TIME BY SIZE OF DYNAMIC ARRAY
    
    
    //CREATE AVERAGE TURNAROUND TIME VARIABLE
    float average_turnaround_time = 0; 
    
    
    //CALCULATE AVERAGE TURNAROUND TIME
    for(int i=0; i < s_initial; i++){ // LOOP THROUGH ALL DATA
        average_turnaround_time += waiting_time[i]+burst_time[i]; // ADDS WAITING TIME AND BURST TIME TO AVERAGE TURNAROUND TIME
    }
    
    //CALCULATE AVERAGE TURNAROUND TIME
    average_turnaround_time /= s_initial;
    
    
    //STORE ALL THE RESULTS
    result->average_waiting_time = average_waiting_time;     
    result->average_turnaround_time = average_turnaround_time;  
    result->total_run_time = universal_timer; 
    
    
    
    free(ex); // FREE EXTRACTION HOLDER
    
    
    return true; // RETURN TRUE
}

/*
* priority
* Runs the priority scheduling algorithm on the ready_queue.
* @param ready_queue The queue of processes to run.
* @param result The result of the scheduling algorithm.
* @return true if the scheduling algorithm was run successfully, false otherwise.
*/
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue; // DYNAMIC ARRAY
    int s = ready_queue->size; // SIZE OF DYNAMIC ARRAY
    int s_initial = ready_queue->size; // SIZE OF DYNAMIC ARRAY
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1; // FIRST POINTER
    ProcessControlBlock_t *p2; // SECOND POINTER
    
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p; 
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t)); 
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME, THEN PRIORITY
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->arrival > p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS GREATER THAN SECOND POINTER'S ARRIVAL TIME
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        } 
        else if(p1->arrival == p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS EQUAL TO SECOND POINTER'S ARRIVAL TIME
            
            if(p1->priority > p2->priority){ // IF FIRST POINTER'S PRIORITY IS GREATER THAN SECOND POINTER'S PRIORITY
                dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
                dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
                
                i = -1; // RESET LOOP
            }
            
        }
    }
    
    
    //SETS UP ALL ID NUMBERS
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p = dyn_array_at(d, i); // TRAVELING POINTER
        p->ID = i; // SETS ID NUMBER
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0; 
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        waiting_time[i] = 0; // SETS WAITING TIME TO ZERO
        burst_time[i] = 0; // SETS BURST TIME TO ZERO
    }
    
    
    //SUBTRACTS ARRIVAL TIMES FROM WAITING TIMES
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER
        waiting_time[i] -= p1->arrival; // SUBTRACTS ARRIVAL TIME FROM WAITING TIME
    }
    
    
    //CPU SCHEDULES THE FIRST ONE (HAPPENS REGARDLESS)
    p = dyn_array_at(d, 0);
    
    while(p->remaining_burst_time != 0){ // LOOP UNTIL BURST TIME IS ZERO
        virtual_cpu(p); // PERFORMS VIRTUAL CPU
        universal_timer++; // INCREASES UNIVERSAL TIMER
        burst_time[p->ID]++; // INCREASES BURST TIME
    }
    
    
    //REMOVES IT FROM THE DYN ARRAY
    dyn_array_extract(d, 0, ex);
    
    
    //STORE THE NEW SIZE
    s = ready_queue->size;
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF PRIORITY
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->priority > p2->priority){ // IF FIRST POINTER'S PRIORITY IS GREATER THAN SECOND POINTER'S PRIORITY
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        }
        
    }

    
    //CPU SCHEDULES THE REST OF THE DATA
    while(s != 0){ // LOOP UNTIL SIZE IS ZERO
        
        //PERFORMS THE NEXT CPU SCHEDULING BY PRIORITY
        for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
            p = dyn_array_at(d, i); // TRAVELING POINTER
            
            if(p->arrival <= universal_timer){ // IF ARRIVAL TIME IS LESS THAN OR EQUAL TO UNIVERSAL TIMER
                
                waiting_time[p->ID] += universal_timer; // ADDS UNIVERSAL TIMER TO WAITING TIME
                 
                while(p->remaining_burst_time != 0){ // LOOP UNTIL BURST TIME IS ZERO
                    virtual_cpu(p); // PERFORMS VIRTUAL CPU
                    universal_timer++; // INCREASES UNIVERSAL TIMER
                    burst_time[p->ID]++; // INCREASES BURST TIME
                }
                
                dyn_array_extract(d, i, ex); // EXTRACTS TRAVELING POINTER
                s = ready_queue->size;        //store the new size
                
                i = s; // SETS LOOP TO END
            }
            
        }
    
    }
    
    
    //CREATE AVERAGE WAITING TIME VARIABLE
    float average_waiting_time = 0;
   
   
    //CALCULATE AVERAGE WAITING TIME
    for(int i=0; i < s_initial; i++){ // LOOP THROUGH ALL DATA
        average_waiting_time += waiting_time[i]; // ADDS WAITING TIME TO AVERAGE WAITING TIME
    }
    
    
    //CALCULATE AVERAGE WAITING TIME
    average_waiting_time /= s_initial;
    
    
    //CREATE AVERAGE TURNAROUND TIME VARIABLE
    float average_turnaround_time = 0;
    
    
    //CALCULATE AVERAGE TURNAROUND TIME
    for(int i=0; i < s_initial; i++){ // LOOP THROUGH ALL DATA
        average_turnaround_time += waiting_time[i]+burst_time[i]; // ADDS WAITING TIME AND BURST TIME TO AVERAGE TURNAROUND TIME
    }
    
    //CALCULATE AVERAGE TURNAROUND TIME
    average_turnaround_time /= s_initial; 
    
    
    //STORE ALL THE RESULTS
    result->average_waiting_time = average_waiting_time;     
    result->average_turnaround_time = average_turnaround_time;  
    result->total_run_time = universal_timer; 
    
    
    free(ex); // FREE EXTRACTION HOLDER
    
    
    return true; // RETURN TRUE
}

/*
* round_robin
* Runs the round robin scheduling algorithm on the ready_queue
* @param ready_queue The queue of processes to run
* @param result The result of the scheduling algorithm
* @param quantum The quantum to use for the algorithm
* @return true if the algorithm was run successfully, false otherwise
*/
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL || quantum == 0){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue; // DYNAMIC ARRAY
    int s = ready_queue->size; // SIZE
    int q = quantum; // QUANTUM 
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1; 
    ProcessControlBlock_t *p2; 
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t)); 
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // FIRST POINTER
        p2 = dyn_array_at(d, i+1); // SECOND POINTER
        
        if(p1->arrival > p2->arrival){ // IF FIRST POINTER'S ARRIVAL TIME IS GREATER THAN SECOND POINTER'S ARRIVAL TIME
            dyn_array_extract(d, i+1, ex); // EXTRACT SECOND POINTER
            dyn_array_insert(d, i, ex); // INSERT SECOND POINTER BEFORE FIRST POINTER
            
            i = -1; // RESET LOOP
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        waiting_time[i] = 0; // SET WAITING TIME TO ZERO
        burst_time[i] = 0; // SET BURST TIME TO ZERO
    }
    
    
    //TRAVELLING POINTER
    ProcessControlBlock_t *p;
    
    
    //TRAVELLING AND COUNTER VARIABLES
    int t = 0;
    int counter = 0;
    
    
    //CPU SCHEDULES ALL OF THE DATA
    //this for loop walks through every pcb in the array one at a time
    //if all the pcb's "remaining burst" times are zero, the loop will stop
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        
        p = dyn_array_at(d, i); // TRAVELING POINTER
        
        if(p->remaining_burst_time == 0){ // IF BURST TIME IS ZERO
            t++; // INCREASES T
        }
        else{ // IF BURST TIME IS NOT ZERO
            
            t = 0;
            counter = 0;
            burst_timer = 0;
            
            while(counter < q && p->remaining_burst_time != 0){ // LOOP UNTIL COUNTER IS GREATER THAN QUANTUM OR BURST TIME IS ZERO
                virtual_cpu(p); // PERFORMS VIRTUAL CPU
                counter++; // INCREASES COUNTER
                universal_timer++; // INCREASES UNIVERSAL TIMER
                burst_timer++; // INCREASES BURST TIMER
            }
            
            for(int j=0; j<s; j++){ // LOOP THROUGH ALL DATA
                p2 = dyn_array_at(d, j); // TRAVELING POINTER
                
                if(j != i && p2->remaining_burst_time != 0){
                    waiting_time[j] += counter;     // this increments all of the 
                }                                   // pcb's in the ready queue
            }
            
            burst_time[i] += burst_timer; // INCREASES BURST TIME
            
        }
        
        
        if(i == s-1){  // this makes the for loop go forever
            i = -1; 
        } 
        
        
        if(t == s){    // this stops the loop
            i = s-1;
        }
        
    }
    
    
    //SUBTRACTS ARRIVAL TIMES FROM WAITING TIMES
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER
        waiting_time[i] -= p1->arrival; // SUBTRACTS ARRIVAL TIME FROM WAITING TIME
    }
    
    
    //CREATE AVERAGE WAITING TIME VARIABLE
    float average_waiting_time = 0;
   
   
    //CALCULATE AVERAGE WAITING TIME
    for(int i=0; i < s; i++){
        average_waiting_time += waiting_time[i];
    }
    
    
    //CALCULATE AVERAGE WAITING TIME
    average_waiting_time /= s;
    
    
    //CREATE AVERAGE TURNAROUND TIME VARIABLE
    float average_turnaround_time = 0;
    
    
    //CALCULATE AVERAGE TURNAROUND TIME
    for(int i=0; i < s; i++){
        average_turnaround_time += waiting_time[i]+burst_time[i];
    }
    
    //CALCULATE AVERAGE TURNAROUND TIME
    average_turnaround_time /= s;
    
    
    //STORE ALL THE RESULTS
    result->average_waiting_time = average_waiting_time;     
    result->average_turnaround_time = average_turnaround_time;  
    result->total_run_time = universal_timer; 
    
    
    free(ex); // FREE EXTRACTION HOLDER
     
    return true;  // RETURN TRUE
}

/*
* load_process_control_blocks
* Loads the process control blocks from the input file
* @param input_file The file to load the process control blocks from
* @return The array of process control blocks
*/
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
    //ERROR CHECKING
    if(input_file == NULL){
        return NULL;
    }
    
    
    //READS IN THE FILE
    FILE* fp = fopen(input_file, "r");
    
    
    //ERROR CHECKING
    if(fp == NULL){
        return NULL;
    }
    
    
    //SETS UP VARIABLE TO HOLD 1ST DATA (N)
    uint32_t *first_data = malloc(2*sizeof(uint32_t));
    
    
    //STORES FIRST DATA (N)
    fread(&first_data[0],sizeof(uint32_t),1,fp);
    
    
    //SETS UP DYN ARRAY
    dyn_array_t *d = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    
    
    //SETS UP DATA ARRAY
    uint32_t *data = malloc(first_data[0]*3*sizeof(uint32_t));
    
    
    //READ IN THE DATA
    for(int i=0; i<(int)first_data[0]*3; i++){
	    fread(&data[i],sizeof(uint32_t),1,fp);
	}
    
    
    //TRAVELING VARIABLE
    int t = 0;
    
    
    //SETS ALL DATA TO THE DYN ARRAY
    for(int i=0; i<(int)first_data[0]; i++){
        
        ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t)); // ALLOCATES MEMORY FOR PCB
    
        p->remaining_burst_time = data[t]; t++; // SETS THE REMAINING BURST TIME
        p->priority = data[t]; t++; // SETS THE PRIORITY
        p->arrival = data[t]; t++;  // SETS THE ARRIVAL TIME
        p->started = false; // SETS THE STARTED VARIABLE TO FALSE
        
        dyn_array_push_back(d, p); // PUSHES THE PCB TO THE DYN ARRAY
        
    }
    
    
    //CLOSE THE FILE
    fclose(fp);
    
    
    free(first_data); // FREE FIRST DATA
    free(data); // FREE DATA
    
    
    //RETURN THE DYN_ARRAY
    return d;
}
/*
* shortest_remaining_time_first
* Runs the shortest remaining time first scheduling algorithm
* @param ready_queue The array of process control blocks
* @param result The structure to store the results in
* @return true if the scheduling was successful, false otherwise
*/
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue; // DYN ARRAY
    int s = ready_queue->size; // SIZE OF DYN ARRAY
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1; // TRAVELING POINTER 1
    ProcessControlBlock_t *p2; // TRAVELING POINTER 2
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // TRAVELING POINTER 1
        p2 = dyn_array_at(d, i+1); // TRAVELING POINTER 2
        
        if(p1->arrival > p2->arrival){ // IF THE ARRIVAL TIME OF P1 IS GREATER THAN P2
            dyn_array_extract(d, i+1, ex); // EXTRACT P2
            dyn_array_insert(d, i, ex); // INSERT P2 BEFORE P1
            
            i = -1; // RESET THE LOOP
        }
        else if(p1->arrival == p2->arrival){ // IF THE ARRIVAL TIME OF P1 IS EQUAL TO P2
            
            if(p1->remaining_burst_time > p2->remaining_burst_time){ // IF THE REMAINING BURST TIME OF P1 IS GREATER THAN P2
                dyn_array_extract(d, i+1, ex); // EXTRACT P2
                dyn_array_insert(d, i, ex); // INSERT P2 BEFORE P1
                
                i = -1; // RESET THE LOOP
            }
            
        }
    }
    
    
    //SETS UP ALL ID NUMBERS
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER 1
        p1->ID = i; // SETS THE ID NUMBER
    }
    
    
    //FINDS THE LATEST ARRIVAL TIME
    p1 = dyn_array_at(d, s-1); // TRAVELING POINTER 1
    uint32_t latest_arrival = p1->arrival; // SETS THE LATEST ARRIVAL TIME
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0; // SETS THE BURST TIMER TO ZERO
    float waiting_time[s]; // SETS UP THE WAITING TIME ARRAY
    float burst_time[s]; // SETS UP THE BURST TIME ARRAY
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        waiting_time[i] = 0; // SETS ALL WAITING TIME TO ZERO
        burst_time[i] = 0; // SETS ALL BURST TIME TO ZERO
    }
    
    
    //CREATES AN ARRIVALS BOOLEAN ARRAY
    bool all_arrivals[latest_arrival];
    
    
    //SETS THEM ALL TO FALSE
    for(int i=0; i < (int)latest_arrival; i++){
        all_arrivals[i] = false;
    }
    
    
    //MARKS WHEN EACH PCB ARRIVES IN THE ARRAY
    for(int i=0; i < s-1; i++){
        p1 = dyn_array_at(d, i);
        all_arrivals[p1->arrival] = true;
    }
    
    
    //SETS UP FOR THE FIRST ONE
    p1 = dyn_array_at(d, 0);
    
    
    //THIS PROCESSES ALL OF THE DATA UNTIL THE LAST DATA ARRIVES
    for(int i=0; i < (int)latest_arrival; i++){
        
        //CPU PROCESSES P1
        virtual_cpu(p1); 
        universal_timer++; 
        burst_time[p1->ID]++; 
        
        
        //INCREMENTS WAIT TIME FOR THE REST OF THEM
        for(int j=0; j<s; j++){ 
            p2 = dyn_array_at(d, j); 
            
            if(j != i){
                waiting_time[j]++;
            }
        }
        
        
        //DOES THIS IF A NEW DATA ARRIVES & SWITCHES TO THE NEW DATA IF IT IS SHORTER)
        if(all_arrivals[i+1] == true){
            
            p2 = dyn_array_at(d, (p1->ID)+1);
            
            while((int)p2->arrival != i+1){
                p2 = dyn_array_at(d, (p2->ID)+1);
                
            }
            
            if(p2->remaining_burst_time < p1->remaining_burst_time){
                p1 = p2;
            }
        }
    }
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF BURST TIME
    for(int i=0; i < s-1; i++){ // LOOP THROUGH ALL DATA
        
        p1 = dyn_array_at(d, i); // TRAVELING POINTER 1 
        p2 = dyn_array_at(d, i+1); // TRAVELING POINTER 2
        
        if(p1->remaining_burst_time > p2->remaining_burst_time){ // IF THE REMAINING BURST TIME OF P1 IS GREATER THAN P2
            dyn_array_extract(d, i+1, ex); // EXTRACT P2
            dyn_array_insert(d, i, ex); // INSERT P2 BEFORE P1
            
            i = -1; // RESET THE LOOP
        }
    }
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    //this picks up where the last function stopped
    //this performs scheduling on the rest of the data SJF
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
         
        p1 = dyn_array_at(d, i); // TRAVELING POINTER 1
        
        burst_timer = 0; // SETS THE BURST TIMER TO ZERO
        
        while(p1->remaining_burst_time != 0){ // WHILE THE REMAINING BURST TIME IS NOT ZERO
            virtual_cpu(p1); // RUNS THE VIRTUAL CPU
            universal_timer++; // INCREMENTS THE UNIVERSAL TIMER
            burst_timer++; // INCREMENTS THE BURST TIMER
        }
        
        
        for(int j=0; j<s; j++){ // LOOP THROUGH ALL DATA
            p2 = dyn_array_at(d, j); // TRAVELING POINTER 2
            
            if(j != i && p2->remaining_burst_time != 0){ // IF THE ID OF P2 IS NOT THE SAME AS P1 AND THE REMAINING BURST TIME IS NOT ZERO
                waiting_time[j] += burst_timer;     // this increments all of the 
            }                                   // pcb's in the ready queue
        }
        
        burst_time[i] += burst_timer; // INCREMENTS THE BURST TIME
    }
    
    
    //SUBTRACTS ARRIVAL TIMES FROM WAITING TIMES
    for(int i=0; i < s; i++){ // LOOP THROUGH ALL DATA
        p1 = dyn_array_at(d, i); // TRAVELING POINTER
        waiting_time[i] -= p1->arrival; // SUBTRACTS ARRIVAL TIME FROM WAITING TIME
    }
    
    
    //CREATE AVERAGE WAITING TIME VARIABLE
    float average_waiting_time = 0;
   
   
    //CALCULATE AVERAGE WAITING TIME
    for(int i=0; i < s; i++){
        average_waiting_time += waiting_time[i]; // ADDS ALL THE WAITING TIME
    }
    
    
    //CALCULATE AVERAGE WAITING TIME
    average_waiting_time /= s;
    
    
    //CREATE AVERAGE TURNAROUND TIME VARIABLE
    float average_turnaround_time = 0;
    
    
    //CALCULATE AVERAGE TURNAROUND TIME
    for(int i=0; i < s; i++){
        average_turnaround_time += waiting_time[i]+burst_time[i]; // ADDS THE WAITING TIME AND BURST TIME
    }
    
    //CALCULATE AVERAGE TURNAROUND TIME
    average_turnaround_time /= s;
    
    
    //STORE ALL THE RESULTS
    result->average_waiting_time = average_waiting_time;     
    result->average_turnaround_time = average_turnaround_time;  
    result->total_run_time = universal_timer; 
    
    
    free(ex); // FREE THE EXTRACTED DATA
    
    
    return true; // RETURN TRUE
}
