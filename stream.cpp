#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <functional>
#include <set>



using namespace std;



template<typename Container>
void populateContainer(Container& cont)
{


	int num;

	while (true) {
		cout << "Enter a number 0 to quit: ";
		cin >> num;
		if (num == 0)
			break;

		cont.push_back(num);
	}

}

//set이라 중복 제거

int main()
{

	vector<int> one;
	set<int> setOne;


	populateContainer(one);


	insert_iterator<set<int>> inserter(setOne, begin(setOne));

	copy_if(cbegin(one), cend(one), inserter,
		[](int i) {return i != 100; });

	copy(cbegin(setOne), cend(setOne), ostream_iterator<int>(cout, " "));

	cout << endl;




	return 0;

}