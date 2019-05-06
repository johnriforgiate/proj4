#include "proj4PT.h"
//proj4PT.c
int main(int argc, char **argv){

	int NUM_THREADS = atoi(argv[1]);
	struct timeval t1, t2;
	double elapsedTime;
	int myVersion = NUM_THREADS;
	FILE *fp;
	size_t len = 0;
	int read;
	fp = fopen(argv[2],"r");
	//fp = fopen("/home/j/johnriforgiate/wiki_dump.txt", "r");
	//fp = fopen("/home/j/johnriforgiate/test.txt","r");
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	void *status;
	
	bufferArray = (char**)malloc((NUM_THREADS+1) * sizeof(char*));
	if (bufferArray)
		for (int i = 0; i < (NUM_THREADS+1); i++)
			bufferArray[i] = malloc(10000 * sizeof(char));
	else
	{
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	retArray = (char**)malloc((NUM_THREADS) * sizeof(char*));
	if (retArray)
		for (int i = 0; i < (NUM_THREADS); i++)
			retArray[i] = malloc(10000 * sizeof(char));
	else
	{
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	gettimeofday(&t1, NULL);
	
	read = getline((&bufferArray[0]), &len, fp);

	int lineNum = 0;
	read = 0;
	
	printf("DEBUG: starting loop on %s\n", getenv("HOSTNAME"));

	while (read != -1) 
	{
		//printf("beforeFor");
		int linesRead = 0;
		for (int i = 1; (i < (NUM_THREADS + 1)) && read != -1; i++)
		{
			len = 0;
			read = getline((&bufferArray[i]), &len, fp);
			linesRead ++;
			lineNum++;
		}
		
		int rc;
		pthread_t threads[NUM_THREADS];
		pthread_attr_t attr;
		void *status;
		
		pthread_barrier_init(&barrier, NULL, NUM_THREADS+1);
		
		/* Initialize and set thread detached attribute */
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		init_arrays();

		for (int i = 0; i < NUM_THREADS; i++ ) {
			rc = pthread_create(&threads[i], &attr, compare_lines, (void *)i);
			if (rc) {
				printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
		
		pthread_barrier_destory(&barrier);
		
		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
		for(i=0; i<NUM_THREADS; i++) {
			rc = pthread_join(threads[i], &status);
			if (rc) {
				printf("ERROR; return code from pthread_join() is %d\n", rc);
				exit(-1);
			}
		}

		// Copy the last buffer to the first spot in the array.
		if(read != -1)
		{
			//works
			
			char* newString = malloc(10000 * sizeof(char));
	
			strcpy(newString, (bufferArray[NUM_THREADS]));
			free(bufferArray[0]);
			bufferArray[0] = newString;
			//Doesn't work, not sure why.
			//strcpy(bufferArray[0], (bufferArray[NUM_THREADS]));

		}
		
	}
	
	fclose(fp);

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
	gettimeofday(&t2, NULL);
	
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, NumThreads: %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
	exit(EXIT_SUCCESS);
}
// one page per line
// take in array of strings 

void *compare_lines(void *myID)
{
	
	int **stringChart;
	int xMax; 
	//int yMax; 
	int xPos; 
	int yPos; 
	int maxVal;
	char* maxString;

	stringChart = (int**)malloc(10000 * sizeof(int*));
	if (stringChart)
		for (int i = 0; i < 10000; i++)
			stringChart[i] = malloc(10000 * sizeof(int));
	xMax = 0; yMax = 0; xPos = 0; yPos = 0; maxVal = 0;
	maxString = malloc(10000* sizeof(char*));
	
	while(bufferArray[threadID][xPos] != '\n') 
	{ 
		while(bufferArray[threadID][yPos] != '\n') 
		{ 
			if(xPos == 0 || yPos == 0) 
			{
				stringChart[xPos][yPos] = 0; 
			} 
			else if(bufferArray[threadID][xPos] == bufferArray[threadID+1][yPos]) 
			{
				stringChart[xPos][yPos] = stringChart[xPos-1][yPos-1] + 1;
				if(maxVal < stringChart[xPos][yPos]) 
				{ 
					xMax = xPos; 
					//yMax = yPos; 
					maxVal = stringChart[xPos][yPos];
				}
			} 
			else 
			{ 
				stringChart[xPos][yPos] = 0; 
			} 
			yPos += 1; 
		} 
		yPos = 0;
		xPos += 1; 
	}
	strncpy(maxString, &bufferArray[threadID][xMax - maxVal], maxVal);
	maxString[maxVal] = '\n';
	free(retArray[threadID]);
	retArray[threadID] = maxString;
	
	pthread_barrier_wait(&barrier);
	
	for(int i = 0; i < linesRead; i++)
	{
		printf("%d-%d: ",(lineNum - linesRead + i),(lineNum - linesRead + i + 1));
		printf("%s", retArray[i]);
	}
}
