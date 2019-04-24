#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable ready;
bool check = false;
const int Stop = 500000;

void ping(const std::string& str) 
{
	for (int i = 0; i < Stop; i++) 
	{
		std::unique_lock<std::mutex> lock(m);
		while(check) 
		{
			ready.wait(lock);
		}
		std::cout << str << std::endl;
		check = true;
		ready.notify_one();
	}
}

void pong(const std::string& str) 
{
	for (int i = 0; i < Stop; i++) 
	{
		std::unique_lock<std::mutex> lock(m);
		while(!check) 
		{
			ready.wait(lock);
		}
		std::cout << str << std::endl;
		check = false;
		ready.notify_one();
	}
}

int main(void) 
{
    std::thread ping_(ping, "ping");
    std::thread pong_(pong, "pong");
    ping_.join();
    pong_.join();
    return 0;
}
