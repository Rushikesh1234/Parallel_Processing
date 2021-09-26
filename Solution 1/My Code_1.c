/*************************************************************************

Name - Rushikesh M. Khamkar
R ID - R11788309

Comment - 
1. Use MPI Synchronous Sending and Receiving Method
2. To solve the problem, to achieve overlapping on P1 process and to keep our buffer ready to send-receive data, i used synchronous mode for sending and receiving data. So what happen, whenever P0 send data to P1, it indicate that P0 send buffer can be reused and P1 starts to receive data, make buffer ready to use for next operation. In other word, our P0 and P1 works synchronously for sending and receiving data & there will be buffer availalbe for them.
3. When i execute my code and compare with Professor code, it give me result like this - 
	- Total Time For My Code - 0.012001 seconds
	- Total Time For Professor Code - 0.012613 seconds
	- Time Differene is - 0.000612 seconds
5. In Code section, i rewrite thecode again (didn't make any other changes, just try to code in simple way), i have added comments in the code.
5. In Code, For debug purpose, i have added few print statements. i'll keep that in code for understanding purpose like how our process our executing. From print statement, we will get to know execution of process for solving a problem.
6. For Execution Time Calculation, I have added Clock file in program
7. I am not sure that my solution is correct or not, but i tried my level best to get solution from my side for this.
**************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define generate_data(i,j)  (i)+(j)*(j)

int main( int argc, char **argv)
{
	int i, j, pid, np, mtag, count;
	double t0, t1 ;
	//int rank, size;
	int data[100][100], row_sum[100] ;
	MPI_Status status;
	MPI_Request req_s, req_r;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	
	printf("Size is %d \n", np);
	printf("Rank is %d \n",pid);

	clock_t begin = clock();							//Use TIme library to calculate how much time will be taken by our code; its begining time of our code

	if(pid == 0) 																						//Start with Process Id 0 
	{
		
		printf("Current Process : %d \n", pid);
	
		///////////////////////////////
		
		printf("Process P0 - Generating data for P1 process \n");
		for(i=0;i<50;i++)																			//P0 is generating data for P1 process
		{
			for(j=0;j<100;j++)
			{
				data[i][j] = generate_data(i,j);
			}
		}
		
		mtag = 1;
		
		printf("Process P0 - Sending generated data to P1 process \n");
		
		MPI_Issend(data, 5000, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_s) ;		//Send nonblocking synchronous send to P1
		
		///////////////////////////////
		
		printf("Process P0 - Generating data for himself /herself  \n");
		
		for(i=50;i<100;i++)																			//P0 is generating data for himself/herself
		{
			for(j=0;j<100;j++)
			{
				data[i][j] = generate_data(i,j);
			}
		}
		
		printf("Process P0 - calculating data:- \n");										//P0 is calculating data
		for(i=50;i<100;i++)
		{
			row_sum[i] = 0;
			for(j=0;j<100;j++)
			{
				row_sum[i] = row_sum[i] + data[i][j];
			}
		}
		printf("Process P0 - Done with Calculation \n");
		
		////////////////////////////////
		/*
		printf("Process P0 - Calculated data from P0 \n");
		for(i=50; i<100; i++) 
		{
			printf(" %d  \n", row_sum[i]) ;
    		}
		*/
		/////////////////////////////////
		
		printf("Process P0 - Waiting for P1 data \n");		
		MPI_Wait(&req_s, &status) ;						//Now, P0 is waiting for P1's data, means waiting for completing receive  call from P1 side
		
		////////////////////////////////
		
		
		printf("Process P0 - Receiving Calculated Data from P1 process \n");
		
		mtag = 2 ;
		MPI_Irecv(row_sum, 50, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_r) ;	//P0 receives data from P1
		printf("Process P0 - Data received from P1 \n");
		
		MPI_Wait(&req_r, &status);					//P0 is wait until buffer is cleared out for next operation,will wait until receive operation is not completed
		
		///////////////////////////////
		
		printf("Process P0 - Printing P0 data and P1 data \n");
		for(i=0; i<100; i++) 														//Print calculated data
		{
			if(i % 5 == 0)
			{
				printf("\n");
			}
			printf(" %d ", row_sum[i]) ;
    		}
	}
	else
	{
		printf("Current Process : %d \n", pid);
		
	
		//////////////////////////////
		
		printf("Process P1 - Receiving Data from P0 process \n");
		
		mtag = 1 ;
		MPI_Irecv(data, 5000, MPI_INT, 0, mtag, MPI_COMM_WORLD, &req_r) ;   	//P1 Receives data from P0
		
		MPI_Wait(&req_r, &status);				//P1 is wait until buffer is cleared out for next operation, will wait until receive operation is not completed
		
		///////////////////////////////
		
		printf("Process P1 - calculating data:- \n");
		for(i=0;i<50;i++)											//P1 is calculating data
		{
			row_sum[i] = 0;
			for(j=0;j<100;j++)
			{
				row_sum[i] = row_sum[i] + data[i][j];
			}
		}
		printf("Process P1 - Done with Calculation \n");
		
		////////////////////////////////
		/*
		printf("Process P1 - Calculated data from P1 \n");
		for(i=0; i<50; i++) 
		{
			printf(" %d  \n", row_sum[i]) ;
    		}
		*/
		///////////////////////////////
					
		printf("Process P1 - Sending calculated data to P0 process \n");
		
		mtag = 2 ;
		MPI_Issend(row_sum, 50, MPI_INT, 0, mtag, MPI_COMM_WORLD,&req_s) ;  //P1 will send nonblocking synchronous send to P0
		
		MPI_Wait(&req_s, &status) ;     //P1 will wait till  your buffers get ready for n
	}
	
	MPI_Finalize();

	clock_t end = clock();    							//Use TIme library to calculate how much time will be taken by our code; its ending time of our code
	
	printf("\n");
	
	printf("Total Time - %f seconds \n", (double)(end - begin) / CLOCKS_PER_SEC);        // Total time taken by processes, on output we will get 2 total time per process

	return 0;	
}	
