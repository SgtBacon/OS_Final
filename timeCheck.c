#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/////////////////////////
// Name: Michael Laramie, Jay Helm, Jonathan Hamstra
// Date: 5/14/23
// Class: Operating Systems
// Project: Final Project
// Dining Philosophers Algorithm #4
// Deadlock Avoidance
/////////////////////////

// Number of philosophers, also serves as the number of chopsticks.
#define PNUM 10
#define RUNS 3

pthread_t philosopher[PNUM];
// Mutex array
pthread_mutex_t stix[PNUM];

pthread_mutex_t fileOutput;

// Returns a random int from 1-3
int myRand()
{
    int x = rand() % 2 + 1;
    return x;
}

// Brings me back to Distributed days
void diningPhilosophers(int thid)
{
    for(int i = 0; i < RUNS; i++) {

        int randTime = myRand();
        printf("\nPhil #%d is Thinking for %d seconds", thid, randTime);
        sleep(randTime);

        //// Wait ////
        // Pick up left stick
        pthread_mutex_lock(&stix[thid - 1]);
        printf("\nPhil #%d wants to GRAB LEFT CHOPSTICK", thid);

        sleep(2);

        bool attempt = true;
        bool fail = false;
        int count = 0;
        printf("\nPhil #%d wants to GRAB RIGHT CHOPSTICK\n", thid);
        // Pick up right stick
        while (attempt)
        {
            if(pthread_mutex_trylock(&stix[thid % PNUM]) == 0) { 
                count = 0;
                break; 
            }
            count++;
            sleep(1);
            if (count >= 10 && thid == 1)
            {
                printf("\nPhil #%d is giving up", thid);
                pthread_mutex_unlock(&stix[thid - 1]);
                fail = true;
                attempt = false;
                sleep(1);
            }
        }
        if (fail == true)
        {
            printf("\n|||| Deadlock Detected ||||");
            sleep(1);
            fail = false;
            continue;
        }
        // Start eating for a rand amount of time
        printf("\nPhil #%d is EATING for %ds", thid, randTime);
        randTime = myRand();
        sleep(randTime);

        //// Signal ////
        // Put left stick down
        printf("\nPhil #%d wants to DROP LEFT CHOPSTICK", thid);
        pthread_mutex_unlock(&stix[thid - 1]);

        // Put right stick down
        printf("\nPhil #%d wants to DROP RIGHT CHOPSTICK", thid);
        pthread_mutex_unlock(&stix[(thid) % PNUM]);

        // All done.
        printf("\nPhil #%d has FINISHED EATING\n", thid);

        //Write data
        pthread_mutex_lock(&fileOutput);
        FILE *file;
        file = fopen("timeCheck3.csv", "a");
        fprintf(file,"%d, %d, %d, %d, %c", randTime, randTime, thid, (i + 1), '\n');
        fclose(file);
        pthread_mutex_unlock(&fileOutput);
    }
}

int main()
{
    //write headers
    FILE *file;
    file = fopen("timeCheck3.csv", "w+");
    fprintf(file,"Time to Eat, Time Satisfied, Philospher, Meals Eaten\n");
    fclose(file);
    // seed rand number
    srand(time(NULL));
    int i;
    // Initializing chopstick mutexes (1)
    for (i = 0; i < PNUM; i++)
    {
        pthread_mutex_init(&stix[i], NULL);
    }

    // Create threads that run diningPhilosophers(i+1)
    for (i = 1; i <= PNUM; i++)
    {
        pthread_create(&philosopher[i], NULL, (void *)diningPhilosophers, (void *)i);
    }

    // Cleanup //

    // Destroy threads
    for (i = 0; i < PNUM; i++)
    {
        pthread_join(philosopher[i], NULL);
    }

    // Destroy the mutexes (1)
    for (i = 0; i < PNUM; i++)
    {
        pthread_mutex_destroy(&stix[i]);
    }
}