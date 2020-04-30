#include <vector>
#include <iostream>
#include <iterator>


using namespace std;



class MoveableClass
{
public:

	MoveableClass() {
		cout << "기본 생성자입니다." << endl;
	}

	MoveableClass(const MoveableClass&) {
		cout << "복사 생성자입니다." << endl;
	}
	MoveableClass(const MoveableClass&&) {
		cout << "이동 생성자입니다." << endl;
	}

	MoveableClass& operator=(const MoveableClass&) {
		cout << "복사 할당 연산자입니다." << endl;
		return *this;
	}
	MoveableClass& operator=(const MoveableClass&&) {
		cout << "이동 대입 연산자입니다." << endl;
		return *this;
	}


};


int main()
{
	vector<MoveableClass> vec;
	MoveableClass mc;

	
	vec.push_back(mc);
	vec.push_back(mc);

	cout << "-----------" << endl;

	//vec원소를 one에 이동
	vector<MoveableClass> one(cbegin(vec), cend(vec));


	vector<MoveableClass> two(make_move_iterator(begin(vec)), make_move_iterator(end(vec)));

	cout << "-------------" << endl;















}
