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

int main()
{
	//I don't like this task one single bit.
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);


	const Integer BASE = 10000;
	const Integer BASE_LOG = 4;

	bool sign = false;

	std::string first_str;
	cin >> first_str;
	if (first_str[0] == '-')
	{
		first_str = first_str.substr(1, first_str.size() - 1);
		sign = sign != true;
	}
	if (first_str.size() % BASE_LOG != 0) 
		first_str = string(BASE_LOG - first_str.size() % BASE_LOG, '0') + first_str;

	std::string second_str;
	cin >> second_str;
	if (second_str[0] == '-')
	{
		second_str = second_str.substr(1, second_str.size() - 1);
		sign = sign != true;
	}
	if (second_str.size() % BASE_LOG != 0) 
		second_str = string(BASE_LOG - second_str.size() % BASE_LOG, '0') + second_str;

	vector<Integer> first(first_str.size() / BASE_LOG + !!(first_str.size() % BASE_LOG));
	for (size_t i = 0; i < first.size(); ++i)
	{
		first[i] = stoll(first_str.substr(i*BASE_LOG, BASE_LOG));
	}
	reverse(first.begin(), first.end());

	vector<Integer> second(second_str.size() / BASE_LOG + !!(second_str.size() % BASE_LOG));
	for (size_t i = 0; i < second.size(); ++i)
	{
		second[i] = stoll(second_str.substr(i*BASE_LOG, BASE_LOG));
	}
	reverse(second.begin(), second.end());

	vector<Integer> result;
	MultiplyNumbers(first, second, result, BASE);

	reverse(result.begin(), result.end());
	if (sign && (result.size() > 1 || result[0] != 0)) cout << '-';
	for (size_t i = 0; i < result.size(); ++i)
	{
		if (i != 0) cout << setw(4) << setfill('0') << right;
		cout << result[i];
	}
	cout << endl;
	return 0;
}
