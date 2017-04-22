
#pragma once 
#include <iostream> 
#include <cassert> 
#include <cmath>
using namespace std;
enum ReplacementPolicy {
	FIFO, LRU
};
class virtualMemoryManagerInterface {
public:
	virtualMemoryManagerInterface(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA) 
		: policy(p), N(pS), numFrames(nF), virtualAddressSpaceSize(vA) {
		assert(virtualAddressSpaceSize > N); 
		assert(pow(2, virtualAddressSpaceSize) > numFrames * pow(2, N));
	} 
	/** This is the method the test bench will drive in emulating memory management.
	  * Your memory manager should return the physical address corresponding to the given virtual
	  * address. This function must NOT return until any page swapping is completed, if necessary.
	  * This function is to effect page swaps by calling the other key function (defined below)
	  */ 
	virtual unsigned long long memoryAccess(unsigned long long address) = 0;
	  /** This is the method your memory manager should call to swap pages. 
	  * This function has been instrumented by the TAs to report memory system performance.
	  * @param frameNumber the physical frame to write to swap file (write is emulated)
	  * @param pageNumber the (virtual) page number to read from swap file into the given frame
	  */ 
	void swap(unsigned int frameNumber, unsigned int pageNumber) {
	  ///@todo ta instrumentation to go here 
	  ///students should instrument memorymanager performance in their own class for their internal verification or may
	  ///modify this code for their testing purpose; however the TAs instrumentation will used for determining grade basis.
			numSwaps++;
	} 
	/*
	* Report the to-date number of page swaps, used by test benches to quantify performance.
	* @returns the number of page swaps 
	*/ 
	unsigned long long& numberPageSwaps() { return numSwaps; }
protected:
	ReplacementPolicy policy; 
	unsigned int N; /// N = Page Address bits size, 2^ N = The physical frame / virtual page size to manage(bytes)
	unsigned int numFrames; /// total number of physical frames. Phy mem size = numFrames * 2 ^ N
	unsigned int virtualAddressSpaceSize; ///  2^virtualAddressSpaceSize = total virtual address space(bytes)
	unsigned long long numSwaps = 0; ///total number of page swaps performed
};
