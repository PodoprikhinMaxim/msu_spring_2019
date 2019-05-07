#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <sys/stat.h>

using namespace std;
int batch_size = 8;

void mymerge(vector<int>& vec, int start, int mid, int end)
{
	vector<int> one (vec.begin() + start, vec.begin() + mid + 1);
	vector<int> two (vec.begin() + mid + 1, vec.begin() + end + 1);

	int a = 0;
	int b = 0;
	int index = start;
	while (a < one.size() && b < two.size())
	{
		if (one[a] < two[b])
			vec[index ++] = one[a ++];
		else 
			vec[index ++] = two[b ++];
	}
	while (a < one.size())
		vec[index ++] = one[a ++];
	while (b < two.size())
		vec[index ++] = two[b ++];
}

void merge_sort(vector<int>& vec, int start, int end)
{
	if (start >= end)
		return;

	int mid = start + (end - start) / 2;

	thread first(merge_sort, std::ref(vec), start, mid);
	thread second(merge_sort, std::ref(vec), mid + 1, end);
	first.join();
	second.join();
	mymerge(vec, start, mid, end);
}


void mergeF(string inOne, string inTwo, string output) 
{
    ifstream inputOne(inOne);
    ifstream inputTwo(inTwo);
    ofstream mergeFile(output);
    merge(istream_iterator<int>(inputOne),
               istream_iterator<int>(),
               istream_iterator<int>(inputTwo),
               istream_iterator<int>(),
               ostream_iterator<int>(mergeFile, " "));
}

int batch_toF(string strF)
{
	ifstream fin;
	fin.open(strF);
	vector<int> v;
	int a;
	int num_off = 0;
	while(fin >> a)
	{	
		if(v.size() < batch_size)
		{
			v.push_back(a);
		}
		else
		{
			ofstream fout;
			string str = "tmp/tmp"+to_string(num_off)+".txt";
			fout.open(str);
			merge_sort(v, 0, v.size()-1);
			for(int i = 0; i < v.size(); i++)
			{
				fout<<v[i]<<" ";	
			}
			v.clear();
			v.push_back(a);
			num_off++;
		}	
	}
	if(v.size() != 0)
	{
		ofstream fout;
		string str = "tmp/tmp"+to_string(num_off)+".txt";
		fout.open(str);
		merge_sort(v, 0, v.size()-1);
		for(int i = 0; i < v.size(); i++)
		{
			fout<<v[i]<<" ";	
		}
		v.clear();
		v.push_back(a);
		num_off++;
	}
	return num_off;
	
}


int main()
{
	mkdir("tmp", S_IRUSR | S_IWUSR | S_IXUSR);
	int num_off = batch_toF("numbers.txt");
	if(num_off > 2)
	{
		int i = 2;
		mergeF("tmp/tmp0.txt", "tmp/tmp1.txt", "tmp/tmp"+ to_string(num_off) +".txt");
		for(i;i < num_off-1; i++)
		{
			mergeF("tmp/tmp" + to_string(i) + ".txt", 
				"tmp/tmp" + to_string(i + num_off - 2) + ".txt", 
				"tmp/tmp"+ to_string(num_off - 1 + i) +".txt");
		
		}
		mergeF("tmp/tmp" + to_string(i) + ".txt", 
			"tmp/tmp" + to_string(i + num_off - 2) + ".txt", 
			"result.txt");
		
	}
	else
	{
		mergeF("tmp/tmp0.txt", "tmp/tmp1.txt", "result.txt");
	}

	return 0;
}


