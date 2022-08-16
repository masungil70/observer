// observer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <memory>
#include <random>
#include <ctime>
#include <functional>

using namespace std;

//rand()
/* 1

1 : 10%
2 : 10% 
3 : 10% 
4
5
6
7
8
9
10 : 10%

~ 10 */

class Random {
private:
	// 시드값을 얻기 위한 random_device 생성.
	random_device _rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	mt19937 _generator;

	// 예 :  0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	uniform_int_distribution<int> _distribution;

public:
	Random(int from, int to) : _generator{ _rd() },
		_distribution{from, to} {

	}

	int getValue() const {
		return _distribution(*const_cast<mt19937*>(&_generator));
	}
	//int getValue() const {
	//	return _distribution(*const_cast<mt19937*>(&_generator));
	//}
};

class WeatherStation {
private:
	Random _randomTemperature; //온도 난수 객체 
	Random _randomHumidity; //습도 난수 객체 
	Random _randomPressure; //압력 난수 객체 

public :
	WeatherStation() : _randomTemperature{ -50, 50 }
		, _randomHumidity{ -100, 100 } 
		, _randomPressure{ -100, 100 } {
	}

	float getTemperature() const {
		return 25.0f + _randomTemperature.getValue() / 10.0f;
	}
	float getHumidity() const {
		return 60.0f + _randomHumidity.getValue() / 10.0f;
	};
	float getPressure() const {
		return 25.0f +  _randomPressure.getValue() / 10.0f;
	};

};

class CurrentConditionsDisplay {
private:
	float _temperature;
	float _humidity;
	float _pressure;

public:
	void update(float temperature, float humidity, float pressure) {
		_temperature = temperature;
		_humidity = humidity;
		_pressure = pressure;
		display();
	}

	void display() {
		cout << "현재 조건 " << endl
			<< "온도: " << _temperature << "℃" << endl
			<< "습도: " << _humidity << "%" << endl
			<< "기압: " << _pressure << endl << endl;

	}
};


class StatisticsDisplay {
private:
	float _maxTemp = 0.0f;
	float _minTemp = 100.0f;
	float _tempSum = 0.0f;
	int _numReadings = 0;

public:
	void update(float temp, float humidity, float pressure) {
		_tempSum += temp;
		_numReadings++;

		if (temp > _maxTemp) {
			_maxTemp = temp;
		}

		if (temp < _minTemp) {
			_minTemp = temp;
		}

		display();
	}

	void display() {
		cout << "기상 통계 " << endl
			<< "평균 기온 : " << (_tempSum / _numReadings) << "℃" << endl
			<< "최저 기온 : " << _minTemp << "℃" << endl
			<< "최고 기온 : " << _maxTemp << "℃" << endl << endl;
	}
};


class ForecastDisplay {
private:
	float _currentPressure = 29.92f;
	float _lastPressure;

public:
	void update(float temp, float humidity, float pressure) {
		_lastPressure = _currentPressure;
		_currentPressure = pressure;

		display();
	}

	void display() {
		cout << "기상 예보" << endl;
		if (_currentPressure > _lastPressure) {
			cout << "가는 길에 날씨 개선" << endl << endl;
		}
		else if (_currentPressure == _lastPressure) {
			cout << "전과 같음" << endl << endl;
		}
		else if (_currentPressure < _lastPressure) {
			cout << "선선하고 비오는 날씨에 조심하십시오" << endl << endl;
		}
	}
};

class WeatherData {
private:
	WeatherStation _weatherStation;
	StatisticsDisplay _statisticsDisplay;
	CurrentConditionsDisplay _currentConditionsDisplay;
	ForecastDisplay _forecastDisplay;

public:
	float getTemperature() const {
		return _weatherStation.getTemperature();
	}

	float getHumidity() const {
		return _weatherStation.getHumidity();
	}

	float getPressure() const {
		return _weatherStation.getPressure();
	}

	void measurementsChanged() {
		float temperature = _weatherStation.getTemperature();
		float humidity  = _weatherStation.getHumidity();
		float pressure  = _weatherStation.getPressure();

		_statisticsDisplay.update(temperature, humidity, pressure);
		_currentConditionsDisplay.update(temperature, humidity, pressure);
		_forecastDisplay.update(temperature, humidity, pressure);
	}


};

//const int arr[] = { 1,2,3,4,5 };
//
///*변수 : read only , read/write, */
//void strcpy11(char* _Destination, char const * _Source) {
//	*_Source = 'A';
//	for (; *_Source;) {
//		*_Destination = *_Source;
//		_Destination++;
//		_Source++;
//	}
//}
//void strcpy12(char* const _Destination, char const* const _Source) {
//	//_Destination += 10;
//	//_Source += 100;
//	for (int idx = 0; _Source[idx] ; idx++) {
//		_Destination[idx] = _Source[idx];
//	}
//	
////	*(const_cast<char*>(_Source)) = 'A';
//}

//class A {
//private:
//	int a;
//	int b;
//public:
//	//void funcA(int a) {
//	void funcA(int a) const {
//
//		this->a = a;
//	}
//};

int main(int argc, char** argv) {
	//A obj;
	//obj.funcA(10);
	//func_name(&obj, 10);

	int a = 10;
	int sum = 10;
	const int b = a; //b 
	const int* c = &a; //c, *c 
	const int* const d = &a;//d, *d
	
	cout << "step1 b = " << b << endl;
	*const_cast<int*>(&b) = 5;
	cout << "step2 b = " << b << endl;

	//b = 20;
	//c = &sum;
	//*c = 20;
	//c[0] = 20;
	//d = &sum;
	//*d = 20;
	//d[0] = 20;


	WeatherData weatherData;

	weatherData.measurementsChanged();
	weatherData.measurementsChanged();
	weatherData.measurementsChanged();

	return 0;
}

