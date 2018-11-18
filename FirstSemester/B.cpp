#include <cstdio>


void mergeSort(int* start, int* finish, int* buffer)
{
	if (finish - start <= 1) return;

	int* middle = start + (finish - start)/2;

	mergeSort(start, middle, buffer);
	mergeSort(middle, finish, buffer);

	int* i = start;
	int* j = middle;
	int* t = buffer;

	while (i < middle || j < finish)
	{
		if (i < middle && (*i < *j || j >= finish)) *(t++) = *(i++);
		else if (j < finish) *(t++) = *(j++);
	}

	for (int i = 0; i < finish - start; i++) start[i] = buffer[i];
}

int main()
{
	int count = 0;
	int length = 0;
	scanf("%d %d", &count, &length);

	int* sequence = new int[count*length];

	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < length; j++)
		{
			scanf("%d", sequence + i*length + j);
		}
	}

	int* buffer = new int[count*length];
	mergeSort(sequence, sequence + count*length, buffer);
	delete[] buffer;

	for (int i = 0; i < count*length; i++)
	{
		printf("%d ", sequence[i]);
	}
	printf("\n");
	delete[] sequence;

	return 0;
}