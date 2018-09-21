/*
used a random number generator with Ñ++ 11. 
link: https://en.cppreference.com/w/cpp/numeric/random/uniform_int_distribution
*/

#ifndef CONTAINERUTILS_H
#define CONTAINERUTILS_H

#include <typeinfo>
#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <algorithm>
#include <iterator>

namespace ContainerOperation {
	///Required for static_assert in the fillcontainer
	template<class Container>
	struct FalseType : std::false_type{ };

	///If the container is not a map or vector, we call this method
	template<class Container>
	void fillContainer(Container& /*container*/, const unsigned short /*mValue*/, const size_t /*size*/)
	{
		static_assert(FalseType<Container>::value, "Container doesn't supported");	
	}
	///if the container is a vector, fill it here.
	template<>
	void fillContainer<std::vector<unsigned short>>(std::vector<unsigned short>& vector, const unsigned short mValue, const size_t size)
	{
		vector.resize(size);
		std::mt19937 generator{ std::random_device()() };
		std::uniform_int_distribution<unsigned short> uid(0, mValue);
		std::generate(vector.begin(), vector.end(), [&uid, &generator]()->unsigned short {return uid(generator); });
	}
	///if container is map, fill it here.
	template<>
	void fillContainer<std::map<size_t, unsigned short>>(std::map<size_t, unsigned short>& map, const unsigned short mValue, const size_t size)
	{
		std::mt19937 generator{ std::random_device()() };
		std::uniform_int_distribution<unsigned short> uid(0, mValue);
		std::generate_n(std::inserter(map, map.begin()), size, [&]() {return std::make_pair(map.size(), uid(generator)); });
	}
	///for any container other than the vector, call this method to remove the random number of the element
	template<class Container>
	void removeRandomElements(Container& container, const size_t mCount)
	{
		std::mt19937 generator{ std::random_device()() };
		std::uniform_int_distribution<unsigned int> uid(0, std::min(container.size(), mCount));
		auto iter = container.begin();
		std::advance(iter, uid(generator));
		container.erase(container.begin(), iter);
	}
	///If the container is a vector, call this method to remove the random number of the element
	template<>
	void removeRandomElements<std::vector<unsigned short>>(std::vector<unsigned short>& vector, const size_t mCount)
	{
		std::mt19937 generator{ std::random_device()() };
		std::uniform_int_distribution<unsigned int> uid(0, std::min(vector.size(), mCount));
		for (int i = uid(generator); i > 0; --i)
			vector.pop_back();
	}
	/// print any container other then map
	template<class Container>
	void printContainer(const Container& container)
	{
		std::cout << "--------------------------------------------------";
		std::cout << std::endl;
		for (const auto& iter : container)
			std::cout << iter << ", ";
		std::cout << std::endl;
		std::cout << "--------------------------------------------------";
		std::cout << std::endl;
	}
	
	///print map
	template<class T, class A>
	void printContainer(const std::map<T, A>& map)
	{
		std::cout << "--------------------------------------------------";
		std::cout << std::endl;
		for (const auto& iter : map)
			std::cout << iter.first << "=>" << iter.second << ", ";
		std::cout << std::endl;
		std::cout << "--------------------------------------------------";
		std::cout << std::endl;
	}
	
}

#endif // CONTAINERUTILS_H
