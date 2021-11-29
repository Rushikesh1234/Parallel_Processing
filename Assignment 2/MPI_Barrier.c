#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


void main(int argc, char** argv)
{
	    int i, j, pid, np, mtag, count = 0, source, destination;
	    int data[1];
	    MPI_Status status;
	    MPI_Request req_s, req_r;

	    MPI_Init(&argc, &argv);
	    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	    MPI_Comm_size(MPI_COMM_WORLD, &np);
	    count = np;
	    
	    while(count > 1 && pid < count)					//divide processor into half and send data to other process and send values to other process
	    {
			  if (pid < count / 2) 		
			  {
				mtag = 1;
				source = (count / 2) + pid;
				MPI_Recv(data, 1, MPI_INT, source, mtag, MPI_COMM_WORLD, &status);
				printf("Process %d received data from %d \n", pid, source);
			  } 
			  else 
			  {
				mtag = 1;
				destination = (pid - count/2);
				MPI_Send(data, 1, MPI_INT, destination, mtag, MPI_COMM_WORLD);
			  }
			  count = count / 2;
	    }
	    
	    count = 2;
	    while(count <= np)							//divide processor into half and send data to other process and receive values to other process
	    {
		  if (pid < count / 2 && pid < count) 
		  {
		      mtag = 1;
		      destination = (count / 2) + pid;
		      MPI_Send(data, 1, MPI_INT, destination, mtag, MPI_COMM_WORLD);
		  } 
		  else if(pid < count)
		  {
		      mtag = 1;
		      source = (pid - count/2);
		      MPI_Recv(data, 1, MPI_INT, source, mtag, MPI_COMM_WORLD, &status);
		      printf("Process %d has received from %d.\n", pid, source);
		  }
		  count = count * 2;
	    }

	    MPI_Finalize();

} 
