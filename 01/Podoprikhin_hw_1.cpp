#include <iostream>
#include "numbers.dat"
#include <vector>
#include <math.h>

const int n = 100001;

std::vector<int> sieve_Er() 
{
	std::vector<int> sieve (n + 1, 1);
	sieve[0] = 0;
	sieve[1] = 0;
	for (int i = 2; i * i < n + 1; i++)
		if (sieve[i])
		{
			if (i * i <= n)
			{
				for (int j = i * i; j < n + 1; j += i)
				{
					sieve[j] = 0;
				}
			}
		}
	return sieve;
};

void prime_count(std::vector<int>& S, int start, int end) 
{
	int count = 0;
	int j = 0;
	while(Data[j] < start)
	{
		j++;
	}
	if(Data[j] != start)
	{
		j++;
	}
	for(j; Data[j] <= end; j++)
	{	
		if(S[Data[j]])
		{
			count++;
		}			
	}
	std::cout<<count<<'\n';
};

int main(int argc, char* argv[])
{
	if(argv[1] == nullptr || argc%2 == 0)
	{
		return -1;
	}
	else
	{
		std::vector<int> S = sieve_Er();
		for (int i = 1; i < argc - 1; i += 2)
		{
			int res = 0;
			int a = std::atoi(argv[i]);
			int b = std::atoi(argv[i + 1]);
			prime_count(S, a, b);
		}
	}
	return 0;
}

