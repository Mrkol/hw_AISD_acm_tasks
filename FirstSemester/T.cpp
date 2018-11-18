#include <iostream>
#include <vector>
#include <deque>

using namespace std;


int main()
{
	int mat_size = 0;
	cin >> mat_size;
	int sub_size = 0;
	cin >> sub_size;

	vector<vector<int>> matrix(mat_size, 
		vector<int>(mat_size));

	for (int j = 0; j < mat_size; j++)
	{
		for (int i = 0; i < mat_size; i++)
		{
			cin >> matrix[j][i];
		}
	}

	vector<vector<int>> row_mins(mat_size, 
		vector<int>(mat_size - sub_size + 1));

	for (int j = 0; j < mat_size; j++)
	{
		deque<int> deq;

		for (int i = 0; i < sub_size - 1; i++)
		{
			while (!deq.empty() && matrix[j][deq.back()] > matrix[j][i])
				deq.pop_back();
			deq.push_back(i);
		}
		
		for (int i = sub_size - 1; i < mat_size; i++)
		{
			while (!deq.empty() && matrix[j][deq.back()] > matrix[j][i])
				deq.pop_back();
			deq.push_back(i);

			row_mins[j][i - sub_size + 1] = matrix[j][deq.front()]; 
			
			if (!deq.empty() && deq.front() == i - sub_size + 1)
				deq.pop_front();
		}
	}

	vector<vector<int>> mins(mat_size - sub_size + 1, 
		vector<int>(mat_size - sub_size + 1));

	for (int j = 0; j < mat_size - sub_size + 1; j++)
	{
		deque<int> deq;

		for (int i = 0; i < sub_size - 1; i++)
		{
			while (!deq.empty() && row_mins[deq.back()][j] > row_mins[i][j])
				deq.pop_back();
			deq.push_back(i);
		}
		
		for (int i = sub_size - 1; i < mat_size; i++)
		{
			while (!deq.empty() && row_mins[deq.back()][j] > row_mins[i][j])
				deq.pop_back();
			deq.push_back(i);

			mins[i - sub_size + 1][j] = row_mins[deq.front()][j]; 
			
			if (!deq.empty() && deq.front() == i - sub_size + 1)
				deq.pop_front();
		}
	}

	for (int j = 0; j < mat_size - sub_size + 1; j++)
	{
		for (int i = 0; i < mat_size - sub_size + 1; i++)
		{
			cout << mins[j][i] << " ";	
		}
		cout << endl;
	}

	return 0;
}