#include "proj4OMP.h"

//proj4OMP.c
int main(int argc, char **argv){

	//argv[1] = number of threads ex: 1
	// argv[2] = path to file. ex: "/home/j/johnriforgiate/wiki_dump.txt"
	
	/* Code adapted with permission from Dan Andresen "mytime.c" */
	int NUM_THREADS = atoi(argv[1]);
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
	
	// Attempt to malloc space for array of read in buffers.
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

	int lineNum = 0;
	read = 0;
	
	printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));

	// While loop to run all calculations.
	while (read != -1) 
	{
		// Used to tell how many threads to spawn.
		int linesRead = 0;
		// Read in each line until the number of threads is achieved or no more lines.
		for (int i = 1; (i < (NUM_THREADS + 1)) && read != -1; i++)
		{
			len = 0;
			read = getline((&bufferArray[i]), &len, fp);
			linesRead ++;
			lineNum++;
		}
		
		// Set the number of threads to run to the number of lines read in.
		omp_set_num_threads(linesRead);

		// Run the threads in parallel. 
		#pragma omp parallel 
		{
			compare_lines(omp_get_thread_num());
		}
		// Wait for threads to finish to guarantee synchronization.
		#pragma omp barrier
		// Print out the return of all threads that were compared.
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
	
	// Close the file.
	fclose(fp);

	// If bufferArray is still allocated, deallocate it.
	if (bufferArray)
	{
		for (int i = 0; i < NUM_THREADS+1; i++)
			free(bufferArray[i]);
		free(bufferArray);
	}
	// If retArray is still allocated, deallocate it.
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
	
	exit(EXIT_SUCCESS);
}

// Function to compare lines. 
void compare_lines(int threadID)
{
	// Variables for computation
	int **stringChart;
	int xMax; int yMax; int xPos; int yPos; int maxVal;
	char* maxString;

	// Declare all necessary variables private so that they don't overlap in memory space.
	#pragma omp private(stringChart,xMax,yMax,xPos,yPos,maxVal,maxString)
	{
		// Allocate all private variables.
		stringChart = (int**)malloc(10000 * sizeof(int*));
		if (stringChart)
			for (int i = 0; i < 10000; i++)
				stringChart[i] = malloc(10000 * sizeof(int));
		xMax = 0; yMax = 0; xPos = 0; yPos = 0; maxVal = 0;
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
						yMax = yPos; 
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
	}
}
