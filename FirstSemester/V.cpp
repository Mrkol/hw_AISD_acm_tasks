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

int main()
{
	//I don't like this task one single bit.
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	std::string first_str;
	cin >> first_str;

	vector<Integer> first(first_str.size());
	for (size_t i = 0; i < first_str.size(); ++i)
		first[i] = first_str[i] - '0';

	vector<Integer> result;
	MultiplyPolynomials(first, first, result);



	//for (auto num : result) cout << num << " ";
	//cout << endl;

	Integer answer = 0;
	for (size_t k = 0; k < first.size(); ++k)
	{
		answer += ((result[2*k] - first[k])*first[k])/2;
	}
	cout << answer << endl;
	return 0;
}
