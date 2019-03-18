#include <iostream>
#include "numbers.dat"
#include <vector>
#include <math.h>

int main(int argc, char* argv[])
{
	std::vector<int> V;
	for (int i = 1; i < argc; ++i)
	{
		int v = std::atoi(argv[i]);
		V.push_back(v);
	}
	if(argv[1] == NULL)
	{
		return -1;
	}
	if(V.size()%2 != 0)
		return -1;
	else
	{
		std::vector<int> a;
		for (int i = 0; i < 100001; i++)
		{
			a.push_back(1);
		}
		for(int i = 2; i*i <= 100001; i++)
		{
			if(a[i])
			{
				for(int j = i*i; j < 100001; j += i)
				{
					a[j] = 0;
				}
			}
		}
		a[0] = 0;
		a[1] = 0;
		for (int i = 0; i < V.size()-1; i += 2)
		{
			int res = 0;
			int u = V[i];
			int v = V[i+1];
			int j = 0;
			while(Data[j] < u)
			{
				j++;
			}
			if(Data[j] != u)
			{
				j++;
			}
			for(j; Data[j] <= v; j++)
			{	
				if(a[Data[j]])
				{
					res++;
				}			
			}
			std::cout<<res<<'\n';		
		}
	}
	return 0;
}

