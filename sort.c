#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	// terminating condition
	if (l >= r) return;
	else {

		int mid = (l+r)/2; // midpoint

		// recursive call;
		mergeSort(pData, l, mid);
		mergeSort(pData, mid+1, r);

		// now we need to actually merge things
		// lengths of temp arrays
		int lSize = mid - l + 1;
		int rSize = r - mid;

		// temp arrays
		int* lArray = (int*)Alloc(lSize*sizeof(int));
		int* rArray = (int*)Alloc(rSize*sizeof(int));

		// populate temp arrays
		for (int x = 0; x < lSize; x++)
			lArray[x] = pData[l + x];
		
		for (int y = 0; y < rSize; y++)
			rArray[y] = pData[mid+1 + y];

		// put data from temp arrays back into array
		int m = 0, n = 0; // indexes for temp arrays
		for (int p = l; p < r+1; p++) {
			
			// if we've already run out of entries in rArray but lArray still has some left
			if (n >= rSize && m < lSize) {
				pData[p] = lArray[m];
				m++;
			}
			// vice versa
			else if (m >= lSize && n < rSize) {
				pData[p] = rArray[n];
				n++;
			}
			// otherwise, compare the two and determine which to put first
			else {

				if (lArray[m] <= rArray[n]) {
					pData[p] = lArray[m];
					m++;
				}
				else {
					pData[p] = rArray[n];
					n++;
				}
			}
			//printf("%d\n",pData[p]);
		}
		//printf("\n\n");
		//for (int i = 0; i < r; i++) printf("%d\n",pData[i]);
		//printf("\n\n");

		DeAlloc(lArray);
		DeAlloc(rArray);
 	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
    int firstHundred = (dataSz < 100 ? dataSz : 100);
    printf("\tData:\n\t");
    for (i=0;i<firstHundred;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");

    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}