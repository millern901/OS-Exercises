// c++ packages
#include <thread>
#include <vector>
#include <iostream>

// find the number function
void find_number(int thread_id, int target) {
	// make an initial guess and loop until the target is found
	int guess = rand() % 10000;
	while (guess != target) {
		guess = rand() % 10000;
	}
	printf("Thread %d completed.\n", thread_id);
}

// thread race function
void race_to_finish(int target) {
	// initialize all threads
	std::vector<std::thread> threads;
	for (int i = 0; i < 10; i++) {
		threads.push_back(std::thread(find_number, i, target));
	}
	// join all threads
	for (std::thread& t: threads) {
		t.join();
	}
	printf("All threads have finished finding numbers.\n");
}

// main functions with terminal arguments
int main(int argc, char **argv) {
	// call the thread race with integer argument
	race_to_finish(atoi(argv[1]));
	return 0;
}
