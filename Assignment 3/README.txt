Hi Professor,

I have tried to get solution. Its working fine, it gives us respective output.

I have tried to divide the tasks for each processor iteratively. I kept inner for loop (one which is used for calculating minimum least value and to get leasPos value) as it is mentioned by you. I just updated their index value. Initially, it was caculating from index 0 and incrmenting index by 1. I changed index to Pid value and they will increment by processor number.
Example -
earlier - Processor 0 - 0 1 2 3 4 5
now - Processor 0 - 0 2 4 and Processor 1 - 1 3 5

Then, i am calculating minimum least value and get leastPos.

After calculation, All processor (Except 0) will share their own calculated value (least and leastPos) to Processor 0. Then, processor 0 will check minimum value from all processor and pick a minimum one. And, then broadcast this value to all processor in order to keep update among all processor. 

Then, we need to update those value in distance and update found value to 1.

To select this minimum value as our final value, we need to compare this value with respective distance value. For this, in a loop, i keep index as their pid value and increment index with num_process. Then compare distance value with our minimum value and edge value. If it minimum, we will update our distance array, else we will check for other value.

After calculation, at the end, if pid=0, we will print final calculated distance value.

To showcase, time execution, I used time library also, it gives time usage by each processor.

Thanks for giving such a problem which makes me to think on differnet possibility of the problem.
