#pragma once
#include <iostream> 
#include <cassert> 
#include <cmath>
#include "virtualMemoryManagerInterface.hpp"

class memoryManager : public virtualMemoryManagerInterface {
private:
	unsigned long long pageSize;
	unsigned long long physicalMemTotalSize;
	unsigned long long virtualMemTotalSize;
	unsigned int numPages;
	struct Page {
		// both inclusive
		unsigned long long startAddr;
		unsigned long long endAddr;
	};
	Page* virtualPages;
	Page* physicalFrames;
	int freeFrames;
	unsigned int* recent; // used in LRU
	unsigned int* pos; // used in FIFO

public:
	memoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA)
		: virtualMemoryManagerInterface(p, pS, nF, vA) {
		pageSize = pow(2, N);
		physicalMemTotalSize = numFrames * pageSize;
		virtualMemTotalSize = pow(2, virtualAddressSpaceSize);
		numPages = virtualMemTotalSize / virtualAddressSpaceSize;

		physicalFrames = new Page[numFrames];
		virtualPages = new Page[numPages];
		for (unsigned int i = 0; i < numPages; i++) {
			virtualPages[i].startAddr = i * pageSize;
			virtualPages[i].endAddr = (i + 1) * pageSize - 1;
		}
		
		freeFrames = numFrames;
		recent = new unsigned int[numFrames];
		pos = new unsigned int[numFrames];
	}

	unsigned long long memoryAccess(unsigned long long address);
};