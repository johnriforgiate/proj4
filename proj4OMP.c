#include "proj4OMP.h"
#define NUM_THREADS 1

int main(void){

	FILE *fp;
	size_t len = 0;
	ssize_t read;
	printf("fopen");
	fp = fopen("/home/j/johnriforgiate/wiki_dump.txt", "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	printf("malloc");
	bufferArray = (char**)malloc((NUM_THREADS+1) * sizeof(char*));
	if (bufferArray)
		for (int i = 0; i < (NUM_THREADS+1); i++)
			bufferArray[i] = malloc(10000 * sizeof(char));
	retArray = (char**)malloc((NUM_THREADS) * sizeof(char*));
	if (retArray)
		for (int i = 0; i < (NUM_THREADS); i++)
			retArray[i] = malloc(10000 * sizeof(char));
	else
	{
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	
	read = getline((&bufferArray[0]), &len, fp);

	read = 0;
	while (read != -1) 
	{
		//printf("beforeFor");
		for (int i = 1; (i < (NUM_THREADS + 1)) && read != -1; i++)
		{
			//printf("read");
			len = 0;
			read = getline((&bufferArray[i]), &len, fp);
			//if (read == -1)
			//	break;
			
			//printf("%s", buffer_array + i);
		}
		
		//#pragma omp parallel 
		//{
			//printf("call");
			compare_lines(0);//omp_get_thread_num());
		//}
		/*
		printf("firststring");
		printf("\n%s\n",bufferArray[0]);
		printf("secondString");
		printf("\n%s\n",bufferArray[1]);
		*/

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
		// For debugging:
		//printf("%s", "CopiedString\n");
		//printf("%s", bufferArray[0]);
		//break;
		
	}
	
	fclose(fp);

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
//char ** compare_lines(int threadID)
void compare_lines(int threadID)
{
	
	int **stringChart = (int**)malloc(10000 * sizeof(int*));
	if (stringChart)
		for (int i = 0; i < 10000; i++)
			stringChart[i] = malloc(10000 * sizeof(int));
	int xMax = 0; int yMax = 0; int xPos = 0; int yPos = 0; int maxVal = 0;
	char* maxString = malloc(10000* sizeof(char*));
	//char * firstString;
	//char * secondString;

	//#pragma omp private(ThreadID,theChar,charLoc,firstString,secondString,longestSubstring,i,j)
	//{
		//firstString = bufferArray[threadID];
		//secondString = bufferArray[threadID + 1];
		
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
						yMax = yPos; 
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
	
			//strcpy(newString, (bufferArray[NUM_THREADS]));
			//free(bufferArray[0]);
			//bufferArray[0] = newString;
		strncpy(maxString, &bufferArray[threadID][xMax - maxVal], maxVal);
		maxString[maxVal] = '\n';
		free(retArray[threadID]);
		retArray[threadID] = maxString;
		printf("%s", retArray[threadID]);
		

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
