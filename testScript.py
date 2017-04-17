#Testing Script for CS475 Project2
#Jesse Thoren
#Description: Compiles/runs project2.cpp with a variety of thread quantities
#   (NUMT).

import os
for NUMT in [1, 2, 4, 6, 8, 10, 12, 14, 16]:
#    for CHUNKSIZE in [1, 32, 128, 1024, 4096]:
    for CHUNKSIZE in [1, 4096]:
        for RUNDYNAMIC in [0, 1]:
            cmd = "g++ -DNUMT=%d -DNUMTRIES=10 -DCHUNKSIZE=%d -DRUNDYNAMIC=%d project2.cpp -o runProg -O3 -lm -fopenmp" % (NUMT,CHUNKSIZE,RUNDYNAMIC)
            os.system( cmd )
            cmd = "./runProg"
            os.system( cmd )
cmd = "rm -f runProg"
os.system( cmd )
