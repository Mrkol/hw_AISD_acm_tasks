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

	for (int j = 0; j < mat_size - sub_size + 1; j++)
	{
		for (int i = 0; i < mat_size - sub_size + 1; i++)
		{
			int m = matrix[j][i];
			for (int i_ = 0; i_ < sub_size; ++i_)
			{
				for (int j_ = 0; j_ < sub_size; ++j_)
				{
					m = min(matrix[j + j_][i + i_], m);
				}
			}
			cout << m << " ";
		}
		cout << endl;
	}

	return 0;
}