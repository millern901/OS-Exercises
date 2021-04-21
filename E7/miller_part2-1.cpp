#include <stdio.h>
#include <string.h>
#include <cstdlib>

int main() {
	// buffer and word counts
	char output[64];
	int op_cnt = 0;

	// loop through buffer until nothing is found
	while (scanf("%s", output) != -1) {
		// determine if the buffer reads success or fail and increment the counter
		if (strcmp(output, "successful.") == 0 || strcmp(output, "failed.") == 0) {
			op_cnt++;
		}
	}

	// print the final count
	printf("Program failed on operation %d\n", op_cnt);
	return 0;
}
