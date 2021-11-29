#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<math.h>
#include <mpi.h>

/*****************************************************************************
In dijkstras, to get minimum distance value for next path, we need to consider all value like we need edge value to get edge between2 nodes, 
found value gives us that current node is visited or not, distance variable will calculate distance between nodes, 
so if we keep those variable in main, our process won't be able to calculate value synchronuously. 
So we declared those value as global variable wich helps process to fetch updated values
******************************************************************************/

#define n 6
int edge[n][n];
int found[n];
int distance[n];
int path[n];
const int temp = 98765432;
int num_process;
int pid;


MPI_Status status;
MPI_Request request;

/*********************************************************************
 Input: n and edge[n][n], where n is the number of vertices of a graph
 edge[i][j] is the length of the edge from vertex i to vertex j
 Output: distance[n], the distance from the SOURCE vertex to vertex i.
 *********************************************************************/
 
 void dijkstra(int SOURCE) 
 {
 	int value[2],val[2],least,leastPos;
 	
 	
 	/*********************************************************************
 	In below loop, we are going to share array data among all processes. 
	Initially pid=0 will take 0th element and assign value to found and distance to that element and incrment index with counter as num_process. 
	So, If we have 6 elment and 2 processor, then Processor 0 will work on - 0, 2, 4 and Processor 1 will work on - 1, 3, 5. 
	It helps us to keep data consistency in all processor.
 	**********************************************************************/
 	for(int i=pid; i<n; i=i+num_process) 
 	{
 		found[i]= 0;
 		distance[i] = edge[SOURCE][i];
 		path[i] = SOURCE;
 	}
 	
 	/************
 	Assign found value as 1 for source.
 	***********/
 	
 	found[SOURCE] = 1 ;
 	
 	/************************************
 	Increment counter till the n value, where we are calculating distance for all nodes in network
 	***************************************/
 	for(int j=1;j<n;j++)
 	{
 		//printf("count %d \n", count);
		least = temp;
		
		/*************************
		We need to calculate least and leastPos value for node, we select minimum calculated value as our distance. 
		We are going to divide those items amng our processors by incrmenting value by num_process.
		*/
		for(int i=pid; i<n; i=i+num_process) 
		{
				//printf("Process %d found sometihng at index %d \n", pid, i);
				int tmp = distance[i] ;
				if( (!found[i]) && (tmp < least))
				{
					least = tmp ;
					leastPos = i ;
					//printf("Process %d found at index %d - least = %d and leasePos = %d \n", pid, i, least, leastPos);
				}
			
		}

		//printf("Before IF - Process %d found Least = %d and leastPos = %d \n", pid, least, leastPos);
		
		/************************************
		Once we done with calculation, we need to broacast those value with all processor, 
		so each processor will calculate their own minimum value and store in one buffer alled 'value' . 
		We need that later, to broadcast among all processor
		************************************/
		value[0] = least;
		value[1] = leastPos;
		
		/***************************************
		Here, we are comparing minimum value (above calculated value) from all processor. 
		and if we found minimum value, we are going to store in variable. For this, 
		all processor (except pid = 0) will share calculated minimum value to the Pid=0 and 
		this pid=0 will compare all processor's minimum value and store in variable.
		***************************************/
		if(pid == 0)
		{
			//printf("Receiving data from all process \n");
			
			for(int i=1;i<num_process;i++)
			{
				MPI_Recv(val, 2, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
				if(val[0] < value[0])
				{
					value[0] = val[0];
					value[1] = val[1];
				}
				//printf("Data received from %d is %d and %d \n", i, val[0], val[1]);
			}
		}
		else
		{
			MPI_Send(value, 2, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		
		
		/******
		once we done with comparison, we are going to broadcast this value to all processor which keeps them updated for further calculation
		*****/
		MPI_Bcast(value, 2, MPI_INT, 0,MPI_COMM_WORLD);
		
		//printf("Process %d - Final Least - %d, and LeastPos - %d \n", pid, least, leastPos);
		
		/***********
		update found value with calculated index as 1 and with its value in distance array
		*****************/
		found[value[1]] = 1;
		distance[value[1]] = value[0];
		
		MPI_Bcast(found, n, MPI_INT, 0,MPI_COMM_WORLD);

		/*******************************
		Now, we need to check our calculated value is minimum than current distance value, if it is, 
		then we wll add minimum value with our edge value. Here, I have divided processes by their index values and incrment them 
		with num_process value which helps me to divide data among all processor.
		****************************/
		for(int i=pid; i<n; i=i+num_process) 
		{
			if( (found[i] == 0) )
			{
				if(distance[i] > value[0]+edge[value[1]][i])
				{
					//printf("In IF - Found -  distance = %d and least+edge is  - %d and leastPos is -  %d \n", distance[i], least+edge[leastPos][i], leastPos);
					distance[i] = value[0]+edge[value[1]][i];
					//printf("Found path %d \n", distance[i]);
				}
			}
		}
	}

} 

int main(int argc, char ** argv) 
{
	clock_t begin = clock();	//Use TIme library to calculate how much time will be taken by our code; its begining time of our code

	int source = 0;
	//value for edges 
	edge[0][0] = 0; edge[0][1] = 2; edge[0][2] = 1; edge[0][3] = 4; edge[0][4] = 5; edge[0][5] = 6; 
  	edge[1][0] = 2; edge[1][1] = 0; edge[1][2] = 5; edge[1][3] = 5; edge[1][4] = 9; edge[1][5] = 7;
  	edge[2][0] = 1; edge[2][1] = 5; edge[2][2] = 0; edge[2][3] = 3; edge[2][4] = 2; edge[2][5] = 11;
  	edge[3][0] = 4; edge[3][1] = 5; edge[3][2] = 3; edge[3][3] = 0; edge[3][4] = 1; edge[3][5] = 6;
	edge[4][0] = 5; edge[4][1] = 9; edge[4][2] = 2; edge[4][3] = 1; edge[4][4] = 0; edge[4][5] = 4;
	edge[5][0] = 6; edge[5][1] = 7; edge[5][2] = 11; edge[5][3] = 6; edge[5][4] = 4; edge[5][5] = 0;
	
	//Initialize MPI
	MPI_Init(&argc, &argv);
	
	//fetch num_process and process id
	MPI_Comm_size(MPI_COMM_WORLD, &num_process);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	//call dijkstra function to calculate distance
  	dijkstra(source);


	//at the end, we are going to print those value using pid=0
	if(pid == 0)
	{
		printf("Calculated Distance \n");
		for(int i=0;i<n;i++)
		{
			printf("%d \n", distance[i]);
		}
	}	
	
	//end of MPI execution
	MPI_Finalize();
	
	clock_t end = clock();    //Use TIme library to calculate how much time will be taken by our code; its ending time of our code
	
	printf("\n");
	
	// Total time taken by processes, on output we will get 2 total time per process
	printf("Total Time - %f seconds \n", (double)(end - begin) / CLOCKS_PER_SEC);    

	return 0;
}