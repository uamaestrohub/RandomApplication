#include <limits>
#include "containerutils.h"
#include <set>


#define MAX_ERASE_COUNT 15
#define MAX_ELEMENT_VALUE 9

int main(int argc, char *argv[])
{
	std::vector<unsigned short> vector;
	std::map<size_t, unsigned short> map;
	{
		const size_t containerSize = []()->size_t {
			bool isValid = false;
			int value = 0;
			///expect the input of an integer > 0
			std::cin.exceptions(std::ios::failbit | std::ios::badbit);
			do
			{
				try
				{
					std::cout << "Please set container size:" << std::endl;
					std::cin >> value;
					if (value <= 0)
						throw "The number must be greater than 0!";
					isValid = true;
				}
				catch (const std::ios_base::failure & fail)
				{
					std::cout << fail.what() << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
				catch (const char* error)
				{
					std::cout << error << std::endl;
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				}
			} while (!isValid);
			return static_cast<size_t>(value);
		}();

		///fill containers with random numbers
		ContainerOperation::fillContainer(vector, MAX_ELEMENT_VALUE, containerSize);
		ContainerOperation::fillContainer(map, MAX_ELEMENT_VALUE, containerSize);
	}

	std::cout << "=================================================" << std::endl;
	std::cout << " container fill" << std::endl;
	std::cout << "=================================================" << std::endl;
	std::cout << "Vector: " << vector.size() << std::endl;
	ContainerOperation::printContainer(vector);
	std::cout << "Map: " << map.size() << std::endl;
	ContainerOperation::printContainer(map);

	///delete MAX_ERASE_COUNT items
	{
		ContainerOperation::removeRandomElements(vector, MAX_ERASE_COUNT);
		ContainerOperation::removeRandomElements(map, MAX_ERASE_COUNT);
	}

	std::cout << "=================================================" << std::endl;
	std::cout << " Removed random items" << std::endl;
	std::cout << "=================================================" << std::endl;

	std::cout << "Vector: " << vector.size() << std::endl;
	ContainerOperation::printContainer(vector);
	std::cout << "Map: " << map.size() << std::endl;
	ContainerOperation::printContainer(map);

	/*synchronize using the set, because the elements of the containers are ushort,
	and their value is from 0 to 9, then the maximum number of elements in the set can be 10,
	which is not a significant memory expense*/
	[&map, &vector]() {
		std::set<unsigned short> setFromMap;
		for (const auto& iter : map)
			setFromMap.insert(iter.second);

		std::set<unsigned short> setFromVector;
		for (const auto& iter : vector)
			setFromVector.insert(iter);

		if (setFromMap == setFromVector)
			return;

		/// get the difference between containers
		std::set<unsigned short> diff;
		std::set_symmetric_difference(setFromVector.begin(), setFromVector.end(), setFromMap.begin(), setFromMap.end(),
			std::inserter(diff, diff.begin()));

		/// sinchronization containers
		vector.erase(std::remove_if(vector.begin(), vector.end(),
			[&diff](const int& iter) {return diff.find(iter) != diff.end(); }), vector.end());

		for (auto iter = map.begin(); iter != map.end(); )
		{
			if (diff.find(iter->second) == diff.end())
				++iter;
			else
				iter = map.erase(iter);
		}
	}();

	std::cout << "=================================================" << std::endl;
	std::cout << " syncronized" << std::endl;
	std::cout << "=================================================" << std::endl;
	std::cout << "Vector: " << vector.size() << std::endl;
	ContainerOperation::printContainer(vector);
	std::cout << "Map: " << map.size() << std::endl;
	ContainerOperation::printContainer(map);

	std::cout << std::endl;

	return EXIT_SUCCESS;
}
