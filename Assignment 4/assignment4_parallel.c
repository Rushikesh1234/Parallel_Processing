#include<stdio.h>
#include<time.h>
#include<mpi.h>
#include<math.h>

#define infinity 9999

int n=4;
int b[4][4];
int a[4][4];

void main(int argc, char ** argv)
{
	int pid, np;

	// input array values
		a[0][0] = 0;
		a[0][1] = 5;
		a[0][2] = infinity;
		a[0][3] = 10;
		a[1][0] = infinity;
		a[1][1] = 0;
		a[1][2] = 3;
		a[1][3] = infinity;
		a[2][0] = infinity;
		a[2][1] = infinity;
		a[2][2] = 0;
		a[2][3] = 1;
		a[3][0] = infinity;
		a[3][1] = infinity;
		a[3][2] = infinity;
		a[3][3] = 0;
	
	// clock variable is defined to calculate execution time
	clock_t begin = clock();

	// initialize mpi init to get number of processor
	MPI_Init(&argc,&argv);
	// get rank for each processor for mpi comm world
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	// get number of processor available in mpi comm world
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	//divide marix into sub matrices for all processor
	int m = n / np;
	
	// broadcast data to all processor with array value with their own matrix size m and also sharing actual array size n to get other array element which is requried for computation (to check minimum distance)
	MPI_Bcast(a, n*n, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	// I have divided data from actual matrix to submatrix based on the k value (use k value to divdie data because k value also helps us get current column and row value for each iteration)
	int value_to_send = 1;
	for(int k=1;k<=n;k++)
	{
		int x =1;
		//we initialize x value which iterate for np (number of process) number of time
		//below loop will execute for k times. It will helps us to allocate row value to each processor
		for(x=1; x<=np; x++)
		{
			//we are deciding which processor will pick which row of the matrix
			if((1+m*(x-1) <= k) && (k <= m*x))
			{
				value_to_send = x;
			}
		}
		// if k value is not less than (m*x), it means that we are at the end of the row value of matrix, so we will allocate this value to our last processor 
		if((1+m*(x-1) <= k) && (k <= n))
		{
			value_to_send = np;
		}
		
		// we are broadcasting array values to specific processor id (value_to_send - 1) which we have decied earlier. Initially, it will broadcast input array to processor, later on for each iteration, it will helps us to broadcast calculated array value
		MPI_Bcast(&a[k], n+1, MPI_INT, value_to_send-1, MPI_COMM_WORLD);
		
		// allocate and process matrix value to each processor
		if(pid != np-1)
		{
			// we are caluclating value for each row which is assigned to each processor. 
			// example - if pid = 0, then our value of i is 1 (here i have considered - m=2), then it will go to next line - j loop and calculate minimum distance value there and store them.
			// next time, i will check condition, where i=2 and i <= 2*(0+1) i.e. i <= 2. So we will again calculate value for 2nd row.
			for(int i=1+m*(pid); i<=m*(pid+1);i++)
			{
				for(int j=1; j<=n; j++)
				{
					if(a[i][j] > a[i][k]+a[k][j])
					{
						a[i][j] = a[i][k]+a[k][j];
					}
					else
					{
						a[i][j] = a[i][j];
					}
				}
			}
		}
		else
		{
			// if processor id is last processor from our list, it will consider last row value from matrix for calculation
			for(int i=1+m*pid; i<= n; i++)
			{
				for(int j=1; j<=n; j++)
				{
					if(a[i][j] > a[i][k]+a[k][j])
					{
						a[i][j] = a[i][k]+a[k][j];
					}
					else
					{
						a[i][j] = a[i][j];
					}
				}
			}
		}
	}
	
	//logic for broadcasting is the same as above
	for(int k=1;k<=n;k++)
	{
		int x=1;
		for(x=1; x<=np; x++)
		{
			if((1+m*(x-1)<=k) && (k<=m*x))
			{
				value_to_send = x;
			}
		}
		if((1+m*(x-1) <= k) && (k<=n))
		{
			value_to_send = np;
		}
		// here we are broadcasting array value among all processor after calculation for each k.
		MPI_Bcast(&a[k], n+1, MPI_INT, value_to_send-1, MPI_COMM_WORLD);
	}
	
	//stored value in the final output marix
	MPI_Reduce(a, b, n*n, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	
	// print output value
	if(pid == 0)
	{
		printf("\nOutput\n");
		for(int i=1;i<=n;i++)
		{
			for(int j=1;j<=n;j++)
			{
				printf("%d ", b[i][j]);
			}
			printf("\n");
		}
	}
	
	// close mpi execution
	MPI_Finalize();
	
	//clock varaible to calculate code execution time
	clock_t end = clock();
	
	printf("\nTotal Time For Execution - %f \n", (double)(end-begin)/CLOCKS_PER_SEC);
}
