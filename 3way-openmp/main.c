#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
	int max_values[MAX_LINES];

	int thread_id;

	bool flag = false;

	// Open file and calculate max in parallel
	#pragma omp parallel private(thread_id)
	{
		thread_id = omp_get_thread_num();
		fprintf(stdout, "Initializing thread %d\n", thread_id);
		#pragma omp barrier

		FILE *fd = fopen("/homes/dan/625/wiki_dump.txt", "r");
		if (fd == NULL)
		{
			fprintf(stderr, "ERROR: File not read\n");
		}

		char* line = (char*) malloc(MAX_LINE_LENGTH);
		int line_num = 0;

		#pragma omp for schedule(dynamic)
		for (int i = 0; i < MAX_LINES; i++){
			if (flag) continue;

			if (fgets(line, MAX_LINE_LENGTH, fd) == NULL)
				flag = true;

			line[strcspn(line, "\n")] = '\0';
			max_values[i] = max_ascii(line);
		}

		free(line);
		fclose(fd);

	}


	// All threads merge, print out first 100 lines
	for (int i = 0; i < 100; i++)
		fprintf(stdout, "Line %d max ASCII: %d\n", i, max_values[i]);



}
