#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

/////////////////////////
// Name: Michael Laramie
// Date: 3/16/23
// Class: Operating Systems
// Project: Lab 5
/////////////////////////
// Sources:
// 1. https://www.ibm.com/docs/en/i/7.3?topic=ssw_ibm_i_73/apis/users_61.html
//     I referenced IBM when making the mutex array stix[] and when destroying them.
/////////////////////////

// Number of philosophers, also serves as the number of chopsticks.
#define PNUM 5

pthread_t philosopher[PNUM];
// Mutex array (1)
pthread_mutex_t stix[PNUM]; 

// Returns a random int from 1-3
int myRand()
{
    int x = rand() % 2 + 1;
    return x;
}

// Brings me back to Distributed days
void diningPhilosophers(int thid)
{
    // uncomment while loop to make it infinite.
    while(1) {
        int randTime = myRand();
        printf("\nHuman Thinker #%d is Thinking for %d seconds", thid, randTime);
        sleep(randTime);

        //// Wait ////
        // Pick up left stick
        pthread_mutex_lock(&stix[thid]);
        printf("\nHuman Thinker #%d has picked up the left chopstick", thid);

        // Pick up right stick
        pthread_mutex_lock(&stix[(thid + 1) % PNUM]);
        printf("\nHuman Thinker #%d has picked up the right chopstick", thid);

        // Start eating for a rand amount of time 
        randTime = myRand();
        printf("\nHuman Thinker #%d is Consuming Food for %d seconds", thid, randTime);
        sleep(randTime);

        //// Signal ////
        // Put left stick down
        pthread_mutex_unlock(&stix[thid]);
        printf("\nHuman Thinker #%d has put down the left chopstick", thid);

        // Put right stick down
        pthread_mutex_unlock(&stix[(thid + 1) % PNUM]);
        printf("\nHuman Thinker #%d has put down the right chopstick", thid);

        // All done.
        printf("\nHuman Thinker #%d has Consumed Enough\n", thid);
    }
}

int main()
{
    // seed rand number
    srand(time(NULL));
    int i;
    // Initializing chopstick mutexes (1)
    for (i = 0; i < PNUM; i++)
    {
        pthread_mutex_init(&stix[i], NULL);
    }

    // Create threads that run diningPhilosophers(i+1)
    for (i = 0; i < PNUM; i++)
    {
        pthread_create(&philosopher[i], NULL, diningPhilosophers, i + 1);
    }

    // Cleanup //

    // Destroy threads
    for (i = 0; i < PNUM; i++)
    {
        pthread_join(philosopher[i], NULL);
    }

    // Destroy the mutexes (1)
    for(i = 0; i < PNUM; i++) {
        pthread_mutex_destroy(&stix[i]);
    }
}