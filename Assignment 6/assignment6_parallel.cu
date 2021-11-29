#include<stdio.h>
#include<cuda.h>
#include<stdlib.h>
#include<math.h>

#define BLOCK_SIZE 16

float **dist;
int n, w;
float p;

__global__ void calculateDistance(float *dev_dist, size_t pitch, int n)
{
	// Initialize blocksize for each threadIdx to access row value in matrix
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	int j = blockIdx.y * blockDim.y + threadIdx.y;
	
	float *row;
	float a,b;
	float temp;
	
	// Now, we are accessing matrix row value, if i and j is less than row size of matrix
	if(i<n && j<n)
	{
		// Iterating through each array element
		for(int k=0; k<n;k++)
		{
			// Fetch a[k][j] value from our column of matrix
			row = (float*)((char*)dev_dist + k*pitch);
			a = row[j];

			// Fetch a[i][k] value from row of matrix for kth element
			row = (float*)((char*)dev_dist + i*pitch);
			b = row[k];
			
			// If our temp value is less than current postion row[j] (i.e. a[i][j]), then, we store caluclated value or minimum distance value
			temp = a + b;
			if(row[j] > temp)
			{
				row[j] = temp;
			}
		}
	}
}

int main()
{
	n = 8;
	w = 4;
	p = 2;
	
	// Declare array
	dist = (float **) malloc(n * sizeof(float*));
	
	for(int i=0; i<n; i++)
	{
		dist[i] = (float *) malloc(n * sizeof(float));
	}
	
	float rand;
	
	// Generate Matrix Data using Random Function
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			rand = ((float)rand()) / n;
			if(rand != 0)
			{
				dist[i][j] = rand;
			}
		}
		// Diagonal element is zero
		dist[i][i] = 0;
	}
	
	float *dev_dist;
	size_t size;
	
	// Allocate CUDA memory for datatypes
	cudaMallocPitch(&dev_dist, &size, n*sizeof(float), n);
	
	// Copy our input data to CUDA variable
	cudaMemcpy2D(dev_dist, size, dist, n*sizeof(float), n*sizeof(float), n, cudaMemcpyHostToDevice);

	// Fetch threads and blocks for GPU  execution
	dim3 threadsPerBlock(BLOCK_SIZE,BLOCK_SIZE);
	dim3 numBlocks(n / threadsPerBlock.x, n / threadsPerBlock.y);
	
	// Call calculateDistance function which is declared and executed in GPU
	calculateDistance<<<numBlocks, threadsPerBlock>>>(dev_dist, pitch, n);

	// Finally, cpy our output in final variable
	cudaMemcpy2D(dist, n*sizeof(float), dev_dist, pitch, n*sizeof(float), n, cudaMemcpyHostToDevice);
	
	// Free Datatype
	cudaFree(dev_dist);
	
	return 0;
}
