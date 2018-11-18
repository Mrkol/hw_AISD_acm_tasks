#include <cstdio>


struct Player
{
	int ind;
	long long eff;
};

void mergeSort(Player* start, Player* finish, Player* buffer, bool byIndex)
{
	if (finish - start <= 1) return;

	Player* middle = start + (finish - start)/2;

	mergeSort(start, middle, buffer, byIndex);
	mergeSort(middle, finish, buffer, byIndex);

	Player* i = start;
	Player* j = middle;
	Player* t = buffer;

	while (i < middle || j < finish)
	{
		bool less = (i->ind <= j->ind && byIndex) 
			|| (!byIndex && i->eff <= j->eff);

		if (i < middle && (less || j >= finish)) *(t++) = *(i++);
		else if (j < finish) *(t++) = *(j++);
	}

	for (int i = 0; i < finish - start; i++) start[i] = buffer[i];
}

int main()
{
	int count = 0;
	scanf("%d", &count);

	Player* players = new Player[count];

	for (int i = 0; i < count; i++)
	{
		scanf("%lld", &players[i].eff);
		players[i].ind = i;
	}

	Player* tmp = new Player[count];
	mergeSort(players, players + count, tmp, false);
	delete[] tmp;

	long long* prefixSumms = new long long[count + 1];
	prefixSumms[0] = 0;
	for (int i = 1; i <= count; i++)
	{
		prefixSumms[i] = prefixSumms[i - 1] + players[i - 1].eff;
	}


	long long bestEff = 0;
	int bestBeg = 0;
	int bestEnd = 0;


	int beg = 0;
	int end = 1;

	while (end < count)
	{
		if (players[end].eff > players[beg].eff + players[beg + 1].eff)
		{
			if (prefixSumms[end] - prefixSumms[beg] >= bestEff)
			{
				bestBeg = beg;
				bestEnd = end;
				bestEff = prefixSumms[end] - prefixSumms[beg];
			}
			beg++;
		}
		else
		{
			end++;
		}
	}
	if (prefixSumms[end] - prefixSumms[beg] >= bestEff)
	{
		bestBeg = beg;
		bestEnd = end;
		bestEff = prefixSumms[end] - prefixSumms[beg];
	}


	Player* result = new Player[bestEnd - bestBeg];
	for (int i = bestBeg; i < bestEnd; i++)
	{
		result[i - bestBeg] = players[i];
	}

	tmp = new Player[bestEnd - bestBeg];
	mergeSort(result, result + bestEnd - bestBeg, tmp, true);
	delete[] tmp;

	printf("%lld\n", prefixSumms[bestEnd] - prefixSumms[bestBeg]);

	for (int i = 0; i < bestEnd - bestBeg; i++)
	{
		printf("%d ", result[i].ind + 1);
	}



	delete[] players;
	delete[] prefixSumms;

	return 0;
}