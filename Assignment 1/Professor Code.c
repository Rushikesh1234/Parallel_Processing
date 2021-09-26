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
		
		printf("Current Process : %d \n", pid);
	
		///////////////////////////////
		
		printf("Process P0 - Generating data for P1 process \n");
		for(i=0;i<50;i++)
		{
			for(j=0;j<100;j++)
			{
				data[i][j] = generate_data(i,j);
			}
		}
		
		mtag = 1;
		
		printf("Process P0 - Sending generated data to P1 process \n");
		
		MPI_Isend(data, 5000, MPI_INT, 1, mtag, MPI_COMM_WORLD, &req_s) ;
		
		///////////////////////////////
		
		printf("Process P0 - Generating data for himself /herself  \n");
		
		for(i=50;i<100;i++)
		{
			for(j=0;j<100;j++)
			{
				data[i][j] = generate_data(i,j);
			}
		}
		
		printf("Process P0 - calculating data:- \n");
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
		MPI_Wait(&req_s, &status) ;
		
		////////////////////////////////
		
		
		printf("Process P0 - Receiving Calculated Data from P1 process \n");
		
		mtag = 2 ;
		MPI_Recv(row_sum, 50, MPI_INT, 1, mtag, MPI_COMM_WORLD, &status) ;	
		printf("Process P0 - Data received from P1 \n");
		
		///////////////////////////////
		
		printf("Process P0 - Printing P0 data and P1 data \n");
		for(i=0; i<100; i++) 
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
		MPI_Recv(data, 5000, MPI_INT, 0, mtag, MPI_COMM_WORLD, &status) ;
		
		///////////////////////////////
		
		printf("Process P1 - calculating data:- \n");
		for(i=0;i<50;i++)
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
		MPI_Send(row_sum, 50, MPI_INT, 0, mtag, MPI_COMM_WORLD) ;	
	}
	
	MPI_Finalize();

	clock_t end = clock();
	
	printf("\n");
	
	printf("Total Time - %f seconds \n", (double)(end - begin) / CLOCKS_PER_SEC);

	return 0;	
}	
