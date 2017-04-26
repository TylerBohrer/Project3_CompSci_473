#include <iostream> 
#include <cassert> 
#include <cmath>
#include "virtualMemoryManagerInterface.hpp"
using namespace std;
// need to get the info for mem

int min = 0;
int counter = -1;

void virtualMemoryManagerInterface::swap(unsigned int frameNumber, unsigned int pageNumber)
{

	//not going to use bc why not
	// just going to call this every time a swap takes place instead


}
unsigned long long virtualMemoryManagerInterface::memoryAccess(unsigned long long mem)
{ // basically we need to implement what I did in here in a seperate function or keep it here if it works
	counter += 1;
	ReplacementPolicy type = virtualMemoryManagerInterface::policy; // policy
	unsigned int framesAmount = virtualMemoryManagerInterface::numFrames; // num of frames
	unsigned int n = virtualMemoryManagerInterface::N; // N = page address bit size... 2^ N = The physical frame / virtual page size to manage(bytes) 
	unsigned int virt = virtualMemoryManagerInterface::virtualAddressSpaceSize; // 2^virtualAddressSpaceSize = total virtual address space(bytes)
	int space = framesAmount; // max amount of frames that can be stored
	//int v_space = pow(2, virt);
	int remaining = space;
	int * pos; // used in FIFO
	pos = new int[space];
	int * recent; // used in LRU
	unsigned long long * taken;
	//int p_space = framesAmount *pow(2, n); // the total size of the physical memory space
	taken = new unsigned long long[space];
	recent = new int[space]; // set the size to the max number of elements that can be used for this
	// first need to set up way to keep track what is in the memory

	// Second need a way to swap pos with elements depending on the replacement policy
	if (type == LRU) {
		//check if it is init
		bool found = false;
		int x = 0;
		for (x = 0; x < space - remaining; x++)
		{
			if (taken[x] == mem) { break; }
		}
		if (found) 
		{
			recent[x] = counter;
			return x;
		
		}
		else { // will need to update this to be the mapping between vm and pm... rn have virtual memory lru and such will need to convert to physical memory and such
			if (remaining == 0)
			{//need to swap
				// need to get the lru
				int y = 0;
				int smallest = counter;
				int spos = -1;
				for (y = 0; y < space; y++) 
				{
					if (recent[y] < smallest) 
					{
						smallest = recent[y];
						spos = y;
					}
				
				}// have smallest and its pos now
				taken[spos] = mem;
				recent[spos] = counter;
				swap(mem, mem);
				return spos;
			}
			else
			{//dont need to swap but remaining must decrement by one
				taken[space - remaining] = mem;
				recent[space - remaining] = counter;
				remaining -= 1;
				return space - remaining + 1;

			}
		}
	
	}
	else if (type == FIFO) 
	{
		//check if it is init
		bool found = false;
		int x = 0;
		for (x = 0; x < space - remaining; x++)
		{
			if (taken[x] == mem) { break; }
		}
		if (found)
		{// dont need to update pos since its only updated when it is inserted
			return x;
		}
		else {
			if (remaining == 0)
			{//need to swap
				//find the min in
				if (min > space - 1) {min = 0;} // min has done a full recycle so must be reset now
				int spot = min;
				taken[min] = 0;
				min += 1;
				taken[min - 1] = mem;
				pos[min - 1] = counter;
				swap(mem,mem);
				return min-1;
			}
			else
			{//dont need to swap but remaining must decrement by one
				taken[space - remaining] = mem;
				pos[space - remaining] = counter;
				remaining -= 1;
				return space - remaining + 1;
			}
		}
	}
	else {
		cout << "There was an error on the policy\n";
		return -1;
	}

}


int main() 
{

	// as of now am returning the virtual memory address need to return the phyiscal memory address\
	// lru and fifo are good so far


	return 0;
}