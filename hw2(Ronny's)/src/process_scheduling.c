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



// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}



bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue;
    int s = ready_queue->size;
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1 = malloc(sizeof(ProcessControlBlock_t));
    ProcessControlBlock_t *p2 = malloc(sizeof(ProcessControlBlock_t));
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->arrival > p2->arrival){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = -1;
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){
        waiting_time[i] = 0;
        burst_time[i] = 0;
    }
    
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t));
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    for(int i=0; i < s; i++){
        
        p = dyn_array_at(d, i);
        
        waiting_time[i] = universal_timer;
        burst_timer = 0;
        
        while(p->remaining_burst_time != 0){
            virtual_cpu(p);
            universal_timer++;
            burst_timer++;
        }
        
        burst_time[i] = burst_timer;
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
    
    
    return true;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue;
    int s = ready_queue->size;
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1 = malloc(sizeof(ProcessControlBlock_t));
    ProcessControlBlock_t *p2 = malloc(sizeof(ProcessControlBlock_t));
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF BURST TIME
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->remaining_burst_time > p2->remaining_burst_time){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = -1;
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){
        waiting_time[i] = 0;
        burst_time[i] = 0;
    }
    
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t));
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    for(int i=0; i < s; i++){
        
        p = dyn_array_at(d, i);
        
        waiting_time[i] = universal_timer;
        burst_timer = 0;
        
        while(p->remaining_burst_time != 0){
            virtual_cpu(p);
            universal_timer++;
            burst_timer++;
        }
        
        burst_time[i] = burst_timer;
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
    
    
    return true;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue;
    int s = ready_queue->size;
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1 = malloc(sizeof(ProcessControlBlock_t));
    ProcessControlBlock_t *p2 = malloc(sizeof(ProcessControlBlock_t));
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF PRIORITY
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->priority > p2->priority){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = -1;
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){
        waiting_time[i] = 0;
        burst_time[i] = 0;
    }
    
    
    //TRAVELING POINTER
    ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t));
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    for(int i=0; i < s; i++){
        
        p = dyn_array_at(d, i);
        
        waiting_time[i] = universal_timer;
        burst_timer = 0;
        
        while(p->remaining_burst_time != 0){
            virtual_cpu(p);
            universal_timer++;
            burst_timer++;
        }
        
        burst_time[i] = burst_timer;
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
    
    
    return true; 
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL || quantum == 0){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue;
    int s = ready_queue->size;
    int q = quantum;
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1 = malloc(sizeof(ProcessControlBlock_t));
    ProcessControlBlock_t *p2 = malloc(sizeof(ProcessControlBlock_t));
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->arrival > p2->arrival){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = -1;
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){
        waiting_time[i] = 0;
        burst_time[i] = 0;
    }
    
    
    //TRAVELLING POINTER
    ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t));
    
    
    //TRAVELLING AND COUNTER VARIABLES
    int t = 0;
    int counter = 0;
    
    
    //CPU SCHEDULES ALL OF THE DATA
    //this for loop walks through every pcb in the array one at a time
    //if all the pcb's "remaining burst" times are zero, the loop will stop
    for(int i=0; i < s; i++){
        
        p = dyn_array_at(d, i);
        
        if(p->remaining_burst_time == 0){
            t++;
        }
        else{
            
            t = 0;
            counter = 0;
            burst_timer = 0;
            
            while(counter < q && p->remaining_burst_time != 0){
                virtual_cpu(p);
                counter++;
                universal_timer++;
                burst_timer++;
            }
            
            for(int j=0; j<s; j++){
                p2 = dyn_array_at(d, j);
                
                if(j != i && p2->remaining_burst_time != 0){
                    waiting_time[j] += counter;     // this increments all of the 
                }                                   // pcb's in the ready queue
            }
            
            burst_time[i] += burst_timer;
            
        }
        
        
        if(i == s-1){  // this makes the for loop go forever
            i = -1;
        }
        
        
        if(t == s){    // this stops the loop
            i = s;
        }
        
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
    
    
    return true; 
}

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
        
        ProcessControlBlock_t *p = malloc(sizeof(ProcessControlBlock_t));
    
        p->remaining_burst_time = data[t]; t++;
        p->priority = data[t]; t++;
        p->arrival = data[t]; t++;
        p->started = false;
        
        dyn_array_push_back(d, p);
        
    }
    
    
    //CLOSE THE FILE
    fclose(fp);
    
    
    //RETURN THE DYN_ARRAY
    return d;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //ERROR CHECKING
    if(ready_queue == NULL || result == NULL){
        return false;
    }
    
    //MAKE CUSTOM VARIABLES
    dyn_array_t *d = ready_queue;
    int s = ready_queue->size;
    
    
    //TRAVELING POINTERS
    ProcessControlBlock_t *p1 = malloc(sizeof(ProcessControlBlock_t));
    ProcessControlBlock_t *p2 = malloc(sizeof(ProcessControlBlock_t));
    
    
    //EXTRACTION HOLDER
    ProcessControlBlock_t *ex = malloc(sizeof(ProcessControlBlock_t));
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF ARRIVAL TIME
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->arrival > p2->arrival){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = 0;
        }
    }
    
    
    //TIMER VARIABLES
    float universal_timer = 0;
    float burst_timer = 0;
    float waiting_time[s];
    float burst_time[s];
    
    
    //SET ALL EQUAL TO ZERO
    for(int i=0; i < s; i++){
        waiting_time[i] = 0;
        burst_time[i] = 0;
    }
    
    
    //PROCESSING THE FIRST DATA (HAPPENS REGARDLESS)
    p1 = dyn_array_at(d, 0);
    virtual_cpu(p1);
    universal_timer++;
    burst_time[0]++;
    
    for(int i=1; i < s; i++){
        waiting_time[i]++;     
    }   
    
    
    //THIS PROCESSES THE DATA AS IT COMES IN
    //this only covers the period of time when data comes in
    //and stops when the data stops coming in
    for(int i=1; i<s; i++){
        
        p2 = dyn_array_at(d, i);
        
        if(p1->remaining_burst_time > p2->remaining_burst_time){
            
            virtual_cpu(p2);
            universal_timer++;
            burst_time[i]++;
            
            for(int j=0; j < s; j++){
                
                if(j != (int)p2->arrival){
                    waiting_time[j]++;     
                }
                
            } 
            
            p1 = p2;
            
        }
        else{
            
            virtual_cpu(p1);
            universal_timer++;
            burst_time[p1->arrival]++;
            
            for(int j=0; j < s; j++){
                
                if(j != (int)p1->arrival){
                    waiting_time[j]++;     
                }
                
            } 
            
        }
            
    }
    
    
    //THIS SORTS ALL THE DATA IN ORDER OF BURST TIME
    for(int i=0; i < s-1; i++){
        
        p1 = dyn_array_at(d, i);
        p2 = dyn_array_at(d, i+1);
        
        if(p1->remaining_burst_time > p2->remaining_burst_time){
            dyn_array_extract(d, i+1, ex);
            dyn_array_insert(d, i, ex);
            
            i = -1;
        }
    }
    
    
    //PERFORMS CPU SCHEDULING FOR ALL DATA
    //this picks up where the last function stopped
    //this performs scheduling on the rest of the data SJF
    for(int i=0; i < s; i++){
        
        p1 = dyn_array_at(d, i);
        
        burst_timer = 0;
        
        while(p1->remaining_burst_time != 0){
            virtual_cpu(p1);
            universal_timer++;
            burst_timer++;
        }
        
        
        for(int j=0; j<s; j++){
            p2 = dyn_array_at(d, j);
            
            if(j != i && p2->remaining_burst_time != 0){
                waiting_time[j] += burst_timer;     // this increments all of the 
            }                                   // pcb's in the ready queue
        }
        
        burst_time[i] += burst_timer;
    }
    
    
    
    
     for(int i=0; i < s; i++){
         
         p1 = dyn_array_at(d, i);
         
         waiting_time[i] -= p1->arrival;
         
         
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
    
    
    return true;
}
