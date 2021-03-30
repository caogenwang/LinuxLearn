#include<iostream>
#include<list>
#include<algorithm>
#include<iterator>
 
#include<future>
 
/* 非现场版本，使用函数式编程
template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input)
{
	if (input.empty())
	{
		return input;
	}
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(),
		[&](T const& t){return t<pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(),
		divide_point);
	auto new_lower(
		sequential_quick_sort(std::move(lower_part)));
	auto new_higher(
		sequential_quick_sort(std::move(input)));
	result.splice(result.end(), new_higher);
	//Using synchronization of operations to simplify code
		result.splice(result.begin(), new_lower);
	return result;
}
*/
 
///并行版本
template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input)//并发排序
{
	if (input.empty())
	{
		return input;
	}
	std::list<T> result;
	result.splice(result.begin(), input, input.begin());
	T const& pivot = *result.begin();
	auto divide_point = std::partition(input.begin(), input.end(),
		[&](T const& t){return t<pivot; });
	std::list<T> lower_part;
	lower_part.splice(lower_part.end(), input, input.begin(),
		divide_point);
	std::future<std::list<T> > new_lower(
		std::async(¶llel_quick_sort<T>, std::move(lower_part)));
	auto new_higher(
		parallel_quick_sort(std::move(input)));
	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());
	return result;
}
 
 
int main()
{
	std::list<int> input={ 5, 7, 3, 4, 1, 9, 2, 8, 10, 6 };
	std::list<int>result;
	result = parallel_quick_sort<int>(input);
 
	std::list<int>::iterator i;
 
	for (i = result.begin(); i != result.end(); ++i)
		std::cout << *i << std::endl;
		
 
 
}