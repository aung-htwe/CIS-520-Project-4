#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 20
#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 2001

char lines[MAX_LINES][MAX_LINE_LENGTH]; // Buffer to store all lines
int max_values[MAX_LINES];

pthread_mutex_t lock;					// Mutex for thread-safe updating

/// Struct used for determining which lines to read for a thread (starting line to ending line)
typedef struct {
	//int fd;
	int start;
	int end;
	long taskid;
} ThreadLocation;

/// Function to calculate the max ASCII value of a given string 
/// \param str The line passed in to calculate
/// \return the max ASCII value
int max_ascii(const char* str){
	int max = 0;

	//some calculation

	return max;
}

/// Starting routine for a thread to calculate the max ASCII value of a line
/// \param arg The sole argument passed to the starting routine
/// \return nothing
void *calculate_lines(void *thread_arg){
	int i, j;
	int max;

	//char* line = (char*) malloc (MAX_LINE_LENGTH);	// no lines larger than 2000 chars

	ThreadLocation *thread_location = (ThreadLocation *) thread_arg;
	int start = thread_location->start;
	int end = thread_location->end;
	/*
	while (start != end){
		fscanf(thread_location->fd, "%[^\n]\n", line);

	}
	*/
	
	for (i = start; i < end; i++) {
		max = max_ascii(lines[i]);

		//Protect access to shared result array
		pthread_mutex_lock(&lock);
		max_values[i] += max; 
		pthread_mutex_unlock(&lock);
  	}

	// terminates the thread
	pthread_exit(NULL);
}

/// Main program that implements parallel programming with threads
int main(){
	// open file
    FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
	if(fd == NULL){
		printf("ERROR; Failed to open file");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_init(&lock, NULL);

	int total_lines = 0;

    // Read all lines into memory
    while (fgets(lines[total_lines], MAX_LINE_LENGTH, fd) != NULL && total_lines < MAX_LINES) {
        // Strip newline if needed
        lines[total_lines][strcspn(lines[total_lines], "\n")] = '\0';
        total_lines++;
    }
    fclose(fd);

	// declare threads and ids
	pthread_t threads[NUM_THREADS];
	
	// return code for error checking
	int rc;

	// declare thread locations to evenly split up workload. Each thread location has a start and end int for the lines they will calculate
	ThreadLocation thread_locations[NUM_THREADS];

	// each thread calculates an even_partition number of lines
	int even_partition = total_lines / NUM_THREADS;

	for (int j = 0; j < NUM_THREADS; j++){
		thread_locations[j].start = j * even_partition;
		thread_locations[j].end = (j + 1) * even_partition - 1;
		//thread_locations[j].fd = fd;
		rc = pthread_create(&threads[j], NULL, calculate_lines, (void *) &thread_locations[j]);
		if (rc) {
	    	printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
	    }

	}

	//condensed
	/*
	for (int i = 0; i < NUM_THREADS; i++){
		rc = pthread_create(&threads[i], NULL, calculate_lines, (void *) &thread_locations[i]);
		if (rc) {
	    	printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
	    }

	}
	*/
	
	//waits until all threads are done
	for (int i = 0; i < NUM_THREADS; i++ ) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	// Print first 10 results as sample
	for (int i = 0; i < 10 && i < total_lines; i++) {
    	printf("Line %d max ASCII: %d\n", i, max_values[i]);
    }


}
