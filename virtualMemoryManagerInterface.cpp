#include <iostream> 
#include <cassert> 
#include <cmath>
#include "virtualMemoryManagerInterface.hpp"
#include "VirtualMemoryManager.hpp"
using namespace std;
// need to get the info for mem

int min = 0;
int counter = -1;
class memoryManager : public virtualMemoryManagerInterface {
	public:
		memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA)
			: virtualMemoryManagerInterface(p, pS, nF, vA) {
		}
		ReplacementPolicy type = virtualMemoryManagerInterface::policy; // policy
		unsigned int framesAmount = virtualMemoryManagerInterface::numFrames; // num of frames
		unsigned int n = virtualMemoryManagerInterface::N; // N = page address bit size... 2^ N = The physical frame / virtual page size to manage(bytes) 
		unsigned int virt = virtualMemoryManagerInterface::virtualAddressSpaceSize; // 2^virtualAddressSpaceSize = total virtual address space(bytes)
		unsigned long long worker(unsigned long long);
		virtual unsigned long long memoryAccess(unsigned long long address) 
		{
			unsigned long long place = worker(address);
			return place;
		}



};
int remaining = 0;
	
int v_space = pow(2, virt);
	//static int remaining = space;
int * pos; // used in FIFO
static int * recent; // used in LRU
unsigned long long * taken;//int p_space = framesAmount *pow(2, n); // the total size of the physical memory space
pos = new int[space];
taken = new unsigned long long[space]; // this needs to change it is making a new one every run
recent = new int[space]; // set the size to the max number of elements that can be used for this
unsigned long long memoryManager::worker(unsigned long long mem)
{
	counter += 1;
	int space = framesAmount; // max amount of frames that can be stored
	if (type == LRU) {
		//check if it is init
		bool found = false;
		int x = 0;
		for (x = 0; x < space; x++)
		{
			if (taken[x] == mem) { found = true; break; }
		}
		if (found)
		{
			recent[x] = counter;
			return x;

		}
		else { // will need to update this to be the mapping between vm and pm... rn have virtual memory lru and such will need to convert to physical memory and such
			if (remaining == space)
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
				swap(0, 0);
				return spos;
			}
			else
			{//dont need to swap but remaining must decrement by one
				taken[space - remaining] = mem;
				recent[space - remaining] = counter;
				remaining += 1;
				return space - remaining + 1;

			}
		}

	}
	else if (type == FIFO)
	{
		//check if it is init
		bool found = false;
		int x = 0;
		for (x = 0; x < space; x++)
		{
			cout << taken[x] << " : " << mem << endl;
			if (taken[x] == mem) { found = true; break; }
		}
		if (found)
		{// dont need to update pos since its only updated when it is inserted
			cout << "it was found" << endl;
			return x;
		}
		else {
			if (remaining == space)
			{//need to swap
			 //find the min in
				cout << "it was swapped" << endl;
				if (min > space - 1) { min = 0; } // min has done a full recycle so must be reset now
				int spot = min;
				taken[min] = 0;
				min += 1;
				taken[min - 1] = mem;
				pos[min - 1] = counter;
				swap(0, 0);
				return min - 1;
			}
			else
			{//dont need to swap but remaining must decrement by one
				cout << "There was room" << endl;
				cout << space << " : " << remaining << endl;
				taken[space - remaining] = mem;
				pos[space - remaining] = counter;
				remaining += 1;
				return space - remaining + 1;
			}
		}
	}
	else {
		cout << "There was an error on the policy\n";
		return -1;
	}


}
/*oiganally used moving on to another way to implement it... will take this completely out once getting code to run better
unsigned long long virtualMemoryManagerInterface::memoryAccess(unsigned long long mem)
{
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
*/

int main() 
{// fixed the errors with 
	virtualMemoryManagerInterface * memManager = new memoryManager(FIFO, 8, 5, 16);
	unsigned long long help1 = 10;
	unsigned long long help2 = 110;
	unsigned long long help3 = 1110;
	unsigned long long help4 = 11110;
	unsigned long long help5 = 111110;
	unsigned long long help6 = 1111110;
	cout<<memManager->memoryAccess(help1)<<endl;
	cout<<memManager->memoryAccess(help2) << endl;
	cout << memManager->memoryAccess(help3) << endl;
	cout << memManager->memoryAccess(help4) << endl;
	cout << memManager->memoryAccess(help5) << endl;
	cout << memManager->memoryAccess(help6) << endl;
	cout << memManager->memoryAccess(help6) << endl;
	cout << memManager->numberPageSwaps() << endl;

	delete memManager;
	int tesst = 0; // for some reason window closes automatic after it has done run through so I added this to stop it will look into removing
	cin >> tesst;

	return 0;
}