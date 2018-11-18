#include <iostream>
#include <iomanip>
#include <cmath>
#include <complex>
#include <string>
#include <algorithm>
#include <vector>


using namespace std;


using Integer = long long;
using Real = long double;
using Complex = complex<Real>;

const Real PI = acos(-1);

template<bool INVERSE>
void FastFourierTransform(vector<Complex>& vec)
{
	int n = static_cast<int>(vec.size());
	for (int i = 1, j = 0; i < n; ++i)
	{
		int mask = n >> 1;
		while (j & mask) 
		{
			j ^= mask;
			mask >>= 1;
		}
		j ^= mask;
		if (j < i) swap(vec[i], vec[j]);
	}

	for (int segment_length = 2; segment_length <= n; segment_length <<= 1)
	{
		const Real angle = 2*PI/segment_length * (INVERSE ? -1 : 1);
		const Complex omega1(cos(angle), sin(angle));

		for (int segment_start = 0; segment_start < n; segment_start += segment_length)
		{
			Complex omega(1);
			for (int segment_offset = 0; segment_offset < segment_length/2; ++segment_offset)
			{
				Complex a = vec[segment_start + segment_offset];
				Complex bc = vec[segment_start + segment_offset + segment_length/2]*omega;
				vec[segment_start + segment_offset] = a + bc;
				vec[segment_start + segment_offset + segment_length/2] = a - bc;
				omega *= omega1;
			}
		}
	}

	if (INVERSE)
	{
		for (auto& num : vec) num /= n;
	}
}

void MultiplyPolynomials(const vector<Integer>& first, const vector<Integer>& second, vector<Integer>& result)
{
	size_t newsize = 1;
	while (newsize < first.size() || newsize < second.size()) newsize <<= 1;
	newsize <<= 1;
	vector<Complex> first_copy(newsize, 0);
	vector<Complex> second_copy(newsize, 0);
	for (size_t i = 0; i < first.size(); ++i) first_copy[i] = Complex(first[i]);
	for (size_t i = 0; i < second.size(); ++i) second_copy[i] = Complex(second[i]);

	FastFourierTransform<false>(first_copy);
	FastFourierTransform<false>(second_copy);
	for (size_t i = 0; i < newsize; ++i) first_copy[i] *= second_copy[i];
	FastFourierTransform<true>(first_copy);

	result.resize(newsize);
	for (size_t i = 0; i < newsize; ++i) result[i] = Integer(first_copy[i].real() + 0.5);
}



void MultiplyNumbers(const vector<Integer>& first, 
	const vector<Integer>& second, 
	vector<Integer>& result, Integer base)
{
	/*if (first.size() * second.size() < 10)
	{

		for (int i = 0; i < second.size(); ++i)
		{

		}
		return;
	}*/
	MultiplyPolynomials(first, second, result);
	Integer overflow = 0;
	for (size_t i = 0; i < result.size(); ++i)
	{
		result[i] += overflow;
		overflow = result[i] / base;
		result[i] %= base;
	}
	size_t realsize = result.size();
	while (realsize > 1 && result[realsize - 1] == 0) realsize--;
	result.resize(realsize);
}

void Add(const vector<Integer>& first, 
	const vector<Integer>& second, 
	vector<Integer>& result, Integer base)
{
	result.resize(max(first.size(), second.size()) + 1);
	Integer overflow;
	for (int i = 0; i < result.size(); ++i)
	{
		result[i] = 
			(i < first.size() ? first[i] : 0) +
			(i < second.size() ? second[i] : 0) + 
			overflow;
		
		overflow = result[i] / base;
		result[i] %= base;
	}
}

void ToRadix(string::iterator begin, string::iterator end, 
	vector<Integer>& first, vector<Integer>& second, Integer base)
{
	if (end - begin == 1)
	{
		result = (*begin) - '0';

		return;
	}
	bitset<2000000> left;
	bitset<2000000> right;
	bitset<2000000> power;
	ToBinarry(begin, begin + (end - begin)/2, pow / 2);
	ToBinarry(begin + (end - begin)/2, end);
	MultiplyNumbers(left, pow, result, base);
	
}

int main()
{
	//I don't like this task one single bit.
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);


	const Integer BASE = 10000;
	const Integer BASE_LOG = 4;

	return 0;
}
