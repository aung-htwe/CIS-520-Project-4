#include <p.thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 20
#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 2001

int max_values[MAX_LINES];

/// Struct used for determining which lines to read for a thread (starting line to ending line)
typedef struct {
	int fd;
	int start;
	int end;
	long taskid;
} ThreadLocation;

/// Starting routine for a thread to calculate the max ASCII value of a line
/// \param arg The sole argument passed to the starting routine
/// \return nothing
void *calculate_lines(void *thread_arg){
	int i, j;
	int max;

	char* line = (char*) malloc (MAX_LINE_LENGTH);	// no lines larger than 2000 chars

	ThreadLocation *thread_location = (ThreadLocation *) thread_arg;
	start = thread_location->start;
	end = thread_location->end;

	while (start != end){
		fscanf(fd, "%[^\n]\n", line);

	}


}

/// Main program that implements parallel programming with threads
int main(){
	// open file
        FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");

	// declare threads and ids
	pthread_t threads[NUM_THREADS];

	// return code for error checking
	int rc;

	// declare thread locations to evenly split up workload. Each thread location has a start and end int for the lines they will calculate
	ThreadLocation thread_locations[NUM_THREADS];

	// each thread calculates an even_partition number of lines
	int even_partition = MAX_LINES / NUM_THREADS;

	for (int j = 0; j < NUM_THREADS; j++){
		thread_locations[j].start = j * even_partition;
		thread_locations[j].end = (j + 1) * even_partition - 1;
		thread_locations[j].fd = fd;
	}


	for (int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&threads[i], NULL, calculate_lines, (void *) &thread_locations[i]);

	}


}
