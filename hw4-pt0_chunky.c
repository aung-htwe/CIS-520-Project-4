#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_THREADS 4

#define ARRAY_SIZE 2000000
#define STRING_SIZE 16
#define ALPHABET_SIZE 26

char char_array[ARRAY_SIZE][STRING_SIZE];
int char_counts[ALPHABET_SIZE];			// count of individual characters

pthread_mutex_t lock;					// Mutex for thread-safe updating

char getRandomChar()
{
	int randNum = 0;
	char randChar = ' ';

	randNum = ALPHABET_SIZE * (rand() / (RAND_MAX + 1.0)); 	// pick number 0 < # < 25
	randNum = randNum + 97;				// scale to 'a'
	randChar = (char) randNum;

	// printf("%c", randChar);
	return randChar;
}

void init_arrays()
{
  int i, j; 

  for ( i = 0; i < ARRAY_SIZE; i++) {
	for ( j = 0; j < STRING_SIZE; j++ ) {
		 char_array[i][j] = getRandomChar();
	}
  }

  for ( i = 0; i < ALPHABET_SIZE; i++ ) {
  	char_counts[i] = 0;
  }

  //Initialize the ability to put a thread lock when we first initialize the arrays
  pthread_mutex_init(&lock, NULL);
}

void count_array(int myID)
{
  	char theChar;
  	int i, j, charLoc;
  	int local_char_count[ALPHABET_SIZE];

  	int startPos = myID * (ARRAY_SIZE / NUM_THREADS);
  	int endPos = startPos + (ARRAY_SIZE / NUM_THREADS);

  	printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);

	// init local count array
	for ( i = 0; i < ALPHABET_SIZE; i++ ) {
  		local_char_count[i] = 0;
	}

	// count up our section of the global array
  	for ( i = startPos; i < endPos; i++) {
		for ( j = 0; j < STRING_SIZE; j++ ) {
	        theChar = char_array[i][j];
		 	charLoc = ((int) theChar) - 97;
		 	local_char_count[charLoc]++;
		}
  	}

	//we thread lock before the most critical section of the count array method
	pthread_mutex_lock(&lock);
	// sum up the partial counts into the global arrays
  	for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     	char_counts[i] += local_char_count[i];
  	}

	//unlock after it completes
	pthread_mutex_unlock(&lock);

	//terminates the calling thread
	pthread_exit(NULL);
}

void print_results()
{
  	int i,j, total = 0;

  	// then print out the totals
  	for ( i = 0; i < ALPHABET_SIZE; i++ ) {
     	total += char_counts[i];
     	printf(" %c %d\n", (char) (i + 97), char_counts[i]);
  	}
  	printf("\nTotal characters:  %d\n", total);
}

main() {
	
	pthread_t threads[NUM_THREADS];
	int i;

	init_arrays();

	//construct threads and do the work already listed
	for (i = 0; i < NUM_THREADS; i++ ) {
		int* tid = malloc(sizeof(int));
		*tid = i;

		if(pthread_create(&threads[i], NULL, count_array, tid) != 0){
			perror("Failed to create thread");
			exit(EXIT_FAILURE);
		}
	}

	//waits until all threads are done
	for (i = 0; i < NUM_THREADS; i++ ) {
		
		pthread_join(threads[i], NULL);
	}

	print_results();
	pthread_mutex_destroy(&lock);
}

