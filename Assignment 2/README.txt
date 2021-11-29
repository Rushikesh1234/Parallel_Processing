Hi Professor, 

I have tried to get solution. Its working fine, it gives us respective output. 

I split the taska into 2 list, then i again spliit the processor, till it reach to 1.
Wheni am splitting task, simultenpusly, i am sending data from one processor from list to another set from second list.

Consider, we have 8 processoor (P0,P1,P2,P3,P4,P5,P6,P7), I am splitting those process into half (4 ((P0,P1,P2,P3) and 4 (P4,P5,P6,P7)).
Then I am sending data from P0 to P4, P1 to P5, in the same way for others.
Once it is completed, I am spliting first list only (P0,P1,P2,P3) in 2 section - (P0,P1) and  (P2,P3). 
And, I am sending data from P0 to P2 and P1 to P3. 
At the end, I am spliting data again from (P0,P1), now, this time i am sending data from P0 to P1.

For Receiving, I am applying same logic, just instead of sendng, i am doing receiving of the data.

I have attached code file and screenshot. I hope it gives you expected answer. If not, I apologize then, I'll try to get perfect solution for next time. 

Thanks for sharing such a nice problem.
