#include <iostream>
#include <bitset>
#include <vector>
#include <string>


using MaskType = unsigned long long;
using Integer = unsigned int;

const std::size_t SIZE = 333;

std::bitset<SIZE> tobin(std::string str)
{
	std::bitset<SIZE> result;

	int index = 0;

	std::size_t start = 0;
	while (start < str.size())
	{
		int rem = 0;
		for (size_t i = start; i < str.size(); i++)
		{
			int div = ((str[i] - '0') + 10*rem) / 2;
			rem = ((str[i] - '0') + 10*rem) % 2;
			str[i] = '0' + div;
		}

		while (start < str.size() && str[start] == '0') 
			++start;

		if (rem) result.set(index, 1);

		++index;
	}

	return result;
}


std::vector<std::vector<Integer>> matmult(const std::vector<std::vector<Integer>>& A, 
	const std::vector<std::vector<Integer>>& B, Integer mod)
{
	std::vector<std::vector<Integer>> res(A.size(), std::vector<Integer>(A.size(), 0));
	for (std::size_t i = 0; i < A.size(); ++i)
	{
		for (std::size_t j = 0; j < A.size(); ++j)
		{
			for (std::size_t k = 0; k < A.size(); ++k)
			{
				res[i][j] += A[i][k]*B[k][j];
				res[i][j] %= mod;
			}
		}
	}
	return res;
}

std::vector<std::vector<Integer>> 
	matpow(const std::vector<std::vector<Integer>>& A, std::bitset<SIZE>& n, 
		int i, int mx, Integer mod)
{
	if (i == mx) return A;

	auto B = matpow(A, n, i + 1, mx, mod);
	if (n[i])
	{
		return matmult(matmult(B, B, mod), A, mod);
	}
	else
	{
		return matmult(B, B, mod);
	}
}

int main()
{
	std::bitset<SIZE> width = 0;
	std::size_t height = 0;
	Integer modulo = 0;
	std::string str;
	std::cin >> str >> height >> modulo;
	width = tobin(str);

	const MaskType MAX_MASK = 1ull << height;

	auto get = [](MaskType mask, std::size_t i) -> bool
		{ return mask & (1ull << i); };

	std::vector<std::vector<Integer>> M(MAX_MASK,
		std::vector<Integer>(MAX_MASK, 0));

	for (MaskType i = 0; i < MAX_MASK; ++i)
	{
		for (MaskType j = 0; j < MAX_MASK; ++j)
		{
			bool succ = true;
			for (std::size_t k = 1; k < height; ++k)
			{
				int cnt = get(i, k - 1) + get(i, k)
					+ get(j, k - 1) + get(j, k);
				if (cnt == 0 || cnt == 4)
					succ = false;
			}
			M[i][j] = succ;
		}
	}

	std::vector<Integer> r(MAX_MASK, 1);

	if (width != 1)
	{
		std::bitset<SIZE> widthMinusOne = width;
		int firstOne = 0;
		while (width[firstOne] != 1) ++firstOne;
		widthMinusOne.set(firstOne--, 0);
		while (firstOne >= 0) widthMinusOne.set(firstOne--, 1);

		int mx = widthMinusOne.size() - 1;
		while (widthMinusOne[mx] == 0) --mx;

		
		std::vector<std::vector<Integer>> R = matpow(M, widthMinusOne, 0, mx, modulo);


		/*for (std::size_t i = 0; i < R.size(); ++i)
		{
			for (std::size_t j = 0; j < R.size(); ++j)
			{
				std::cout << R[j][i] << " ";
			}
			std::cout << std::endl;
		}*/

		for (std::size_t i = 0; i < MAX_MASK; ++i)
		{
			r[i] = 0;
			for (std::size_t j = 0; j < MAX_MASK; ++j)
			{
				r[i] += R[i][j];
				r[i] %= modulo;
			}
		}
	}


	Integer result = 0;
	for (auto val : r)
	{
		result += val;
		result %= modulo;
	}
	std::cout << result << std::endl;

	return 0;
}
