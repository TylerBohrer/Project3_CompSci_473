# Project3_CompSci_473
This is the place for project 3 of compsci 473  
Issues that still need to be done for this project:  
1. TestBench  
2. Return physical mem address not the virtual mem address  
3. Make sure can take a wide range of inputs... should be able to see with a test bench   
4. More if issues come up   
I also emailed a TA to see what else would be needed since their code is pretty vague, here is his response:
I would suggest you (as mentioned in the project description) to make a sample testbench for your self and testing it out on your project, as well as comparing the results for the same with other groups, you can also ask for other group's test bench.

 But after going through your code, I saw these things that you should take care of:

1. The class/.hpp file provided in the project description in an abstract class, and as instructed by Professor, you should not make any changes to it. You are however allowed to extend the class into your own class and use the functions thereon, even adding more functions if that helps.

2. You are managing your virtual memory rather than managing your physical memory. If you read the description, you will see that the input given is a logical/virtual address, for which, your project should return a physical address. The size of physical and virtual memory are very different.  

3. You have to take care of the range of input that the project can take. As I can see, you have type-casted many of the variables in your definition, which may create a problem. 

4. The project will be graded on the final physical address returned and the number of page swaps (both for every input). So your project should be able to keep a track of number of page swaps done till date. Also, if I am not wrong, you are returning the frame number and not the physical address of the corresponding virtual address. 

 Hope these suggestions make sense.

