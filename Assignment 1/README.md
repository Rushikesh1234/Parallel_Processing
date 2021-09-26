# Assignment 1
This module contains MPI one to one communication assignment

## Proofessor Problem Statement - 

Assignment 1 was given in detail during a class and a brief email later indicated the focus of the assignment.  Now I'd like to describe below what was discussed in the class for the assignment.

The given code (attached) is to calculate the sums of rows for an array data[][], where the process with pid==0 generates the data, send half the data to the process with pid==1 for pid1 to calculate the sums of the 50 rows, and pid0 calculates the row sums of the remaining 50 rows of the data. The code shows how overlapping of computation and communication can be done for  pid0.  But on the process with pid==1, there is no overlapping of computation and communication. Simply replacing MPI_Recv() by MPI_Irecv() on  pid1 will not work.  Also will not work by replacing MPI_Recv() by MPI_Irecv() immediately followed by MPI_Wait() on pid1.

So Assignment 1 is to keep the same computation partition among the two processes,  but achieve significant   overlapping of computation and communication on the process with pid==1.  That means, the process with pid==0 still generates all data, send half the data to pid1 for it to calculate the row sums from row 0 through row 49, and pid0 calculates the remaining half row sums (row50 through row 99).  By significant, I do not require you achieve overlapping for all possible cases, as long as there is significant overlapping.  My hint on achieving overlapping is how pid0 sends half of the data to pid1 and how pid1 receives them from pid0.

## My Comments - 

I have tried to solve a problem and try to achieve enough overlapping on computation and calculation between 2 processes. I solved this problem in 2 ways – Using Non-Blocking Synchronous method for sending-receiving data and other way is – I make changes at code level, I tried to get that processes synchronously by updating their send and receive process. I have attached both the files in blackboard with their screenshots.

In 1st File, I used Non-blocking Synchronous Send Mode to share data from one process to other process. If we used that method in code, then we will be able to achieve 2 things – first, whenever 2nd process
receives data, it will indicate our 1st process that buffer is ready to use for next task, so 1st process will be able to start to work on his/her next task. And, second, receiving process (2nd Process) has started receiving
the data simultaneously. So, in this way, our both processes will work parallel and we will able to achieve overlapping in those 2 processes.

In 2nd File I didn’t used any other mode, I used only non-blocking standard methods but, here I changed the way of sending and receiving data for both process. Now, 1st process generates data for 1strow and immediately sent to the 2nd process for calculation. When 2nd process is receiving those data, 1st process will wait to get buffer ready to use. Once buffer gets ready, 1st process will start to work on generating 2nd row data and simultaneously, our 2nd process will work on calculation for 1st row data. Once calculation is finished by 2nd row data, it will send back to 1st process. And, this will goes on, till our loop is not finished.

While compiling code, I observed some Total Execution Time changes within my code and professor code. I am sharing my observation here –
1) For 1st File - \
    Total Time for My Code - 0.012001 seconds\
    Total Time for Professor Code - 0.012613 seconds\
    Time Difference is - 0.000612 seconds
2) For 2nd File -
    Total Time for My Code - 0.007951 seconds\
    Total Time for Professor Code - 0.009202 seconds\
    Time Difference is - 0.001251 seconds\
When I run my program, I observed that my code takes less time to execute than Professor Code (only a difference of milliseconds)

I have tried very hard to find work around of professor problem. It looks simple, but forced us to think on it. If my solution is not able to follow as per your constraints, please share solution to us. It will helps us a lot.
