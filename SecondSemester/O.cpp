#include <iostream>
#include <vector>
#include <string>


using Integer = unsigned long long;


int main()
{
	Integer n = 0;
	std::cin >> n;

	if (n == 1)
	{
		std::cout << "Vasya" << std::endl;
		return 0;
	}

	bool prime = true;
	for (Integer j = 2; j*j <= n; ++j)
	{
		if (n % j == 0) 
		{
			prime = false;
			break;
		}
	}

	if (prime)
	{
		std::cout << "David" << std::endl;
		return 0;
	}


	std::vector<Integer> primes;
	primes.reserve(78498 + 1);
	primes.push_back(1);

	for (Integer i = 2; i*i <= n; ++i)
	{
		bool succ = true;
		for (Integer j = 1; j < primes.size() && j*j <= i; ++j)
		{
			if (i % primes[j] == 0) 
			{
				succ = false;
				break;
			}
		}

		if (succ) primes.push_back(i);
	}

	Integer result = 0;

	std::size_t cprime = 1;
	Integer m = n;
	while (m > 1 && cprime < primes.size())
	{
		if (m % primes[cprime] == 0)
		{
			m /= primes[cprime];
			result = result ^ cprime;
		}
		else
		{
			++cprime;
		}
	}

	if (result)
	{
		std::cout << "David" << std::endl;
	}
	else
	{
		std::cout << "Vasya" << std::endl;
	}

	return 0;
}
