#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

const std::size_t SIZE = 100001;

int main()
{
	std::vector<std::vector<bool>> dp(SIZE, std::vector<bool>(452, false));

	for (std::size_t n = 0; n < SIZE; ++n)
	{
		for (std::size_t k = 450; k > 0; --k)
		{
			if (n == 0 || n < k) dp[n][k] = false;
			else if (n == k || n == k + 1) dp[n][k] = true;
			else
			dp[n][k] = 
				!(dp[n - k][k] && dp[n - k - 1][k + 1]);
		}
	}


	int n = 0;
	std::cin >> n;

	int i = 1;
	while (n != 0)
	{
		std::cout << "Case #" << i << ": ";
		if (!dp[n - 1][1])
			std::cout << "First";
		else
			std::cout << "Second";
		std::cout << " player wins." << std::endl;

		++i;
		std::cin >> n;
	}

	return 0;
}
