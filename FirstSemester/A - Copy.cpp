#include <cstdio>

unsigned int findRepetitionsCount(const char* s, char c1, char c2)
{
	unsigned int answer = 0;

	while (*(s + 1) != '\0')
	{
		if (*s == c1 && *(s + 1) == c2)	answer++;

		s++;
	}

	return answer;
}

int main()
{
	size_t strsize = 16;
	char* str = new char[strsize];

	unsigned int i = 0;
	char c = getchar();
	while (c != '\n' && c != '\r')
	{
		str[i] = c;
		i++;
		if (i >= strsize)
		{
			char* tmp = str;
			str = new char[strsize*2];

			for (unsigned int j = 0; j < strsize; j++) str[j] = tmp[j];

			delete[] tmp;
			strsize *= 2;
		}
		c = getchar();
	}
	str[i] = '\0';

	char a = getchar();
	getchar();
	char b = getchar();

	unsigned int answer = findRepetitionsCount(str, a, b);

	printf("%d", answer);
	delete[] str;

	return 0;
}