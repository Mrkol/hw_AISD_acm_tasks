#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int segment_count = 0;
	cin >> segment_count;
	int point_count = 0;
	cin >> point_count;

	int OP = -1;
	int CL = point_count;

	vector<pair<int, int>> events;
	events.reserve(2*segment_count + point_count);
	for (int i = 0; i < segment_count; i++)
	{
		int fst = 0;
		int snd = 0;
		cin >> fst >> snd;
		if (fst > snd) swap(fst, snd);
		events.push_back(make_pair(fst, OP));
		events.push_back(make_pair(snd, CL));
	}

	for (int i = 0; i < point_count; i++)
	{
		int tmp = 0;
		cin >> tmp;
		events.push_back(make_pair(tmp, i));
	}

	sort(events.begin(), events.end());
	

	vector<int> result(point_count, 0);
	int curr_segments = 0;
	for (auto& event : events)
	{
		if (event.second == OP) curr_segments++;
		else if (event.second == CL) curr_segments--;
		else result[event.second] = curr_segments;
	}

	for (auto& value : result) cout << value << " ";
	return 0;
}