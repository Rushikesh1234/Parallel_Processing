#include<stdio.h>
#include <stdlib.h>
#define n 10
int input[n][n];

int main()
{
	// Generate data
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			if(i==j)
			{
				input[i][j] = 0;
			}
			else
			{
				int r = rand() % 10;
				if(r != 0)
				{
					input[i][j] = r;
					if(input[j][i] == 0)
					{
						input[j][i] = input[i][j];
					}
				}
			}
		}
	}
	
	printf("Input data \n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			printf("%d ", input[i][j]);
		}
		printf("\n");
	}
}