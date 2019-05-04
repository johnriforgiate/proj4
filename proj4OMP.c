#include "proj4OMP.h"
#define NUM_THREADS 2

int main(void){

	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
	
	fp = fopen("/home/j/johnriforgiate/wiki_dump.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	bufferArray = malloc(NUM_THREADS+1 * sizeof(int));
	if (bufferArray)
		for (int i = 0; i < NUM_THREADS+1; i++)
			bufferArray[i] = malloc(10000 * sizeof *bufferArray[i]);
	
	read = getline(bufferArray, &len, fp);

	while (read != -1) 
	{
		for (int i = 1; (i < NUM_THREADS + 1) && read != -1; i++)
		{
			read = getline((bufferArray + i), &len, fp);
			//if (read == -1)
			//	break;
			
			//printf("%s", buffer_array + i);
		}
		
		//#pragma omp parallel 
		//{
			compare_lines(0);//omp_get_thread_num());
		//}
		
		// Copy the last buffer to the first spot in the array.
		if(read != -1)
		{
			//works
			
			char* newString = malloc(10000 * sizeof(*bufferArray[0]));
			
			strcpy(newString, (bufferArray[NUM_THREADS]));
			free(bufferArray[0]);
			bufferArray[0] = newString;
			
			//Doesn't work, not sure why.
			//strcpy(bufferArray[0], (bufferArray[NUM_THREADS]));

		}
		// For debugging:
		//printf("%s", "CopiedString\n");
		//printf("%s", bufferArray[0]);
		//break;
		
	}
	
	fclose(fp);
	if (line)
		free(line);
	if (bufferArray)
	{
		for (int i = 0; i < NUM_THREADS+1; i++)
			free(bufferArray[i]);
		free(bufferArray);
	}
	exit(EXIT_SUCCESS);
}
// one page per line
// take in array of strings 
char ** compare_lines(int threadID)
{
	char theChar;
	int i, j, charLoc;
	int longestSubstring[10000];
	
	char * firstString;
	char * secondString;

	//#pragma omp private(ThreadID,theChar,charLoc,firstString,secondString,longestSubstring,i,j)
	//{
		firstString = bufferArray[threadID];
		secondString = bufferArray[threadID + 1];

		//printf("myID = %d startPos = %d endPos = %d \n", myID, startPos, endPos);
		
		//Debugging to check correct strings
		/*
		printf("%s", "firstString\n");
		printf("%s", firstString);
		printf("%s", "secondString\n");
		printf("%s", secondString);
		*/

		/*				// init local count array
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

						// sum up the partial counts into the global arrays
		//#pragma omp critical
		//{
			for ( i = 0; i < ALPHABET_SIZE; i++ ) {
		 		char_counts[i] += local_char_count[i];
			}
		//}
		*/
}
