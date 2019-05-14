#include <iostream>
#include <thread>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <algorithm>
#include <sys/stat.h>


using namespace std;
const int batch_size = 1000;

void mymerge(vector<uint64_t>& vec, int start, int mid, int end)
{
	vector<uint64_t> one (vec.begin() + start, vec.begin() + mid + 1);
	vector<uint64_t> two (vec.begin() + mid + 1, vec.begin() + end + 1);

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

void merge_sort(vector<uint64_t>& vec, int start, int end)
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


void mergeF(const string input1, const string input2, const string output)
{
	uint64_t x, y;
	ifstream f1(input1, ios::binary);
	ifstream f2(input2, ios::binary);
	ofstream f_res(output, ios::binary);
	if (!f1 || !f2 || !f_res) 
	{
		cout << "Unable to open file"<<'\n';
		return;
	}
	f1.read(reinterpret_cast<char*>(&x), sizeof(uint64_t));
	f2.read(reinterpret_cast<char*>(&y), sizeof(uint64_t));

	while(!f1.eof() && !f2.eof())
	{
		if(x > y)
		{
			f_res.write(reinterpret_cast<char*>(&y), sizeof(uint64_t));
			f2.read(reinterpret_cast<char*>(&y), sizeof(uint64_t));
		} 
		else
		{
			f_res.write(reinterpret_cast<char*>(&x), sizeof(uint64_t));
			f1.read(reinterpret_cast<char*>(&x), sizeof(uint64_t));
		}
	}
	if(f1.eof())
	{
		while(!f2.eof())
		{
			f_res.write(reinterpret_cast<char*>(&y), sizeof(uint64_t));
			f2.read(reinterpret_cast<char*>(&y), sizeof(uint64_t));
		}
	}
	else
	{
		while(!f1.eof())
		{
			f_res.write(reinterpret_cast<char*>(&x), sizeof(uint64_t));
			f1.read(reinterpret_cast<char*>(&x), sizeof(uint64_t));
		}
	}
}

int batch_toF(string strF)
{
	ifstream fin;
	fin.open(strF, ios::binary);
	vector<uint64_t> v;
	uint64_t a;
	int num_off = 0;
	if (!fin) 
	{
		cout << "Unable to open file"<<'\n';
		return num_off;
	}
	while(fin.read(reinterpret_cast<char *>(&a), sizeof(uint64_t)))
	{	
		if(v.size() < batch_size)
		{
			v.push_back(a);
		}
		else
		{
			ofstream fout;
			string str = "tmp/tmp" + to_string(num_off) + ".dat";
			fout.open(str, ios::binary);
			merge_sort(v, 0, v.size()-1);
			for(int i = 0; i < v.size(); i++)
			{
				fout.write(reinterpret_cast<char *>(&v[i]), sizeof(uint64_t));	
			}
			v.clear();
			v.push_back(a);
			num_off++;
		}	
	}
	if(v.size() != 0)
	{
		ofstream fout;
		string str = "tmp/tmp" + to_string(num_off) + ".dat";
		fout.open(str, ios::binary);
		merge_sort(v, 0, v.size()-1);
		for(int i = 0; i < v.size(); i++)
		{
			fout.write(reinterpret_cast<char *>(&v[i]), sizeof(uint64_t));	
		}
		v.clear();
		num_off++;
	}
	return num_off;
	
}

int main()
{
	mkdir("tmp", S_IRUSR | S_IWUSR | S_IXUSR);
	int num_off = batch_toF("numbers.dat");
	if(num_off == 0)
	{
		cout<<"error"<<'\n';
	}
	if(num_off > 2)
	{
		int i = 2;
		mergeF("tmp/tmp0.dat", "tmp/tmp1.dat", "tmp/tmp" + to_string(num_off) + ".dat");
		for(i;i < num_off-1; i++)
		{
			mergeF("tmp/tmp" + to_string(i) + ".dat", 
				"tmp/tmp" + to_string(i + num_off - 2) + ".dat", 
				"tmp/tmp"+ to_string(num_off - 1 + i) +".dat");
		
		}
		mergeF("tmp/tmp" + to_string(i) + ".dat", 
			"tmp/tmp" + to_string(i + num_off - 2) + ".dat", 
			"result.dat");
		
	}
	else
	{
		mergeF("tmp/tmp0.dat", "tmp/tmp1.dat", "result.dat");
	}
	return 0;
}
