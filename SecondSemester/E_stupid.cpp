#include <iostream>
#include <vector>
#include <tuple>
#include <map>

using namespace std;


int main()
{
	srand(time(0));

	int obj_count = 0;
	cin >> obj_count;

	int type_count = 0;
	cin >> type_count;

	int box_count = 0;
	cin >> box_count;

	vector<tuple<vector<int>, map<int, int>>> boxes(box_count);
	for (int i = 0; i < box_count; ++i)
	{
		int box_size = 0;
		cin >> box_size;

		get<0>(boxes[i]).reserve(box_size);
		for (int j = 0; j< box_size; ++j)
		{
			int type = 0;
			cin >> type;
			get<0>(boxes[i]).push_back(type);
			++get<1>(boxes[i])[type];
		}
	}

	int request_count = 0;
	cin >> request_count;


	for (int q = 0; q < request_count; ++q)
	{
		int op = 0;
		cin >> op;

		if (op == 1)
		{
			int type = 0;
			int box = 0;
			cin >> type >> box;
			--box;
			cout << get<1>(boxes[box])[type] << endl;
		}
		else if (op == 2)
		{
			int type = 0;
			int from = 0;
			int depth = 0;
			int to = 0;
			cin >> type >> from >> depth >> to;
			--from;
			--to;

			vector<int>& box = get<0>(boxes[from]);

			int total = 0;
			int limit = box.size() - depth;
			for (int i = box.size() - 1; i >= 0 && i >= limit; --i)
			{
				if (box[i] == type)
				{
					box.erase(box.begin() + i);
					++total;
				}
			}

			get<1>(boxes[from])[type] -= total;
		
			for (int i = 0; i < total; ++i)
				get<0>(boxes[to]).push_back(type);

			get<1>(boxes[to])[type] += total;
		}
	}

	
	return 0;
} 
