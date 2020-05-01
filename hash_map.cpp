#include "hash_map.h"
#include <iostream>


using namespace Proc;
using namespace std;


int main()
{
	hash_map<int, int> myHash;

	myHash.insert(make_pair(4, 40));
	myHash.insert(make_pair(6, 60));


	auto x = myHash.find(4);
	if (x != nullptr) {
		cout << "4 maps to " << x->second << endl;
	}
	else {
		cout << "failed to find 4" << endl;
	}



	myHash.erase(4);

	auto x2 = myHash.find(4);
	if (x2 != nullptr) {
		cout << "4 maps to " << x2->second << endl;
	}
	else {
		cout << "Failed to find 4" << endl;
	}


	myHash[4] = 35;
	myHash[4] = 60;


	auto x3 = myHash.find(4);
	if (x3 != nullptr) {
		cout << "4 maps to " << x3->second << endl;
	}
	else {
		cout << "Failed to find 4" << endl;
	}
	// std::swap() test

	hash_map<int, int> other(std::equal_to<>(), 11);
	swap(other, myHash);

	


	// copy constructor, assign 
	hash_map<int, int> myHash2(other);
	hash_map<int, int> myHash3;
	myHash3 = myHash2;

	// move constructor, assign
	hash_map<int, int> myHash4(std::move(myHash3));
	hash_map<int, int> myHash5;
	myHash5 = std::move(myHash4);


	return 0;

}