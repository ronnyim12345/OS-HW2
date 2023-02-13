#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"
#include "../include/dyn_array.h"
#include <sys/stat.h>



#include "dyn_array.h"
#include "processing_scheduling.h"

extern "C" 
{
    #include "../include/processing_scheduling.h"
}




// Using a C library requires extern "C" to prevent function managling
extern "C" 
{
#include <dyn_array.h>
}


#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment 
{
    public:
        virtual void SetUp() 
        {
            score = 0;
            total = 230;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};



// LOAD PROCESS CONTROL BLOCKS TESTS:

TEST(load_process_control_blocks, NullParams)
{
    dyn_array_t *d = load_process_control_blocks(NULL);
    EXPECT_EQ(NULL, d);
    if (d == NULL)
        score += 10;

}


TEST(load_process_control_blocks, FileNotFound)
{
    dyn_array_t *d = load_process_control_blocks("ppcb.bin");
    EXPECT_EQ(NULL, d);
    if (d == NULL)
        score += 10;

}


TEST(load_process_control_blocks, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ProcessControlBlock_t *p = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
    p = (ProcessControlBlock_t*)dyn_array_at(d, 3);
    uint32_t result = 20;
    EXPECT_EQ(result, p->remaining_burst_time);
    if (result == p->remaining_burst_time)
        score += 10;    
}









// FIRST COME FIRST SERVE TESTS:

TEST(first_come_first_serve, NullParams)
{
    bool result = first_come_first_serve(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}


TEST(first_come_first_serve, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    first_come_first_serve(d, r);
    
    EXPECT_EQ(17.5, r->average_waiting_time);
    EXPECT_EQ(30, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (17.5 == r->average_waiting_time)
        score += 10;
        
    if (30 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    
    
}











// SHORTEST JOB FIRST TESTS:

TEST(shortest_job_first, NullParams)
{
    bool result = shortest_job_first(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}



TEST(shortest_job_first, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_job_first(d, r);
    
    
    EXPECT_EQ(12.5, r->average_waiting_time);
    EXPECT_EQ(25, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (12.5 == r->average_waiting_time)
        score += 10;
        
    if (25 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    
}













// PRIORITY TESTS:

TEST(priority, NullParams)
{
    bool result = priority(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}



TEST(priority, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    
    priority(d, r);
    
    //ProcessControlBlock_t *p = (ProcessControlBlock_t*)malloc(sizeof(ProcessControlBlock_t));
    //p = (ProcessControlBlock_t*)dyn_array_at(d, 3);
    //uint32_t result = 3;
    //EXPECT_EQ(result, p->arrival);
    
    
    EXPECT_EQ(17.5, r->average_waiting_time);
    EXPECT_EQ(30, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (17.5 == r->average_waiting_time)
        score += 10;
        
    if (30 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    
}















// ROUND ROBIN TESTS:

TEST(round_robin, NullParams)
{
    bool result = round_robin(NULL, NULL, 0);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}



TEST(round_robin, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    round_robin(d, r, 5);
    
    EXPECT_EQ(21.25, r->average_waiting_time);
    EXPECT_EQ(33.75, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (21.25 == r->average_waiting_time)
        score += 10;
        
    if (33.75 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    
}













// ROUND ROBIN TESTS:

TEST(shortest_remaining_time_first, NullParams)
{
    bool result = shortest_remaining_time_first(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}



TEST(shortest_remaining_time_first, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_remaining_time_first(d, r);
    
    EXPECT_EQ(13.25, r->average_waiting_time);
    EXPECT_EQ(25.75, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (13.25 == r->average_waiting_time)
        score += 10;
        
    if (25.75 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    
}










int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
