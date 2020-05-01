#include "hash_map.h"
#include <iostream>
#include <map>


using namespace Proc;
using namespace std;


int main()
{
	
	hash_map<string, int> myHash{
		{"one", 100},
		{"two", 200} };

	myHash.insert({
		{"three", 300},
		{"Four", 400} });

	for (auto it = myHash.cbegin(); it != myHash.cend(); ++it)
	{
		cout << it->first << ", " << (*it).second << endl;
	}

	cout << "-------------" << endl;


	for (auto& p : myHash) {
		cout << p.first << " maps to " << p.second << endl;
	}
	cout << "-------------" << endl;

	for (auto&[key, value] : myHash)
	{
		cout << key << " maps to " << value << endl;
	}

	cout << "-------------" << endl;

	map<string, int> myMap(cbegin(myHash), cend(myHash));
	for (auto& p : myMap)
	{
		cout << p.first << " maps to " << p.second << endl;
	}


	cout << "-------------" << endl;


	auto found = myHash.find("three");
	if (found != end(myHash))
	{
		cout << "Found three value: " << found->second << endl;
	}

	cout << "-------------" << endl;

	map<string, int> some{ {"one",1},{"two",2} };
	hash_map<string, int> hmap(cbegin(some), cend(some));


	hash_map<string, int> myHash2;
	myHash.swap(myHash2);

	hash_map<string, int> myHash3(std::move(myHash2));

	cout << myHash3.size() << endl;
	cout << myHash3.max_size() << endl;





	return 0;

}