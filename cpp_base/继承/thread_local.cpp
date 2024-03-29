#include <thread>
#include <mutex>
#include <iostream>
#include <string>
 
///
// reference: https://en.cppreference.com/w/cpp/language/storage_duration
namespace {
 
thread_local unsigned int rage = 1;
std::mutex cout_mutex;
 
void increase_rage(const std::string& thread_name)
{
	++rage; // modifying outside a lock is okay; this is a thread-local variable, 每个线程有一个副本
	std::lock_guard<std::mutex> lock(cout_mutex);
	fprintf(stdout, "Rage counter for: %s : %d\n", thread_name.c_str(), rage); // : 2, 子线程中rage的值为2
}
 
} // namespace
 
int main()
{
	std::thread a(increase_rage, "a"), b(increase_rage, "b");
 
	{
		std::lock_guard<std::mutex> lock(cout_mutex);//这里加锁主要是输出的时候不要乱
		fprintf(stdout, "Rage counter for main: %d\n", rage); // 1, 主线程中rage的值始终为1
	}
	a.join();
	b.join();
	return 0;
}