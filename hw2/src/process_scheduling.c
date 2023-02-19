#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
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

/*
* first_come_first_serve
*  Runs the First Come First Served Process Scheduling algorithm over the incoming ready_queue
*  @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
*  @param result used for first come first served stat tracking \ref ScheduleResult_t
*  @return true if function ran successful else false for an error
*/
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{

       // check for bad parameters
    if(ready_queue == NULL || result == NULL)
    {
        return false;
    }

    // initialize variables
    int i = 0;
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t *pcb = NULL;
    int turnaround_time = 0;
    int waiting_time = 0;
    int total_burst_time = 0;
    int burst_time = 0;

    // loop through the ready queue
    for(i = 0; i < size; i++)
    {
        // get the pcb
        pcb = dyn_array_at(ready_queue, i);

        // check for bad pcb
        if(pcb == NULL)
        {
            return false;
        }

        burst_time = pcb->remaining_burst_time;

        // run the virtual cpu
        virtual_cpu(pcb);

        // calculate the waiting time for each process
        if (i > 0)
        {
            waiting_time = waiting_time + (turnaround_time - pcb->arrival - burst_time);
        }

        // calculate the turnaround time for each process
        turnaround_time = turnaround_time + burst_time;
        total_burst_time = total_burst_time + burst_time;
    }

    // update the schedule result with the calculated values
    result->average_waiting_time = (float) waiting_time / size;
    result->average_turnaround_time = (float) turnaround_time / size;
    result->total_run_time = total_burst_time;

    return true;

}

/*
* shortest_job_first
* Runs the Shortest Job First Scheduling algorithm over the incoming ready_queue
* @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
* @param result used for shortest job first stat tracking \ref ScheduleResult_t
* @return true if function ran successful else false for an error
*/
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //check for bad params
    if (ready_queue == NULL || result == NULL)
    {
        return false;
    }

    // initialize variables
    int i = 0;
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t *pcb = NULL;
    int turnaround_time = 0;
    int waiting_time = 0;

    // loop through the ready queue
    for (i = 0; i < size; i++)
    {
        // get the pcb
        pcb = dyn_array_at(ready_queue, i);

        // check for bad pcb
        if (pcb == NULL)
        {
            return false;
        }

        // calculate the waiting time for each process
        if (i > 0)
        {
            waiting_time = waiting_time + (turnaround_time - pcb->arrival);
        }

        // run the virtual cpu
        virtual_cpu(pcb);

        // check if the pcb is done
        if (pcb->remaining_burst_time == 0)
        {
            turnaround_time = turnaround_time + pcb->arrival + pcb->remaining_burst_time;
        }
    }

    // update the schedule result with the calculated values
    result->average_waiting_time = (float) waiting_time / size;
    result->average_turnaround_time = (float) turnaround_time / size;
    result->total_run_time = turnaround_time;

    return true;
}


/*
* priority
*  Runs the Priority Scheduling algorithm over the incoming ready_queue
*  @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
*  @param result used for priority stat tracking \ref ScheduleResult_t
*  @return true if function ran successful else false for an error
*/
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    // //check for bad params
    // if (ready_queue == NULL || result == NULL)
    // {
    //     return false;
    // }

    // // initialize variables
    // int i = 0;
    // int size = dyn_array_size(ready_queue);
    // ProcessControlBlock_t *pcb = NULL;
    // int turnaround_time = 0;
    // int waiting_time = 0;

    // // loop through the ready queue
    // for (i = 0; i < size; i++)
    // {
    //     // get the pcb
    //     pcb = dyn_array_at(ready_queue, i);

    //     // check for bad pcb
    //     if (pcb == NULL)
    //     {
    //         return false;
    //     }

    //     // calculate the waiting time for each process
    //     waiting_time = waiting_time + (turnaround_time - pcb->arrival);

    //     // run the virtual cpu
    //     virtual_cpu(pcb);

    //     // check if the pcb is done
    //     if (pcb->remaining_burst_time == 0)
    //     {
    //         turnaround_time = turnaround_time + pcb->arrival + pcb->remaining_burst_time;
    //     }
    // }

    // // update the schedule result with the calculated values
    // result->average_waiting_time = (float) waiting_time / size;
    // result->average_turnaround_time = (float) turnaround_time / size;
    // result->total_run_time = turnaround_time;

    // return true;
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}


/*
* round_robin
*  Runs the Round Robin Scheduling algorithm over the incoming ready_queue
*  @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
*  @param result used for round robin stat tracking \ref ScheduleResult_t
*  @param quantum the time slice for the round robin algorithm
*  @return true if function ran successful else false for an error
*/
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
   // check for bad params
    if (ready_queue == NULL || result == NULL || quantum == 0)
    {
         return false;
    }

    // initialize variables
    int i = 0;
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t *pcb = NULL;
    int turnaround_time = 0;
    int waiting_time = 0;
    int response_time = 0;

    // loop through the ready queue
    for (i = 0; i < size; i++)
    {
        // get the pcb
        pcb = dyn_array_at(ready_queue, i);

        // check for bad pcb
        if (pcb == NULL)
        {
            return false;
        }

        if (pcb->remaining_burst_time > 0) {

        // set the response time for the first process
        if (response_time == 0)
        {
            response_time = pcb->arrival;
        }
        else
        {
           response_time += turnaround_time - pcb->arrival;
        }

        // run the virtual CPU for quantum amount of time
        int burst = pcb->remaining_burst_time > quantum ? quantum : pcb->remaining_burst_time;
        // run the virtual cpu
        virtual_cpu(pcb);
        // update the remaining burst time
        pcb->remaining_burst_time -= burst;

        // accumulate turnaround time and waiting time
        turnaround_time += burst;
        waiting_time += turnaround_time - pcb->arrival - burst;
        }
    }

    // update the schedule result with the calculated values
    result->average_waiting_time = (float) waiting_time / size;
    result->average_turnaround_time = (float) turnaround_time / size;
    result->total_run_time = turnaround_time;

    return true;
}
/* 
* load_process_control_blocks
*  Reads the PCB burst time values from the binary file into ProcessControlBlock_t remaining_burst_time field
*  @param input_file the file containing the PCB burst times
*  @return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
*/
dyn_array_t *load_process_control_blocks(const char *input_file) 
{
   //check for bad params
   if (input_file == NULL)
   {
       return NULL;
   }

    // initialize variables
    int fd = 0;
    int i = 0;
    int size = 0;
    int read_size = 0;
    ProcessControlBlock_t *pcb = NULL;
    dyn_array_t *pcb_array = NULL;

    // open the file
    fd = open(input_file, O_RDONLY);

    // check for bad file
    if (fd == -1)
    {
        return NULL;
    }

    // read the size of the file
    read_size = read(fd, &size, sizeof(int));

    // check for bad read
    if (read_size == -1)
    {
        return NULL;
    }

    // create the dyn array
    pcb_array = dyn_array_create(size, sizeof(ProcessControlBlock_t), NULL);

    // check for bad dyn array
    if (pcb_array == NULL)
    {
        return NULL;
    }


    // loop through the file
    for (i = 0; i < size; i++)
    {
        // get the pcb
        pcb = dyn_array_at(pcb_array, i);

        // check for bad pcb
        if (pcb == NULL)
        {
            return NULL;
        }

        // read the pcb burst time
        read_size = read(fd, &pcb->remaining_burst_time, sizeof(int));

        // check for bad read
        if (read_size == -1)
        {
            return NULL;
        }
    }

    // close the file
    close(fd);

    return pcb_array;
}

/*
* shortest_remaining_time_first
*  Runs the Shortest Remaining Time First Scheduling algorithm over the incoming ready_queue
*  @param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
*  @param result used for shortest remaining time first stat tracking \ref ScheduleResult_t
*  @return true if function ran successful else false for an error
*/
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    //check for bad params
    if (ready_queue == NULL || result == NULL)
    {
        return false;
    }

    // initialize variables
    int i = 0;
    int size = dyn_array_size(ready_queue);
    ProcessControlBlock_t *pcb = NULL;
    int turnaround_time = 0;
    int waiting_time = 0;
    int response_time = 0;

    // loop through the ready queue
    for (i = 0; i < size; i++)
    {
        // get the pcb
        pcb = dyn_array_at(ready_queue, i);

        // check for bad pcb
        if (pcb == NULL)
        {
            return false;
        }

        // set the response time for the first process
        if (response_time == 0)
        {
            response_time = pcb->arrival;
        }
        else
        {
            response_time += turnaround_time - pcb->arrival;
        }

        // run the virtual cpu
        virtual_cpu(pcb);

        // accumulate turnaround time and waiting time
        turnaround_time += pcb->remaining_burst_time;
        waiting_time += turnaround_time - pcb->arrival - pcb->remaining_burst_time;
    }

    // update the schedule result with the calculated values
    result->average_waiting_time = (float) waiting_time / size;
    result->average_turnaround_time = (float) turnaround_time / size;
    result->total_run_time = turnaround_time;

    return true;
}
