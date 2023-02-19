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
            total = 380;
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

    free(d);
}


TEST(load_process_control_blocks, FileNotFound)
{
    dyn_array_t *d = load_process_control_blocks("ppcb.bin");
    EXPECT_EQ(NULL, d);
    if (d == NULL)
        score += 10;

    free(d);
}


TEST(load_process_control_blocks, GoodWorking)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ProcessControlBlock_t *p;
    p = (ProcessControlBlock_t*)dyn_array_at(d, 3);
    uint32_t result = 20;
    EXPECT_EQ(result, p->remaining_burst_time);
    if (result == p->remaining_burst_time)
        score += 10;    
        
    free(d);
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


TEST(first_come_first_serve, PcbTest)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    first_come_first_serve(d, r);
    
    EXPECT_EQ(16, r->average_waiting_time);
    EXPECT_EQ(28.5, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (16 == r->average_waiting_time)
        score += 10;
        
    if (28.5 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
        
    free(r);
    free(d);
    
}



TEST(first_come_first_serve, TextBookExample)
{
    dyn_array_t *d = load_process_control_blocks("FCFS.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    first_come_first_serve(d, r);
    
    EXPECT_EQ(17, r->average_waiting_time);
    EXPECT_EQ(27, r->average_turnaround_time);
    EXPECT_EQ(30, (int)r->total_run_time);
    
    if (17 == r->average_waiting_time)
        score += 10;
        
    if (27 == r->average_turnaround_time)
        score += 10;
        
    if (30 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
    
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



TEST(shortest_job_first, PcbTest)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_job_first(d, r);
    
    
    EXPECT_EQ(14.75, r->average_waiting_time);
    EXPECT_EQ(27.25, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (14.75 == r->average_waiting_time)
        score += 10;
        
    if (27.25 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}



TEST(shortest_job_first, TextBookExample)
{
    dyn_array_t *d = load_process_control_blocks("SJF.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_job_first(d, r);
    
    
    EXPECT_EQ(7, r->average_waiting_time);
    EXPECT_EQ(13, r->average_turnaround_time);
    EXPECT_EQ(24, (int)r->total_run_time);
    
    if (7 == r->average_waiting_time)
        score += 10;
        
    if (13 == r->average_turnaround_time)
        score += 10;
        
    if (24 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
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



TEST(priority, PcbTest)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    
    priority(d, r);
    
    EXPECT_EQ(16.0, r->average_waiting_time);
    EXPECT_EQ(28.5, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (16.0 == r->average_waiting_time)
        score += 10;
        
    if (28.5 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}


TEST(priority, TextBookExample)
{
    dyn_array_t *d = load_process_control_blocks("P.bin");
    
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    
    priority(d, r);
    
    float res = r->average_waiting_time;
    float ex = 8.2;
    
    EXPECT_EQ(ex, res);
    EXPECT_EQ(12, r->average_turnaround_time);
    EXPECT_EQ(19, (int)r->total_run_time);
    
    if (ex == res)
        score += 10;
        
    if (12 == r->average_turnaround_time)
        score += 10;
        
    if (19 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
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



TEST(round_robin, PcbTest)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    round_robin(d, r, 5);
    
    EXPECT_EQ(19.75, r->average_waiting_time);
    EXPECT_EQ(32.25, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (19.75 == r->average_waiting_time)
        score += 10;
        
    if (32.25 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}

TEST(round_robin, TextBookExample)
{
    dyn_array_t *d = load_process_control_blocks("RR.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    round_robin(d, r, 4);
    
    float res1 = 17;
    float res2 = 47;
    
    EXPECT_EQ(res1/3, r->average_waiting_time);
    EXPECT_EQ(res2/3, r->average_turnaround_time);
    EXPECT_EQ(30, (int)r->total_run_time);
    
    if (res1/3 == r->average_waiting_time)
        score += 10;
        
    if (res2/3 == r->average_turnaround_time)
        score += 10;
        
    if (30 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}







// SHORTEST REMAINING TIME FIRST TESTS:

TEST(shortest_remaining_time_first, NullParams)
{
    bool result = shortest_remaining_time_first(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, result);
    if (result == expected)
        score += 10;
}



TEST(shortest_remaining_time_first, PcbTest)
{
    dyn_array_t *d = load_process_control_blocks("pcb.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_remaining_time_first(d, r);
    
    EXPECT_EQ(11.75, r->average_waiting_time);
    EXPECT_EQ(24.25, r->average_turnaround_time);
    EXPECT_EQ(50, (int)r->total_run_time);
    
    if (11.75 == r->average_waiting_time)
        score += 10;
        
    if (24.25 == r->average_turnaround_time)
        score += 10;
        
    if (50 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}


TEST(shortest_remaining_time_first, TextBookExample)
{
    dyn_array_t *d = load_process_control_blocks("SRTF.bin");
    ScheduleResult_t *r = (ScheduleResult_t*)malloc(sizeof(ScheduleResult_t));
    shortest_remaining_time_first(d, r);
    
    EXPECT_EQ(6.5, r->average_waiting_time);
    EXPECT_EQ(13, r->average_turnaround_time);
    EXPECT_EQ(26, (int)r->total_run_time);
    
    if (6.5 == r->average_waiting_time)
        score += 10;
        
    if (13 == r->average_turnaround_time)
        score += 10;
        
    if (26 == (int)r->total_run_time)
        score += 10;
    
    free(r);
    free(d);
}







int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
