#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>

/////////////////////////
// Name: Michael Laramie, Jay Helm, Jonathan Hamstra
// Date: 5/14/23
// Class: Operating Systems
// Project: Final Project
// Dining Philosophers Algorithm #2
// Handedness Deadlock Prevention
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
void diningPhilosophersRight(int thid) {
    int randTime = myRand();
    printf("\nRighty #%d is Thinking for %d seconds", thid, randTime);
    sleep(randTime);

    // Pick up right stick
    pthread_mutex_lock(&stix[(thid) % PNUM]);
    printf("\nRighty #%d has picked up the right chopstick", thid);

    // Pick up left stick
    pthread_mutex_lock(&stix[thid - 1]);
    printf("\nRighty #%d has picked up the left chopstick", thid);

    // Start eating for a rand amount of time 
    randTime = myRand();
    printf("\nRighty #%d is Consuming Food for %d seconds", thid, randTime);
    sleep(randTime);

    // Put right stick down
    pthread_mutex_unlock(&stix[(thid) % PNUM]);
    printf("\nRighty #%d has put down the right chopstick", thid);

    // Put left stick down
    pthread_mutex_unlock(&stix[thid - 1]);
    printf("\nRighty #%d has put down the left chopstick", thid);

    // All done.
    printf("\nRighty #%d has Consumed Enough\n", thid);

    //Write data
    FILE *file;
    file = fopen("handedness.csv", "w+");
    fprintf(file, randTime, randtime, thid, hungryAmount);
    fclose(file);
}
void diningPhilosophersLeft(int thid) {

    // uncomment while loop to make it infinite.
    //while(1) {
        int randTime = myRand();
        printf("\nLefty #%d is Thinking for %d seconds", thid, randTime);
        sleep(randTime);

        //// Wait ////
        // Pick up left stick
        pthread_mutex_lock(&stix[thid - 1]);
        printf("\nLefty #%d has picked up the left chopstick", thid);

        // Pick up right stick
        pthread_mutex_lock(&stix[(thid) % PNUM]);
        printf("\nLefty #%d has picked up the right chopstick", thid);

        // Start eating for a rand amount of time 
        randTime = myRand();
        printf("\nLefty #%d is Consuming Food for %d seconds", thid, randTime);
        sleep(randTime);

        //// Signal ////
        // Put left stick down
        pthread_mutex_unlock(&stix[thid - 1]);
        printf("\nLefty #%d has put down the left chopstick", thid);

        // Put right stick down
        pthread_mutex_unlock(&stix[(thid) % PNUM]);
        printf("\nLefty #%d has put down the right chopstick", thid);

        // All done.
        printf("\nlefty #%d has Consumed Enough\n", thid);

        //write data
        FILE *file;
        file = fopen("handedness.csv", "w+");
        fprintf(file, randTime, randtime, thid, hungryAmount);
        fclose(file);
    //}
}

int main()
{
    //write headers
    FILE *file;
    file = fopen("handedness.csv", "w+");
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
    for (i = 0; i < PNUM; i++)
    {
        if(i % 2 == 0) {
            pthread_create(&philosopher[i], NULL, diningPhilosophersRight, i + 1);
        }
        else {
            pthread_create(&philosopher[i], NULL, diningPhilosophersLeft, i + 1);
        }
    }

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