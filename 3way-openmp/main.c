#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 3000


/// Function to calculate the max ASCII value of a given string
/// \param str The line passed in to calculate
/// \return the max ASCII value
int max_ascii(const char* str){
	int max = 0;

	for (int i = 0; str[i] != '\0'; i++){
		if ((unsigned char)str[i] > max)
			max = (unsigned char)str[i];
	}

	return max;
}



/// Main program that implements parallel programming with OpenMP
int main(){
	// Open file
	FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
	if (fd == NULL)
	{
		fprintf(stderr, "ERROR: File not read\n");
		return 1;
	}



	// Initialize array to store max ascii values
	int* max_values = (int*) malloc(MAX_LINES * sizeof(int));
	if (max_values == NULL){
		fprintf(stderr, "Failed to allocate max_values\n");
		return 1;
	}



	// array to hold all lines from file
	char **lines = (char**)malloc(MAX_LINES * sizeof(char*));
	if (lines == NULL){
		printf("ERROR: Failed to allocate memory for lines\n");
		return 1;
	}

	for (int i = 0; i < MAX_LINES; i++){
		lines[i] = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
		if (lines[i] == NULL){
			printf("ERROR: Failed to allocate memory for lines\n");
			return 1;
		}
	}


	// Read all lines into array
	int total_lines = 0;
	while (total_lines < MAX_LINES && fgets(lines[total_lines], MAX_LINE_LENGTH, fd) != NULL){
		lines[total_lines][strcspn(lines[total_lines], "\n")] = '\0';
		total_lines++;
	}
	fclose(fd);



	// Get number of threads and print
	int num_threads = omp_get_num_threads();
	fprintf(stdout, "Number of threads: %d\n", num_threads);

	int partition = MAX_LINES / num_threads;

	// ID of thread
	int thread_id;



	#pragma omp parallel private(thread_id)
	{
		// grab thread_id & print
		thread_id = omp_get_thread_num();
		fprintf(stdout, "Initializing thread %d\n", thread_id);


		int i, max;

		// Run the for loop in parallel that calculates the max ascii of each line
		// No need for synchronization
		#pragma omp for schedule(static, partition)
		for (i = 0; i < total_lines; i++){ //for (i = thread_id * partition; i < (thread_id + 1) * partition - 1; i++){
			max = max_ascii(lines[i]);
			max_values[i] = max;
		}
	}


	// All threads merge, print out first 100 lines
	for (int i = 0; i < 100; i++)
		fprintf(stdout, "Line %d max ASCII: %d\n", i, max_values[i]);




	for (int i = 0; i < MAX_LINES; i++)
		free(lines[i]);

	free(max_values);
	free(lines);

}
