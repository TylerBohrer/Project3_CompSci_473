#include "memoryManager.hpp"
using namespace std;
// need to get the info for mem

int currentFifoFrame = 0;
int counter = -1;

unsigned long long memoryManager::memoryAccess(unsigned long long mem) {
	counter++;

	// find the page corresponding to logical address mem
	unsigned int pageNumber = numPages + 1;
	for (unsigned int i = 0; i < numPages; i++) {
		if (mem <= virtualPages[i].endAddr) {
			pageNumber = i;
			break; // breaks when logical address mem is within page's range
		}
	}
	if (pageNumber == numPages + 1) { // no page contains this address
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
				return frameNumber * pageSize + memOffsetFromPageStart;
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
			return lruFrameNumber * pageSize + memOffsetFromPageStart;
		}

		// dont need to swap but remaining must decrement by one
		physicalFrames[numFrames - freeFrames] = virtualPages[pageNumber];
		recent[numFrames - freeFrames] = counter;
		freeFrames--;
		return (numFrames - (freeFrames + 1)) * pageSize + memOffsetFromPageStart;

	}
	else if (policy == FIFO) {
		//check if logical address mem is already in physical memory
		for (unsigned int frameNumber = 0; frameNumber < numFrames - freeFrames; frameNumber++) {
			if (physicalFrames[frameNumber].startAddr <= mem && mem <= physicalFrames[frameNumber].endAddr) { // found
				// dont need to update pos since its only updated when it is inserted
				return frameNumber * pageSize + memOffsetFromPageStart;
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
			return (currentFifoFrame - 1) * pageSize + memOffsetFromPageStart;
		}

		// dont need to swap but remaining must decrement by one
		physicalFrames[numFrames - freeFrames] = virtualPages[pageNumber];
		pos[numFrames - freeFrames] = counter;
		freeFrames--;
		return (numFrames - (freeFrames + 1)) * pageSize + memOffsetFromPageStart;
	}
	else {
		cout << "Invalid Policy\n";
		return -1;
	}

}


int main() {
	// just a sample object to make sure things are being constructed alright
	virtualMemoryManagerInterface * memManager = new memoryManager(FIFO, 8, 5, 16);

	unsigned long long help1 = 10;
	unsigned long long help2 = 110;
	unsigned long long help3 = 1110;
	unsigned long long help4 = 11110;
	unsigned long long help5 = 111110;
	unsigned long long help6 = 111;

	cout << memManager->memoryAccess(help1) << endl;
	cout << memManager->memoryAccess(help2) << endl;
	cout << memManager->memoryAccess(help3) << endl;
	cout << memManager->memoryAccess(help4) << endl;
	cout << memManager->memoryAccess(help5) << endl;
	cout << memManager->memoryAccess(help6) << endl;

	cout << "PageSwaps: ";
	cout << memManager->numberPageSwaps() << endl;

	delete memManager;

	system("pause");

	return 0;
}

