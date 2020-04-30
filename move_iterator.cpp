#include <vector>
#include <iostream>
#include <iterator>


using namespace std;



class MoveableClass
{
public:

	MoveableClass() {
		cout << "�⺻ �������Դϴ�." << endl;
	}

	MoveableClass(const MoveableClass&) {
		cout << "���� �������Դϴ�." << endl;
	}
	MoveableClass(const MoveableClass&&) {
		cout << "�̵� �������Դϴ�." << endl;
	}

	MoveableClass& operator=(const MoveableClass&) {
		cout << "���� �Ҵ� �������Դϴ�." << endl;
		return *this;
	}
	MoveableClass& operator=(const MoveableClass&&) {
		cout << "�̵� ���� �������Դϴ�." << endl;
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

	//vec���Ҹ� one�� �̵�
	vector<MoveableClass> one(cbegin(vec), cend(vec));


	vector<MoveableClass> two(make_move_iterator(begin(vec)), make_move_iterator(end(vec)));

	cout << "-------------" << endl;















}
