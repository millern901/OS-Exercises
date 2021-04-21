#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <algorithm>

int main(int argc, char **argv) {
	// declare global child process ids and statuses
	pid_t child1_pid, child2_pid;
	int child1_stat, child2_stat;

	// declare, open, and validate pipes
	int p_c1_pipe[2], c1_p_pipe[2], c2_p_pipe[2], c1_c2_pipe[2];
	if (pipe(p_c1_pipe) == -1 || pipe(c1_p_pipe) == -1 || pipe(c2_p_pipe) == -1 || pipe(c1_c2_pipe) == -1) 
	{
		fprintf(stderr, "Pipe openings failed.\n");
        	return EXIT_FAILURE;
	}

	// fork child 1 process
	child1_pid = fork();
	if (child1_pid < 0) // forking fail
	{
		fprintf(stderr, "Child 1 forking failed.\n");
		return EXIT_FAILURE;
	} 
	else if (child1_pid == 0) // child 1 process
	{
		// close unused pipes
		close(p_c1_pipe[1]); // close p->c1 W
		close(c1_p_pipe[0]); // close c1->p R
		close(c1_c2_pipe[0]); // close c1->c2 R
		close(c2_p_pipe[0]); // close c2->p R
		close(c2_p_pipe[1]); // close c2->p W

		// read integer list written by parent
		int child1_list[5];
		read(p_c1_pipe[0], child1_list, sizeof(child1_list));

		// sort integer list
		int child1_list_length = sizeof(child1_list) / sizeof(child1_list[0]);
		std::sort(child1_list, child1_list + child1_list_length);

		// write sorted integer list to parent and child 2
		write(c1_p_pipe[1], child1_list, sizeof(child1_list));
		write(c1_c2_pipe[1], child1_list, sizeof(child1_list));

		// close remaining pipes
		close(p_c1_pipe[0]); // close p->c1 R
		close(c1_p_pipe[1]); // close c1->p W
		close(c1_c2_pipe[1]); // close c1->c2 W

		// exit child 1 process
		exit(EXIT_SUCCESS);
	}
	else if (child1_pid > 0) // parent process
	{
		// close unused pipes
		close(p_c1_pipe[0]); // close p->c1 R
		close(c1_p_pipe[1]); // close c1->p W
		close(c1_c2_pipe[1]); // close c1->c2 W

		// grab integer list from arguments and write it to child 1
		int arg_list[] = {atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5])};
		write(p_c1_pipe[1], arg_list, sizeof(arg_list));

		// wait for child 1 process to finish
		waitpid(child1_pid, &child1_stat, 0);

		// read sorted integer list written by child 1
		int sorted_list[5];
		read(c1_p_pipe[0], sorted_list, sizeof(sorted_list));

		// print sorted integer list
		printf("The sorted list is: ");
		for (int i = 0; i < 5; i++) {
			if (i == 4) {
				printf("%d\n", sorted_list[i]);
			} else {
				printf("%d ", sorted_list[i]);
			}
		}

		// fork child 2 process
		child2_pid = fork();
		if (child2_pid < 0) // forking fail
		{
			fprintf(stderr, "Child 2 forking failed.\n");
			return EXIT_FAILURE;
		}
		else if (child2_pid == 0) // child 2 process
		{
			// close unused pipes
			close(p_c1_pipe[1]); // close p->c1 W
			close(p_c1_pipe[0]); // close p->c1 R
			close(c1_p_pipe[1]); // close c1->p W
			close(c1_p_pipe[0]); // close c1->p R
			close(c2_p_pipe[0]); // close c2->p R
			close(c1_c2_pipe[1]); // close c1->c2 W

			// read sorted integer list from child 1
			int child2_list[5];
			read(c1_c2_pipe[0], child2_list, sizeof(child2_list));

			// grab median and write it to parent
			int child2_median = child2_list[2];
			write(c2_p_pipe[1], &child2_median, sizeof(child2_median));

			// close remaining pipes
			close(c2_p_pipe[1]); // close c2->p W
			close(c1_c2_pipe[0]); // close c1->c2 R

			// exit child 2 process
			exit(EXIT_SUCCESS);
		}
	
		// wait for child 2 process to finish
		waitpid(child2_pid, &child2_stat, 0);

		// read median written by child 2
		int parent_median;
		read(c2_p_pipe[0], &parent_median, sizeof(parent_median));

		// print median
		printf("The median is: %d\n", parent_median);

		// close remaining pipes
		close(c2_p_pipe[1]); // close c2->p W
		close(p_c1_pipe[1]); // close p->c1 W
		close(c1_p_pipe[0]); // close c1->p R
		close(c1_c2_pipe[0]); // close c1->c2 R
		close(c2_p_pipe[0]); // close c2->p R
	}
	return EXIT_SUCCESS;
}
