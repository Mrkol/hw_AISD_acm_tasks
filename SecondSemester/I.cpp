#include <iostream>
#include <vector>
#include <string>


using Integer = unsigned long long;

const Integer MODULO = 1000000007;


Integer pow(Integer x, Integer n)
{
	if (n == 0) return 1;

	Integer half = pow(x, n >> 1);

	if (n % 2)
	{
		return (x*((half*half) % MODULO)) % MODULO;
	}
	else
	{
		return (half*half) % MODULO;
	}
}

int main()
{
	int n = 0;
	std::cin >> n;


	std::vector<std::vector<Integer>> choose(n + 1);

	for (std::size_t i = 0; i < choose.size(); ++i)
	{
		choose[i].resize(i + 1);

		for (std::size_t j = 0; j <= i; ++j)
		{
			if (j == 0 || i == j) choose[i][j] = 1;
			else choose[i][j] = (choose[i - 1][j] + choose[i - 1][j - 1]) % MODULO;
		}
	}

	std::vector<Integer> any_count(n + 1);
	for (std::size_t i = 0; i < any_count.size(); ++i)
	{
		any_count[i] = pow(2, (((i)*(i - 1))/2));
	}



	std::vector<Integer> dp(n + 1);
	dp[1] = 1;
	for (std::size_t i = 2; i < dp.size(); ++i)
	{
		Integer sum = 0;

		for (std::size_t k = 1; k < i; ++k)
		{
			sum = 
				(sum + 
					((
						((choose[i - 1][k - 1] * dp[k]) % MODULO)	
				 	* any_count[i - k]) % MODULO))
				 % MODULO;
		}

		dp[i] = (any_count[i] - sum + MODULO) % MODULO;
	}

	std::cout << dp[n] << std::endl;


	return 0;
}
