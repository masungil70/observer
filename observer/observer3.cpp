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
	virtual void update(float temp, float humidity, float pressure) = 0;
};

class ISubject {

public:
	//옵저버 등록 
	virtual void registerObserver(shared_ptr<IObserver> pObserver) = 0;

	//옵저버 제거 
	virtual void removeObserver(shared_ptr<IObserver> pObserver) = 0;

	//변경 사실을 알린다
	virtual void notifyObserver() = 0;
};

class Random {
private:
	// 시드값을 얻기 위한 random_device 생성.
	random_device _rd;

	// random_device 를 통해 난수 생성 엔진을 초기화 한다.
	mt19937 _generator;

	// 예 :  0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
	uniform_int_distribution<int>_distribution;

public:
	Random(int from, int to) : _generator{ _rd() },
		_distribution{from, to} {

	}

	int getValue() const {
		return _distribution(*const_cast<mt19937*>(&_generator));
	}
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

class StatisticsDisplay : public IObserver {
private:
	float _maxTemp = 0.0f;
	float _minTemp = 100.0f;
	float _tempSum = 0.0f;
	int _numReadings = 0;

public:
	void update(float temp, float humidity, float pressure) override {
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

class CurrentConditionsDisplay : public IObserver {
private:
	float _temperature;
	float _humidity;
	float _pressure;

public:
	void update(float temperature, float humidity, float pressure) override {
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

class ForecastDisplay : public IObserver {
private:
	float _currentPressure = 29.92f;
	float _lastPressure;

public:
	void update(float temp, float humidity, float pressure) override {
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

class WeatherData : public ISubject {
private:
	WeatherStation _weatherStation;

	//ISubject 구현시 사용할 멤버변수 
	list<shared_ptr<IObserver>> _list;

	//지역 변수를 멤버 변수화 한다 
	float _temperature;
	float _humidity;
	float _pressure;

public:

	//옵저버 등록 
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
			pObserver->update(_temperature, _humidity, _pressure);
		}
	}


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
		notifyObserver();
	}

	void readMeasurements() {
		_temperature = _weatherStation.getTemperature();
		_humidity = _weatherStation.getHumidity();
		_pressure = _weatherStation.getPressure();
		measurementsChanged();
	}
};

int main(int argc, char** argv) {

	shared_ptr<WeatherData> pWeatherData = make_shared<WeatherData>();
	//출력 장치 객체 생성
	shared_ptr<StatisticsDisplay> pStatisticsDisplay = make_shared<StatisticsDisplay>();
	shared_ptr<CurrentConditionsDisplay> pCurrentConditionsDisplay = make_shared<CurrentConditionsDisplay>();
	shared_ptr<ForecastDisplay> pForecastDisplay = make_shared<ForecastDisplay>();

	//출력 장치를 등록한다
	pWeatherData->registerObserver(pStatisticsDisplay);
	pWeatherData->registerObserver(pCurrentConditionsDisplay);
	pWeatherData->registerObserver(pForecastDisplay);

	//측정값을 읽는다 
	pWeatherData->readMeasurements(); //등록된 3개의 출력 장치에 값을 출력한다
	pWeatherData->readMeasurements();
	pWeatherData->readMeasurements();

	//출력 장치를 제거한다 
	pWeatherData->removeObserver(pForecastDisplay);

	//측정값을 읽는다 
	pWeatherData->readMeasurements(); //등록된 2개의 출력 장치에 값을 출력한다
	pWeatherData->readMeasurements();
	pWeatherData->readMeasurements();

	return 0;
}

