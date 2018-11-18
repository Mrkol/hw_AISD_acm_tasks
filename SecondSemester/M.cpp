#include <iostream>
#include <vector>
#include <functional>

using std::vector;
using std::pair;

vector<vector<int>> edge_list2adj_list(int vert_co, const vector<pair<int, int>>& vert_list)
{
	vector<vector<int>> adj_list(vert_co);

	for (auto& p : vert_list)
	{
		adj_list.reserve(vert_co/2);
		adj_list[p.first].push_back(p.second);
		adj_list.shrink_to_fit();
	}

	return adj_list;
}

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);

	int vert_count = 0;
	int edge_count = 0;
	while (std::cin >> vert_count >> edge_count)
	{
		vector<pair<int, int>> edge_list(edge_count);
		vector<pair<int, int>> edge_list_reversed(edge_count);
		for (int i = 0; i < edge_count; ++i)
		{
			std::cin >> edge_list[i].first >> edge_list[i].second;
			--edge_list[i].first;
			--edge_list[i].second;
			edge_list_reversed[i].second = edge_list[i].first;
			edge_list_reversed[i].first = edge_list[i].second;
		}

		vector<vector<int>> adj_list = edge_list2adj_list(vert_count, edge_list);
		vector<vector<int>> adj_list_reversed = edge_list2adj_list(vert_count, edge_list_reversed);

		const int DRAW = 0;
		const int LOSE = 1;
		const int WIN = 2;
		vector<bool> used(vert_count, false);
		vector<int> win_turns(vert_count, 0);
		vector<int> gamestate(vert_count, 0);
		std::function<void(int)> dfs = [&](int current)
			{
				used[current] = true;
				for (auto next : adj_list_reversed[current])
				{
					if (used[next]) continue;

					if (gamestate[current] == WIN)
					{
						++win_turns[next];
						if (win_turns[next] == adj_list[next].size())
						{
							gamestate[next] = LOSE;
							dfs(next);
						}
					}
					else if (gamestate[current] == LOSE)
					{
						gamestate[next] = WIN;
						dfs(next);
					}
				}
			};

		for (int i = 0; i < vert_count; ++i)
		{
			if (adj_list[i].size() == 0)
			{
				gamestate[i] = LOSE;
				dfs(i);
			}
		}



		for (int i = 0; i < vert_count; ++i)
		{
			if (gamestate[i] == WIN) std::cout << "FIRST";
			if (gamestate[i] == LOSE) std::cout << "SECOND";
			if (gamestate[i] == DRAW) std::cout << "DRAW";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	return 0;
}
