#include <iostream>
#include <vector>


using MaskType = unsigned long long;

int main()
{
	std::size_t width = 0;
	std::size_t height = 0;
	std::cin >> width >> height;

	if (width == 1 && height == 1)
	{
		std::cout << 2 << std::endl;
		return 0;
	}

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


	std::vector<long long> dp_current(1 << profile_length);
	std::vector<long long> dp_next(1 << profile_length);

	for (MaskType j = 0; j <= profile_mask; ++j)
	{
		dp_current[j] = 1;
	}

	for (std::size_t i = 0; i < width*height - profile_length; ++i)
	{
		dp_next.assign(dp_next.size(), 0);

		for (MaskType j = 0; j <= profile_mask; ++j)
		{
			int cnt = get(j, profile_length - 1) + get(j, 0) + get(j, 1);

			if (0 < cnt || (i + 1) % height == 0)
			{
				dp_next[shift0(j)] += dp_current[j];
				dp_next[shift0(j)] %= modulo;
			}

			if (cnt < 3 || (i + 1) % height == 0)
			{
				dp_next[shift1(j)] += dp_current[j];
				dp_next[shift1(j)] %= modulo;
			}
		}

		std::swap(dp_current, dp_next);
	}

	// for (MaskType j = 0; j <= profile_mask; ++j)
	// {
	// 	for (std::size_t i = 0; i < width*height + 1; ++i)
	// 	{
	// 		std::cout << dp[j][i] << ' ';
	// 	}
	// 	std::cout << std::endl;
	// }

	long long answer = 0;
	for (MaskType j = 0; j <= profile_mask; ++j)
	{
		answer += dp_current[j];
		answer %= modulo;
	}
	std::cout << answer << std::endl;


	return 0;
}
