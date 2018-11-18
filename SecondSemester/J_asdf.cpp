#include <iostream>
#include <vector>

using namespace std;


using MaskType = unsigned long long;

int main()
{
	int width = 0;
	int height = 0;
	cin >> width >> height;
	if (height > width) swap(width, height);
	const int profile_length = height + 2;

	const MaskType profile_mask = (1ull << profile_length) - 1;

	auto shift = [profile_mask](MaskType mask) -> MaskType 
		{ return (mask << 1) & profile_mask; };


	vector<vector<int>> dp(width * height + profile_length + 1, 
		vector<int>(2 << profile_length, 0));
	dp[0][0] = 1;
	dp[0][1] = 1;
	for (int k = 0; k < width * height + profile_length; k++)
	{
		for (MaskType mask = 0; mask < (1 << profile_length) - 1; mask++)
		{
			if (k % height == 0)
			{
				dp[k + 1][shift(mask)] += dp[k][mask];
				dp[k + 1][shift(mask | 1)] += dp[k][mask];
				continue;
			}
			int count = ((mask >> 1) & 1)
				+ ((mask >> (profile_length - 1)) & 1)
				+ ((mask >> (profile_length - 2)) & 1);
			if (count == 0)
			{
				dp[k + 1][shift(mask | 1)] += dp[k][mask];
			}

			if (count == 1 || count == 2)
			{
				dp[k + 1][shift(mask | 1)] += dp[k][mask];
				dp[k + 1][shift(mask)] += dp[k][mask];
			}

			if (count == 3)
			{
				dp[k + 1][shift(mask)] += dp[k][mask];
			}
		}
	}

	cout << dp[width * height + profile_length][0] << endl;


	return 0;
}
