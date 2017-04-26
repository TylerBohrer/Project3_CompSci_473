#pragma once
#include <iostream> 
#include <cassert> 
#include <cmath>
#include "virtualMemoryManagerInterface.hpp"

class VirtualMemoryManager : public virtualMemoryManagerInterface {
public:
	VirtualMemoryManager(ReplacementPolicy p, unsigned int pS, unsigned int nF, unsigned int vA)
		: virtualMemoryManagerInterface(p, pS, nF, vA) {
	}

	unsigned long long memoryAccess(unsigned long long address);

};