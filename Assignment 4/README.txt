Hi Professor,

I am able to solve the problem. It is a tough problem to solve but very nice to solve. I have attached code for both serial and parallel mode with screenshot.

I have divided matrix value rowwise and broadcast array length to each processor for further calculation. Then iterate through matrix for each k-iteration. I am executing a loop for number of processor which helps us to decide which processor will work on which row value from matrix. After dividing data to processor, I am broadcasting that data to all processor for further calculation. Then each process will calculate data (check minimum value from row), after calculation, we will broadcast the data to all process. At the end, processor id 0 will print all calculated data.

I have attached code and screenshot in the file. And, I have mentioned comments for each line in the code.

