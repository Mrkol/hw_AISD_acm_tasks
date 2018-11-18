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
	char str[1000010];
	fgets(str, 1000010, stdin);

	char a = 0;
	char b = 0;
	scanf("%c %c", &a, &b);

	unsigned int answer = findRepetitionsCount(str, a, b);

	printf("%d", answer);

	return 0;
}