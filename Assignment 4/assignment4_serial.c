#include<stdio.h>
#include<time.h>

#define infinity 9999

int main()
{
	int n = 4;
	int a[4][4] = {
		{0,5, infinity, 10},
		{infinity, 0, 3, infinity},
		{infinity, infinity, 0, 1},
		{infinity, infinity, infinity, 0}
	};
	int b[4][4];
	
	printf("\nInput\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	
	clock_t begin = clock();
	
	printf("\nAfter calculation \n");
	for(int k=0;k<n;k++)
	{
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(a[i][j] > a[i][k]+a[k][j])
				{
					b[i][j] = a[i][k]+a[k][j];
				}
				else
				{
					b[i][j] = a[i][j];
				}
			}
		}
		
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				a[i][j] = b[i][j];
			}
		}
	}
	
	printf("\nOutput\n");
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			printf("%d ", b[i][j]);
		}
		printf("\n");
	}
	
	clock_t end = clock();
	
	printf("\nTotal Time For Execution - %f \n", (double)(end-begin)/CLOCKS_PER_SEC);
}
