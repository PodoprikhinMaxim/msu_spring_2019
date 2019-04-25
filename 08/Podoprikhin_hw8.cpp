#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::condition_variable ready;
const int Stop = 500000;
bool pOP  = false;

void ping_pong(const std::string str, const bool p) 
{
	for (int i = 0; i < Stop; i++) 
	{
		//std::cout<<"kek "<<str<<'\n';
		std::unique_lock<std::mutex> lock(m);
		while(pOP == p) 
		{
			//std::cout<<"l\n";
			ready.wait(lock);
		}
		std::cout << str << '\n';
		pOP = p;
		//std::cout<<"kek2 "<<p<<'\n';
		ready.notify_one();
	}
}



int main(void) 
{
	std::thread thread1(ping_pong, "ping", 1);
	std::thread thread2(ping_pong, "pong", 0);
	thread1.join();
	thread2.join();
	return 0;
}
