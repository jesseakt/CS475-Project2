/***
 * CS 475 Proejct 2
 * Author: Jesse Thoren
 * Date: April 17, 2017
 * Description: A project to compare performance in static vs. dynamic
 *    thread scheduling and small vs large chunksize in a parallel
 *    programming application.
 * References: web.engr.oregonstate.edu/~mjb/cs575/Projects/proj02.html
 ***/

#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


//Run by default with 4 threads unless script overrides NUMT
#ifndef NUMT
#define NUMT    4
#endif

//Run by default with an ARRAYSIZE of 32K unless script overrides
#ifndef ARRAYSIZE
#define ARRAYSIZE   32768
#endif

//Run by default with a CHUNKSIZE of 4096 unless script overrides
#ifndef CHUNKSIZE
#define CHUNKSIZE   4096
#endif

//Loop 10 times by default to compute an average run unless script overrides
#ifndef NUMTRIES
#define NUMTRIES    10
#endif

//Default to dynamic unless script overrides
#ifndef RUNDYNAMIC
#define RUNDYNAMIC  1
#endif

int main()
{
    //Confirm OpenMP is supported by current machine
    #ifndef _OPENMP
        fprintf(stderr, "OpenMP is not supported on this machine. \n");
        return 1;
    #endif

    //Inform user if NUMT exceeds omp_get_max_threads();
    if(NUMT > omp_get_max_threads())
    {
        fprintf(stderr, "Caution: NUMT specified exceeds omp_get_max_threads()\n");
        fprintf(stderr, "   NUMT: %d\nmaxNUMT: %d\n", NUMT, omp_get_max_threads());
        return 1;
    }

    //Set Number of threads to calculate with
    omp_set_num_threads(NUMT);

    double maxMegaMults = 0.;
    double sumMegaMults = 0.;
    long int numMuled = (long int)ARRAYSIZE * (long int)(ARRAYSIZE+1)/2;

    //Loop NUMTRIES times to form an average performance
    for(int times = 0; times < NUMTRIES; times++)
    {
        //Define and populate a random array
        srand(time(NULL));
        float *randArray = new float[ARRAYSIZE];
        for(int i = 0; i<ARRAYSIZE; i++)
            randArray[i] = (float) -2 + rand()*(float)4/(float)RAND_MAX;

        double time0 = omp_get_wtime();

        float prev_product = 0; //NECESSARY or loop will get optimized out
        if(RUNDYNAMIC)
        {
            #pragma omp parallel for schedule(dynamic,CHUNKSIZE)
            for(int i = 0; i<ARRAYSIZE; i++)
            {
                float prod = 1.;
                for(int j =0; j<=i; j++)
                {
                    prod *= randArray[j];
                }
                //NECESSARY or loop will get optimized out
                prev_product = prod;
            }
        }
        else
        {
            #pragma omp parallel for schedule(static,CHUNKSIZE)
            for(int i = 0; i<ARRAYSIZE; i++)
            {
                float prod = 1.;
                for(int j =0; j<=i; j++)
                {
                    prod *= randArray[j];
                }
                //NECESSARY or loop will get optimized out
                prev_product = prod;
            }
        }

        double time1 = omp_get_wtime();
        
        double megaMults = (double)numMuled/(time1-time0)/1000000.;
        if(maxMegaMults<megaMults)
            maxMegaMults = megaMults;
        sumMegaMults += megaMults;
    }
    double avgMegaMults = sumMegaMults/(double)NUMTRIES;

    if(RUNDYNAMIC)
        fprintf(stderr, "Threads = %2d; ChunkSize = %5d; Scheduling = dynamic\n", NUMT, CHUNKSIZE);
    else
        fprintf(stderr, "Threads = %2d; ChunkSize = %5d; Scheduling = static\n", NUMT, CHUNKSIZE);
    fprintf(stderr, "Max Performance: %8.2lf MegaMults/sec\n", maxMegaMults);
    fprintf(stderr, "Avg Performance: %8.2lf MegaMults/sec\n\n", avgMegaMults);
    return 0;
}
