#include "proj4PT.h"
//proj4PT.c
int main(int argc, char **argv){
	//argv[1] = number of threads ex: 1
	// argv[2] = path to file. ex: "/home/j/johnriforgiate/wiki_dump.txt"
	int NUM_THREADS = atoi(argv[1]);
	
	/* Code adapted with permission from Dan Andresen "mytime.c" */
	struct timeval t1, t2;
	double elapsedTime;
	int myVersion = NUM_THREADS;
	
	// Attempt to open the file.
	FILE *fp;
	size_t len = 0;
	int read;
	fp = fopen(argv[2],"r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	// Attempt to malloc space for the array of read in buffers.
	bufferArray = (char**)malloc((NUM_THREADS+1) * sizeof(char*));
	if (bufferArray)
		for (int i = 0; i < (NUM_THREADS+1); i++)
			bufferArray[i] = malloc(10000 * sizeof(char));
	else
	{
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	// Attempt to malloc space for the array of return buffers.
	retArray = (char**)malloc((NUM_THREADS) * sizeof(char*));
	if (retArray)
		for (int i = 0; i < (NUM_THREADS); i++)
			retArray[i] = malloc(10000 * sizeof(char));
	else
	{
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	/* Code adapted with permission from Dan Andresen "mytime.c" */
	gettimeofday(&t1, NULL);
	
	// Read the first line into the buffer.
	// This is necessary because we will copy the last string into this location every other time.
	read = getline((&bufferArray[0]), &len, fp);

	lineNum = 0;
	read = 0;
	
	printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));

	// While loop to run all calculations.
	while (read != -1) 
	{
		// Used to tell how many threads to spawn.
		linesRead = 0;
		// Read in each line until the number of threads is achieved or no more lines.
		for (int i = 1; (i < (NUM_THREADS + 1)) && read != -1; i++)
		{
			len = 0;
			read = getline((&bufferArray[i]), &len, fp);
			linesRead ++;
			lineNum++;
		}
		
		/* Code adapted with permission from Dan Andresen "pt1.c" */
		int rc, ci;		
		void *status;		
		NUM_THREADS = linesRead;
		pthread_t threads[NUM_THREADS];
		pthread_attr_t attr;
		/* Initialize and set thread detached attribute */
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		for (ci = 0; ci < NUM_THREADS; ci++ ) {
			rc = pthread_create(&threads[ci], &attr, compare_lines, (void *)(__intptr_t)ci);
			if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
			}
		}
		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
		for(int i=0; i<NUM_THREADS; i++) {
			rc = pthread_join(threads[i], &status);
			if (rc) {
				printf("ERROR; return code from pthread_join() is %d\n", rc);
				exit(-1);
			}
		}
		
		// Print all lines that were read and compared.
		for(int i = 0; i < linesRead; i++)
		{
			printf("%d-%d: ",(lineNum - linesRead + i),(lineNum - linesRead + i + 1));
			printf("%s", retArray[i]);
		}
			
		// Copy the last buffer to the first spot in the array.
		if(read != -1)
		{
			char* newString = malloc(10000 * sizeof(char));
	
			strcpy(newString, (bufferArray[NUM_THREADS]));
			free(bufferArray[0]);
			bufferArray[0] = newString;
		}
		
	}
	// Close the open file.
	fclose(fp);
	
	// Free buffer and retArray.
	if (bufferArray)
	{
		for (int i = 0; i < NUM_THREADS+1; i++)
			free(bufferArray[i]);
		free(bufferArray);
	}
	if (retArray)
	{
		for (int i = 0; i < NUM_THREADS; i++)
			free(retArray[i]);
		free(retArray);
	}
	
	/* Code adapted with permission from Dan Andresen "mytime.c" */
	gettimeofday(&t2, NULL);	
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, NumThreads: %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	
	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
}

// Function to compare lines.
// Takes in and returns a void pointer.
void *compare_lines(void *myID)
{
	// Variables for pthreads.
	int threadID = (int)(__intptr_t)myID;
	int **stringChart;
	
	// Variables for computation.
	int xMax; 
	int xPos; 
	int yPos; 
	int maxVal;
	char* maxString;
	


	// Allocate stringChart in the heap.
	stringChart = (int**)malloc(10000 * sizeof(int*));
	if (stringChart)
		for (int i = 0; i < 10000; i++)
			stringChart[i] = malloc(10000 * sizeof(int));
	xMax = 0; xPos = 0; yPos = 0; maxVal = 0;
	// Allocate maxString in the heap.
	maxString = malloc(10000* sizeof(char*));
	
	// Double while loop iterating until end of either string.
	while(bufferArray[threadID][xPos] != '\n') 
	{ 
		while(bufferArray[threadID][yPos] != '\n') 
		{ 
			// If the position is 0 the value is 0.
			if(xPos == 0 || yPos == 0) 
			{
				stringChart[xPos][yPos] = 0; 
			} 
			// If the values are the same.
			else if(bufferArray[threadID][xPos] == bufferArray[threadID+1][yPos]) 
			{
				stringChart[xPos][yPos] = stringChart[xPos-1][yPos-1] + 1;
				// If the val is greater than the maximum, update the maximum.
				if(maxVal < stringChart[xPos][yPos]) 
				{ 
					xMax = xPos; 
					maxVal = stringChart[xPos][yPos];
				}
			} 
			// If they don't match the value is 0
			else 
			{ 
				stringChart[xPos][yPos] = 0; 
			} 
			// Iterate through y string.
			yPos += 1; 
		} 
		// Reset y string to 0.
		yPos = 0;
		// Iterate through x string.
		xPos += 1; 
	}
	// Copy the string in question to the maxString variable.
	strncpy(maxString, &bufferArray[threadID][xMax - maxVal], maxVal);
	maxString[maxVal] = '\n';
	free(retArray[threadID]);
	retArray[threadID] = maxString;
	
	// Free the string chart to avoid memory leaks.
	if (stringChart)
	{
		for (int i = 0; i < 10000; i++)
			free(stringChart[i]);
		free(stringChart);
	}
	
	pthread_exit(NULL);
}
