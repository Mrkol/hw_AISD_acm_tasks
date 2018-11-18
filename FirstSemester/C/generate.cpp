#include <cstdio>
#include <cstdlib>

int main(int argc, const char* argw[])
{
	if (argc != 3) return 1;

	int count = 0;
	count = atoi(argw[2]);


	FILE* out = fopen(argw[1], "w");

	unsigned long long a = 0;
	unsigned long long b = 1;
	for (int i = 0; i < count; i++)
	{
		fprintf(out, "%lf ", b / 1000000.0);
		unsigned long long n = (a + b) % 1000000007;
		a = b;
		b = n;
	}

	fclose(out);

	return 0;
}