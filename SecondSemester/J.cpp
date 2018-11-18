#include <iostream>
#include <vector>


using MaskType = unsigned long long;

int main()
{
	std::size_t width = 0;
	std::size_t height = 0;
	std::cin >> width >> height;
	if (height > width) std::swap(width, height);


	const long long modulo = (1 << 30) + 1;
	const std::size_t profile_length = height + 1;
	const MaskType profile_mask = (1 << profile_length) - 1;

	auto shift0 = [](MaskType mask)
		{ return mask >> 1; };

	auto shift1 = [profile_length](MaskType mask)
		{ return (mask >> 1) | (1 << (profile_length - 1)); };

	auto get = [](MaskType mask, std::size_t i) -> bool
		{ return mask & (1 << i); };


	std::vector<std::vector<long long>> dp(1 << profile_length, 
		std::vector<long long>(width*(height + 3), 0));

	for (MaskType j = 0; j <= profile_mask; ++j)
	{
		dp[j][0] = 1;
	}

	for (std::size_t i = 0; i < width*height; ++i)
	{
		for (MaskType j = 0; j <= profile_mask; ++j)
		{
			int cnt = get(j, profile_length - 1) + get(j, 0) + get(j, 1);

			if (0 < cnt || (i + 1) % height == 0)
			{
				dp[shift0(j)][i + 1] += dp[j][i];
				dp[shift0(j)][i + 1] %= modulo;
			}

			if (cnt < 3 || (i + 1) % height == 0)
			{
				dp[shift1(j)][i + 1] += dp[j][i];
				dp[shift1(j)][i + 1] %= modulo;
			}
		}
	}

	// for (MaskType j = 0; j <= profile_mask; ++j)
	// {
	// 	for (std::size_t i = 0; i < width*height + 1; ++i)
	// 	{
	// 		std::cout << dp[j][i] << ' ';
	// 	}
	// 	std::cout << std::endl;
	// }

	if (width == 1 && height == 1)
	{
		std::cout << 2;
		return 0;
	}

	long long answer = 0;
	for (MaskType j = 0; j <= profile_mask; ++j)
	{
		answer += dp[j][(width - 1)*height - 1];
		answer %= modulo;
	}
	std::cout << answer;


	return 0;
}
