/*************************************************************************

Name - Rushikesh M. Khamkar
R ID - R11788309

Comment - 
1. Use Code Changes in Professor Code and try to get synchronisation in code
1. To Solve this problem statement, I make changes at code level, i have updated sending and receiving process for P0 and P1 respectively. When P0 will generate data for sending, once he generate data for 1st row, we will send those data immediately to our P1 process. Our P1 will get those data, calculate and send back to P0 process. Till that time, P0 will wait for getting buffer ready for next task. As soon as P0 receives empty buffer and calculated data from P1, it starts working next row. So, in this way, we keep our P0 and P1 process busy, so when P0 sends data, P1 is already calculating previous row data and when P1 sends data, P0 is working on Data generation for next row. So, this is how we are able to achieve overlapping on computation and communication as well.
3. When i execute my code and compare with Professor code, it give me result like this - 
	- Total Time For My Code - 0.007951 seconds
	- Total Time For Professor Code - 0.009202 seconds
	- Time Differene is - 0.001251 seconds
4. In Code section, i rewrite thecode again (didn't make any other changes, just try to code in simple way), i have added comments in the code.
5. In Code, For debug purpose, i had added few print statements. i'll keep that in code for understanding purpose like how our process are executing. From print statement, we will get to know those things.
6. For Execution Time Calculation, I have added Clock file in program
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

	clock_t begin = clock();

	if(pid == 0)
	{
		
		printf("Current Process : %d \n", pid);											//Print current process id for debugging
	
		///////////////////////////////
		
		printf("Process P0 - Starts Sending data to and Receiving data from P1 \n");
		for(i=0;i<50;i++)																// P0 is generating data for Process P1
		{
			for(j=0;j<100;j++)													//P0 will generate data rowwise
			{
				data[i][j] = generate_data(i,j);
			}
			
			mtag = 1;
			MPI_Isend(&data[i][i], 100, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_s) ; //P0 will send data to P1 rowwise, like P0 will send 1st row, then 2nd
			
			MPI_Wait(&req_s, &status) ;											//While P1 is receiving data, P0 will wait to get buffer free for next task
			
			mtag = 2 ;
			MPI_Recv(&row_sum[i], 1, MPI_INT, 1, mtag, MPI_COMM_WORLD, &status) ;  //P0 will receives data from P1
		}
		
		/*
		printf("Process P1 - Calculated data from P1 \n");
		for(i=0; i<50; i++) 
		{
			printf(" %d  \n", row_sum[i]) ;
    		}*/

		
		////////////////////////////////
		
		printf("Process P0 - Generating data for himself /herself  \n");
		
		for(i=50;i<100;i++)																		//P0 is generating data for himself/herself for calculation
		{
			for(j=0;j<100;j++)
			{
				data[i][j] = generate_data(i,j);
			}
		}
		
		////////////////////////////////
		
		printf("Process P0 - calculating data:- \n");
		for(i=50;i<100;i++)																//P0 is calculating data
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
		///////////////////////////////
		
		printf("Process P0 - Printing P0 data and P1 data \n");
		for(i=0; i<100; i++) 															//P0 is printing calculated data
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
		
		///////////////////////////////
		
		printf("Process P1 - Starts Sending data to and Receiving data from P0 \n");
		for(i=0;i<50;i++)											// P1 process receives data from P0, calculate data and sent back to process P0
		{
			row_sum[i] = 0;
			
			mtag = 1 ;
			MPI_Recv(&data[i][i], 100, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status) ; //P1 receives data from P0
			
			for(j=0;j<100;j++)																			//P1 is calculating data
			{
				row_sum[i] = row_sum[i] + data[i][j];
			}
			
			mtag = 2 ;
			MPI_Isend(&row_sum[i], 1, MPI_INT, 0, mtag, MPI_COMM_WORLD, &req_s) ; //P1 sends calculated data to P0 
		}
		printf("Process P1 - Done with Calculation \n");
		
	}
	
	//////////////////////////////
	
	MPI_Finalize();																		//Ending MPI Process

	clock_t end = clock();
	
	printf("\n");
	
	printf("Total Time - %f seconds \n", (double)(end - begin) / CLOCKS_PER_SEC);		//calculating time spent by processes
	
	
	return 0;	
}	
