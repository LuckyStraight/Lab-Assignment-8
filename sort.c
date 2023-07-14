#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
void merge(int pData[], int l, int m, int r, int temp[]);
void mergeSortHelper(int pData[], int l, int r, int temp[]);
void mergeSort(int pData[], int l, int r);
int parseData(char* inputFileName, int** ppData);
void printArray(int pData[], int dataSz);
int extraMemoryAllocated;

int main(void)
{
	clock_t start, end;
	int i;
	double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

	for (i = 0; i < 4; ++i) 
    {
		int* pDataSrc, * pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);

		if (dataSz <= 0)
			continue;

		pDataCopy = (int*)malloc(sizeof(int) * dataSz);

		printf("---------------------------\n");
		printf("Dataset Size : %d\n", dataSz);
		printf("---------------------------\n");

		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.9lf\n", cpu_time_used);
		printf("\textra memory allocated\t: %d\n", extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		free(pDataCopy);
		free(pDataSrc);
	}

	return 0;
}


void heapify(int arr[], int n, int i)
{
	int largest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	if (l < n && arr[l] > arr[largest])
		largest = l;

	if (r < n && arr[r] > arr[largest])
		largest = r;

	if (largest != i) {
		int temp = arr[i];
		arr[i] = arr[largest];
		arr[largest] = temp;

		heapify(arr, n, largest);
	}
}

void heapSort(int arr[], int n)
{
	extraMemoryAllocated += sizeof(int) * n;

	for (int i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	for (int i = n - 1; i > 0; i--) 
    {
		int temp = arr[0];
		arr[0] = arr[i];
		arr[i] = temp;

		heapify(arr, i, 0);
	}
}

void merge(int pData[], int l, int m, int r, int temp[])
{
	int i = l;
	int j = m + 1;
	int k = 0;

	while (i <= m && j <= r) 
    {
		if (pData[i] <= pData[j])
			temp[k++] = pData[i++];
		else
			temp[k++] = pData[j++];
	}

	while (i <= m)
		temp[k++] = pData[i++];

	while (j <= r)
		temp[k++] = pData[j++];

	memcpy(pData + l, temp, k * sizeof(int));
	extraMemoryAllocated += k * sizeof(int);
}

void mergeSortHelper(int pData[], int l, int r, int temp[])
{
	if (l < r) 
    {
		int m = l + (r - l) / 2;

		mergeSortHelper(pData, l, m, temp);
		mergeSortHelper(pData, m + 1, r, temp);

		merge(pData, l, m, r, temp);
	}
}

void mergeSort(int pData[], int l, int r)
{
	int* temp = (int*)malloc(sizeof(int) * (r - l + 1));
	mergeSortHelper(pData, l, r, temp);
	free(temp);
}

int parseData(char* inputFileName, int** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;

	if (inFile) 
    {
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (int*)malloc(sizeof(int) * dataSz);

		if (*ppData == NULL) 
        {
			printf("Cannot allocate memory\n");
			exit(-1);
		}

		for (i = 0; i < dataSz; ++i) 
        {
			fscanf(inFile, "%d ", &n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");

	for (i = 0; i < 100; ++i) 
    {
		printf("%d ", pData[i]);
	}
	printf("\n\t");

	for (i = sz; i < dataSz; ++i) 
    {
		printf("%d ", pData[i]);
	}
	printf("\n\n");
}