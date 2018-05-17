#include <thread>
#include <iostream>
#include <unistd.h>
#include <signal.h>

int a = 5;

int thread1(int n)
{
	a = 8;
	//sleep(1);
	sleep(20);
	//sleep(1);
	return 0;
}

int thread2(int n)
{
	a = 3;
	//sleep(1);
	sleep(20);
	//sleep(1);
	return 0;
}

void sigi(int s)
{
	std::cout << "sig" << std::endl;
}

int main(void)
{
	std::thread t1(thread1, 1);
	std::thread t2(thread2, 2);
	t1.detach();
	t2.detach();
	std::cout << a << std::endl;
	
	while (1);
	return 0;
}