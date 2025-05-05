#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 1000000
#define MAX_LINE_LENGTH 2100

//char lines[MAX_LINES][MAX_LINE_LENGTH]; // Buffer to store all lines
int* max_values;

//taken from the 3 way pthread
/// Function to calculate the max ASCII value of a given string 
/// \param str The line passed in to calculate
/// \return the max ASCII value
int max_ascii(const char* str){
	int max = 0;

	for (int i = 0; str[i] != '\0'; i++) {
        	if ((unsigned char)str[i] > max) {
            		max = (unsigned char)str[i];
        	}
	}

	return max;
}

//int argc, char *argv[] are apart of the sample
/*
Might need to include ./main /homes/dan/625/wiki_dump.txt
OR
Change MPI_Init(2, &argv)
*/
///  The Main program for using MPI
/// \param argc The number of arguements in total
/// \param argv should include the program name then filename
/// \return 0 if completed successfully and 1 if not
int main(int argc, char *argv[]){
	int rank, size;

	// initialize MPI
	MPI_Init(&argc, &argv);
	//MPI_Init(2, &argv);
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//No File Designated
	if (argc < 2) {
    		if (rank == 0){
			fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
			MPI_Finalize();
        		return 1;
	    	}
	}

	int total_lines = 0;

	char **lines =(char**)malloc(MAX_LINES * sizeof(char*));
	if (lines == NULL){
		printf("ERROR; Failed to allocate memory for lines\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	for (int i = 0; i < MAX_LINES; i++){
		lines[i] = (char*)malloc(MAX_LINE_LENGTH * sizeof(char));
		if (lines[i] == NULL){
			printf("ERROR; failed to allocate memory for lines");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}


	// get number of tasks and load project file
	if (rank == 0) {
        	FILE *fd = fopen(argv[1], "r");
        	if (fd == NULL) {
            		printf("ERROR; Failed to open file\n");
            		MPI_Abort(MPI_COMM_WORLD, 1);
        		return 1;
		}

        	while (total_lines < MAX_LINES && fgets(lines[total_lines], MAX_LINE_LENGTH, fd) != NULL) {
	            	lines[total_lines][strcspn(lines[total_lines], "\n")] = '\0';
	            	total_lines++;
        	}

        fclose(fd);

	}

	//broadcast total line count
	MPI_Bcast(&total_lines, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//Calculate distribution. Extra of 0 or 1 depending on if total number of processes are odd or even
	int lines_per_proc = total_lines / size;
	int extra = total_lines % size;

	int primary_count = lines_per_proc + (extra > 0 ? 1 : 0);

	// Prepare local buffer. Used to store proceses' distribution of lines
	char *local_lines_buffer = (char*) malloc(primary_count * MAX_LINE_LENGTH);
	if (local_lines_buffer == NULL){
		fprintf(stderr, "ERROR: Memory allocation failed for local lines buffer\n");
		MPI_Abort(MPI_COMM_WORLD, 1);

	}

	char **local_lines = (char**) malloc(primary_count * sizeof(char*));
	if (local_lines == NULL)
	{
		fprintf(stderr, "ERROR: Memory allocation failed for local lines\n");
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	for (int i = 0; i < primary_count; i++){
		local_lines[i] = local_lines_buffer + i * MAX_LINE_LENGTH;
	}

	/*
	for (int i = 0; i < primary_count; i++){
		local_lines[i] = (char*) malloc(MAX_LINE_LENGTH * sizeof(char));
		if (local_lines[i] == NULL){
			printf("ERROR; failed to allocate memory for lines");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
	}

	*/

	// If in master rank, send lines of file to slaves.
	if(rank == 0){
		// for all slave ranks, send from local_lines 
		for(int i = 1; i < size; i++){
			int local_count = lines_per_proc;
			int local_offset = i * local_count;

			MPI_Send(&(lines[local_offset][0]), local_count * MAX_LINE_LENGTH, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			// MPI_Send(lines[local_offset], local_count * MAX_LINE_LENGTH, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}

		// in master rank, directly copy into local_lines from lines
		for (int i = 0; i < primary_count; i++)
			memcpy(local_lines[i], lines[i], MAX_LINE_LENGTH);
	}
	else{
		//included in examples I found
		// recieves data
		MPI_Recv(local_lines_buffer, primary_count * MAX_LINE_LENGTH, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	//calculate max ASCII values for local lines
	int *local_max = (int*) malloc(sizeof(int) * primary_count);
	for(int i = 0; i < primary_count; i++){
		local_max[i] = max_ascii(local_lines[i]);
	}

	//Gather results at root
	int *recv_counts = NULL;
	int *displs = NULL;
	if(rank == 0){
		recv_counts = malloc(sizeof(int) * size);
		displs = malloc(sizeof(int) * size);
		max_values = (int*) malloc(sizeof(int) * total_lines);

		for(int i = 0; i < size; i++){
			recv_counts[i] = lines_per_proc + (i < extra ? 1 : 0);
			displs[i] = i * lines_per_proc + (i < extra ? 1 : 0);
		}
	}

	//This collects data from all processes and puts them into the buffer max_values
	/*
	looks like this
	int MPI_Gatherv(
				const void* buffer_send,
                int count_send,
                MPI_Datatype datatype_send,
                void* buffer_recv,
                const int* counts_recv,
                const int* displacements,
                MPI_Datatype datatype_recv,
                int root,
                MPI_Comm communicator);
	*/

	MPI_Gatherv(local_max, primary_count, MPI_INT, max_values, recv_counts, displs, MPI_INT, 0, MPI_COMM_WORLD);

	//clean up and print results
	if(rank == 0){
		for (int i = 0; i < 100; i++) {
			//printf("Line %d: %s\n", i, lines[i]);
			printf("Line %d max ASCII: %d\n", i + 1, max_values[i]);
		}
		free(recv_counts);
		free(displs);
		free(max_values);
	}


	free(local_max);

	for (int i = 0; i < MAX_LINES; i++){
		free(lines[i]);
	}

	free(local_lines);
	free(lines);

	// done with MPI
	MPI_Finalize();
	return 0;
}
