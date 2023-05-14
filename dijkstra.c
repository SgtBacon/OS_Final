#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

/////////////////////////
// Name: Michael Laramie, Jay Helm, Jonathan Hamstra
// Date: 5/14/23
// Class: Operating Systems
// Project: Final Project
// Dining Philosophers Algorithm #1
// Dijkstras Method (Limiting # of eaters) 
/////////////////////////

// Number of philosophers, also serves as the number of chopsticks.
#define PNUM 5
#define HUNGER_MAX PNUM-1

int hungryAmount = 0;


pthread_t philosopher[PNUM];
// Mutex array (1)
pthread_mutex_t stix[PNUM]; 

pthread_mutex_t hungryMutex;

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
    //while(1) {
        int randTime = myRand();
        printf("\nHuman Thinker #%d is Thinking for %d seconds", thid, randTime);
        sleep(randTime);

        while(hungryAmount >= HUNGER_MAX) {
            sleep(1);
        }
        
        pthread_mutex_lock(&hungryMutex);
        hungryAmount++;
        pthread_mutex_unlock(&hungryMutex);

        // Pick up left stick
        pthread_mutex_lock(&stix[thid - 1]);
        printf("\nHuman Thinker #%d has picked up the left chopstick", thid);

        // Pick up right stick
        pthread_mutex_lock(&stix[(thid) % PNUM]);
        printf("\nHuman Thinker #%d has picked up the right chopstick", thid);

        // Start eating for a rand amount of time 
        randTime = myRand();
        printf("\nHuman Thinker #%d is Consuming Food for %d seconds", thid, randTime);
        sleep(randTime);

        //// Signal ////
        // Put left stick down
        pthread_mutex_unlock(&stix[thid - 1]);
        printf("\nHuman Thinker #%d has put down the left chopstick", thid);

        // Put right stick down
        pthread_mutex_unlock(&stix[(thid) % PNUM]);
        printf("\nHuman Thinker #%d has put down the right chopstick", thid);

        pthread_mutex_lock(&hungryMutex);
        hungryAmount--;
        pthread_mutex_unlock(&hungryMutex);
 
        // All done.
        printf("\nHuman Thinker #%d has Consumed Enough\n", thid);
    //}
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