#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

void conv_bin_to_txt(string file_bin,string file_txt,int batch_size)
{
	ifstream fin(file_bin, ios::binary);
	ofstream fout(file_txt);
	if (!fout || !fin)
	{
		cout << "can not open file" << endl;
	}
	vector<uint64_t> v;
	uint64_t val;
	int i = 0;
	while(fin.read(reinterpret_cast<char *>(&val), sizeof(val)))
	{	
		if(v.size() < batch_size)
		{
			v.push_back(val);
		}
		else
		{
			for(int i = 0; i < v.size(); i++)
			{
				fout << v[i] << ' ';
			}
			v.clear();
			v.push_back(val);
		}	
	}
	if(v.size() != 0)
	{
		for(int i = 0; i < v.size(); i++)
		{
			fout << v[i] << ' ';	
		}
		v.clear();
	}
}

void conv_txt_to_bin(string file_bin, string file_txt,int batch_size)
{
	ifstream fin(file_txt);
	ofstream fout(file_bin, ios::binary);
	if (!fout || !fin)
	{
				cout << "can not open file" << endl;
	}
	vector<uint64_t> v;
	uint64_t val;
	int i = 0;
	while(fin >> val)
	{	
		if(v.size() < batch_size)
		{
			v.push_back(val);
		}
		else
		{
			for(int i = 0; i < v.size(); i++)
			{
				fout.write(reinterpret_cast<const char *>(&v[i]), sizeof(uint64_t));
			}
			v.clear();
			v.push_back(val);
		}	
	}
	if(v.size() != 0)
	{
		for(int i = 0; i < v.size(); i++)
		{
			fout.write(reinterpret_cast<const char *>(&v[i]), sizeof(uint64_t));	
		}
		v.clear();
	}
}
