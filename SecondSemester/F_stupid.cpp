#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;


using Integer = long long;


int main()
{
	int count = 0;
	std::cin >> count;

	vector<Integer> data(count);
	for (int i = 0; i < count; ++i)
	{
		std::cin >> data[i];
	}

	int request_count = 0;
	std::cin >> request_count;

	for (int q = 0; q < request_count; ++q)
	{
		int request = 0;
		std::cin >> request;
		switch (request)
		{
			case 1: //segment sum
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					cout << accumulate(data.begin() + l, data.begin() + r + 1, 0) << endl;
				}
				break;

			case 2: //insert at
				{
					Integer value = 0;
					int pos = 0;
					std::cin >> value >> pos;
					data.insert(data.begin() + pos, value);
				}
				break;

			case 3: //remove at
				{
					int pos = 0;
					std::cin >> pos;
					data.erase(data.begin() + pos);
				}
				break;

			case 4: //set on segment
				{
					Integer value = 0;
					int l = 0;
					int r = 0;
					std::cin >> value >> l >> r;
					std::for_each(data.begin() + l, data.begin() + r + 1, [value](Integer& i) { i = value; });
				}
				break;

			case 5: //add on segment
				{
					Integer value = 0;
					int l = 0;
					int r = 0;
					std::cin >> value >> l >> r;
					std::for_each(data.begin() + l, data.begin() + r + 1, [value](Integer& i) { i += value; });
				}
				break;

			case 6: //next perm
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					std::next_permutation(data.begin() + l, data.begin() + r + 1);
				}
				break;

			case 7: //prev perm
				{
					int l = 0;
					int r = 0;
					std::cin >> l >> r;
					std::prev_permutation(data.begin() + l, data.begin() + r + 1);
				}
				break;
		}
	}
	for_each(data.begin(), data.end(), [](Integer& i) { std::cout << i << " "; });
	cout << endl;
	
	return 0;
} 
