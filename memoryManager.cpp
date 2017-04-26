#include "memoryManager.hpp"
using namespace std;
// need to get the info for mem

int currentFifoFrame = 0;
int counter = -1;

unsigned long long memoryManager::memoryAccess(unsigned long long mem) {
	counter++;

	// find the page corresponding to logical address mem
	unsigned int pageNumber = numPages;
	for (unsigned int i = 0; i < numPages; i++) {
		if (mem <= virtualPages[i].endAddr) {
			pageNumber = i;
			break; // breaks when logical address mem is within page's range
		}
	}
	if (pageNumber == numPages) { // no page contains this address
		cout << "Invalid logical memory address\n";
		return -1;
	}

	// used to convert from virtual to physical address
	int memOffsetFromPageStart = mem - virtualPages[pageNumber].startAddr;

	unsigned long long* taken;
	taken = new unsigned long long[numFrames];

	/* second need a way to swap pos with elements depending on the replacement policy */
	if (policy == LRU) {
		//check if logical address mem is already in physical memory
		for (unsigned int frameNumber = 0; frameNumber < numFrames - freeFrames; frameNumber++) {
			if (physicalFrames[frameNumber].startAddr <= mem && mem <= physicalFrames[frameNumber].endAddr) { // found
				recent[frameNumber] = counter;
				return physicalFrames[frameNumber].startAddr + memOffsetFromPageStart;
			}
		}
		// not found in physical memory

		if (freeFrames == 0) { // need to swap
			// need to get the lru
			unsigned int smallest = counter;
			unsigned int lruFrameNumber = -1;
			for (unsigned int y = 0; y < numFrames; y++) {
				if (recent[y] < smallest) {
					smallest = recent[y];
					lruFrameNumber = y;
				}
			} // have lru frame and its frame number now
			physicalFrames[lruFrameNumber] = virtualPages[pageNumber];
			recent[lruFrameNumber] = counter;
			swap(lruFrameNumber, pageNumber);
			return physicalFrames[lruFrameNumber].startAddr + memOffsetFromPageStart;
		}

		// dont need to swap but remaining must decrement by one
		physicalFrames[numFrames - freeFrames] = virtualPages[pageNumber];
		recent[numFrames - freeFrames] = counter;
		freeFrames--;
		return physicalFrames[numFrames - (freeFrames + 1)].startAddr + memOffsetFromPageStart;

	}
	else if (policy == FIFO) {
		//check if logical address mem is already in physical memory
		for (unsigned int frameNumber = 0; frameNumber < numFrames - freeFrames; frameNumber++) {
			if (physicalFrames[frameNumber].startAddr <= mem && mem <= physicalFrames[frameNumber].endAddr) { // found
				// dont need to update pos since its only updated when it is inserted
				return physicalFrames[frameNumber].startAddr + memOffsetFromPageStart;
			}
		}

		if (freeFrames == 0) { // need to swap
			// find the oldest frame index to swap with
			if (currentFifoFrame == numFrames) {
				currentFifoFrame = 0; // min has done a full recycle so must be reset now
			}
			physicalFrames[currentFifoFrame] = virtualPages[pageNumber];
			pos[currentFifoFrame] = counter;
			swap(currentFifoFrame, pageNumber);
			currentFifoFrame++;
			return physicalFrames[currentFifoFrame - 1].startAddr + memOffsetFromPageStart;
		}

		// dont need to swap but remaining must decrement by one
		physicalFrames[numFrames - freeFrames] = virtualPages[pageNumber];
		pos[numFrames - freeFrames] = counter;
		freeFrames--;
		return physicalFrames[numFrames - (freeFrames + 1)].startAddr + memOffsetFromPageStart;
	}
	else {
		cout << "Invalid Policy\n";
		return -1;
	}

}


int main() {
	// just a sample object to make sure things are being constructed alright
	virtualMemoryManagerInterface * memManager = new memoryManager(FIFO, 8, 5, 16);

	delete memManager;

	return 0;
}

