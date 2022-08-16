// observer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <memory>
#include <random>
#include <ctime>
#include <functional>
#include <list>

using namespace std;

class IObserver {
public:
	virtual void update(int value) = 0;
};

class Subject {
private:
	list<shared_ptr<IObserver>> _list;
	int _value;

public:
	//옵저버 등록 
//	void addObserver(shared_ptr<IObserver> pObserver) {
	void registerObserver(shared_ptr<IObserver> pObserver) {
			_list.push_back(pObserver);
	}

	//옵저버 제거 
	void removeObserver(shared_ptr<IObserver> pObserver) {
		_list.remove(pObserver);
	}

	//변경 사실을 알린다
	void notifyObserver() {
		for (auto& pObserver : _list) {
			pObserver->update(_value);
		}
	}

	void setValue(int value) {
		//값을 변경한다 
		_value = value;
		//모든 구독자에 변경 사실을 알린다
		notifyObserver();
	}
};

class Dog : public IObserver {
public:
	virtual void update(int value) override {
		cout << "Dog::update()" << endl;
		cout << " value = " << value << endl << endl;
	}
};

class Cat : public IObserver {
public:
	virtual void update(int value) override {
		cout << "Cat::update()" << endl;
		cout << " value = " << value << endl << endl;
	}
};

class Mouse : public IObserver {
public:
	virtual void update(int value) override {
		cout << "Mouse::update()" << endl;
		cout << " value = " << value << endl << endl;
	}
};

class Duck : public IObserver {
public:
	virtual void update(int value) override {
		cout << "Duck::update()" << endl;
		cout << " 별 = ";
		for (int i = 0; i < value; i++) {
			cout << "*";
		}
		cout << endl;
	}
};

int main(int argc, char** argv) {

	Subject subject;
	shared_ptr<Dog> pDog = make_shared<Dog>();
	shared_ptr<Cat> pCat = make_shared<Cat>();
	shared_ptr<Mouse> pMouse = make_shared<Mouse>();
	shared_ptr<Duck> pDuck = make_shared<Duck>();

	//주제 객체에 등록 
	subject.registerObserver(pDog);
	subject.registerObserver(pCat);
	subject.registerObserver(pMouse);
	subject.registerObserver(pDuck);

	//값을 변경한다 
	cout << "값 10으로 변경 후 " << endl;
	subject.setValue(10); //내부에서 변경 사실을 통보 받을 객체를 호출한다. 
	//Dog, Cat, Mouse 클래스의 update() 메서드 호출된다 

	//통보 받을 객체를 제거한다 
	subject.removeObserver(pCat);

	cout << "값 20으로 변경 후 " << endl;

	subject.setValue(20); //내부에서 변경 사실을 통보 받을 객체를 호출한다. 
	//위에서 Cat 클래스 제거 했기 때문에 
	//Dog, Mouse 클래스의 update() 메서드 호출된다 

	return 0;
}

