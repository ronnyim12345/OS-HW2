#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

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
            total = 210;
        }

        virtual void TearDown()
        {
            ::testing::Test::RecordProperty("points_given", score);
            ::testing::Test::RecordProperty("points_total", total);
            std::cout << "SCORE: " << score << '/' << total << std::endl;
        }
};

TEST(first_come_first_serve, NullParams)
{
    bool ret = first_come_first_serve(NULL, NULL);
    bool expected = false; 
    EXPECT_EQ(expected, ret);
    if (ret == expected)
        score += 10;
}

// TEST (first_come_first_serve, EmptyQueue)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     EXPECT_EQ(0, first_come_first_serve(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (first_come_first_serve, OneProcess)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(10, first_come_first_serve(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (first_come_first_serve, MultipleProcesses)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, first_come_first_serve(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (first_come_first_serve, MultipleProcessesWithArrivalTime)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 5;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 10;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, first_come_first_serve(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (shortest_job_first, NullParameter)
// {
//     EXPECT_EQ(0, shortest_job_first(NULL));
//     score += 10;
// }

// TEST (shortest_job_first, EmptyQueue)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     EXPECT_EQ(0, shortest_job_first(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (shortest_job_first, OneProcess)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(10, shortest_job_first(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }


// TEST (shortest_job_first, MultipleProcesses)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, shortest_job_first(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (shortest_job_first, MultipleProcessesWithArrivalTime)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 5;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 10;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, shortest_job_first(queue));
//     dyn_array_destroy(queue);
//     score += 10;
// }


// TEST (round_robin, NullParameter)
// {
//     EXPECT_EQ(0, round_robin(NULL, 1));
//     score += 10;
// }

// TEST (round_robin, EmptyQueue)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     EXPECT_EQ(0, round_robin(queue, 1));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (round_robin, OneProcess)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(10, round_robin(queue, 1));
//     dyn_array_destroy(queue);
//     score += 10;
// }


// TEST (round_robin, MultipleProcesses)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, round_robin(queue, 1));
//     dyn_array_destroy(queue);
//     score += 10;
// }


// TEST (round_robin, MultipleProcessesWithArrivalTime)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 5;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 10;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, round_robin(queue, 1));
//     dyn_array_destroy(queue);
//     score += 10;
// }

// TEST (round_robin, MultipleProcessesWithArrivalTimeAndQuantum)
// {
//     dyn_array_t *queue = dyn_array_create(0, sizeof(pcb_t), NULL);
//     pcb_t pcb;
//     pcb.pid = 1;
//     pcb.arrival_time = 0;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 2;
//     pcb.arrival_time = 5;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     pcb.pid = 3;
//     pcb.arrival_time = 10;
//     pcb.run_time = 10;
//     dyn_array_push_back(queue, &pcb);
//     EXPECT_EQ(30, round_robin(queue, 5));
//     dyn_array_destroy(queue);
//     score += 10;
// }


int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}
