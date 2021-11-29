#include<stdio.h>
#include<mpi.h>

#define c1 1.23456
#define c2 6.54321

#define n 4
double f[n];
double x[n];

int pid;
int np;


double sgn(double x)
{
	if(x < 0.0)
	{
		return -1.0;
	}
	else
	{
		return 1.0;
	}
}

void calcForce(int i) 
{
  int j;
  double diff, tmp;

	// Calculate Force Particle value from 0 to i
	for(j=0; j<i; j++) 
	{
		diff = x[i] - x[j];
   	tmp = c1/(diff*diff*diff) - c2*sgn(x[j])/(diff*diff) ;
 		// After calculating value, Broadcast those values to all processor to keep them up to date.
   	f[i] += tmp;
   	MPI_Bcast(f, n, MPI_INT, 0, MPI_COMM_WORLD);
   	// After calculating value, Broadcast those values to all processor to keep them up to date.
   	f[j] -= tmp;
   	MPI_Bcast(f, n, MPI_INT, 0, MPI_COMM_WORLD);
	}
}

int main(int argc, char ** argv)
{
	x[0] = 10; x[1] = 20; x[2] = 30; x[3] = 40;
	f[0] = 0; f[1] = 0; f[2] = 0; f[3] = 0;
	
	// Initialize mpi init to get number of processor
	MPI_Init(&argc,&argv);
	// Get rank for each processor for mpi comm world
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	// Get number of processor available in mpi comm world
	MPI_Comm_size(MPI_COMM_WORLD, &np);
	
	// Assign F array to 0.0 if PID is 0
	if(pid == 0)
	{
		for(int i=0; i<n; i++)
  	{
  		f[i] = 0.0;
  	}
	}

	// Allocate each element of input particle to each processor and calculate value
	for(int i=pid; i<n; i=i+np)
	{
		calcForce(i);
	}
	
	// Print F value
	if(pid == 0)
	{
   for(int i=0; i<n; i++)
   {
   	printf("%f \n", f[i]);
   }
	}

	// Finish MPI Execution
	MPI_Finalize();
	
	return 0;
}
