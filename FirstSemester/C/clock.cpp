#include <cstdio>
#include <iostream>
#include <fstream>
#include <ctime>
#include <functional>


void time_function(std::function<void(void)> f)
{
	int start = 0;
	int end = 0;

	start = clock();
	f();
	end = clock();

	std::cout << "Function took ";
	std::cout << (end - start);
	std::cout << " clocks (";
	std::cout << ((end - start) / (CLOCKS_PER_SEC / 1000.0));
	std::cout << "ms) to finish." << std::endl;
}

int main(int argc, const char* argw[])
{
	if (argc != 3) return 1;

	int count = 0;
	count = atoi(argw[2]);
	const char* file = argw[1];

	//No more copy-pasta :)

	std::function<void(std::function<void(double&)>)> reader =
		[&count, &file](std::function<void(double&)> read_func)
		{
			freopen(file, "r", stdin);
			double input = 0;
			for (int i = 0; i < count; i++)
			{
				read_func(input);
			}
		};

	std::cout << "Testing std::cin: ";
	time_function(std::bind(reader,
		[](double& input)
		{
			std::cin >> input;
		}));

	std::cout << "Testing scanf: ";
	time_function(std::bind(reader, 
		[](double& input)
		{
			scanf("%lf", &input);
		}));



	return 0;
}