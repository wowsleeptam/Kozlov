#include <chrono>
#include <iostream>
#include <ratio>
#include <vector>
#include <algorithm>
#include <thread>

// enum class Period{std::chrono::millisecond, std::chrono::microseconds, std::chrono::nanoseconds, std::chrono::seconds};
enum class Period{sec, millisec, microsec, nanosec};


// Переделал аргументы оператора switch: принимает не int, а Period
long long form(std::chrono::duration<double> a, Period format = Period::microsec){
	switch (format) {
		case Period::sec:
			return a.count();
			break;
		case Period::millisec:
			return std::chrono::duration_cast<std::chrono::milliseconds>(a).count();
			break;
		case Period::microsec:
			return std::chrono::duration_cast<std::chrono::microseconds>(a).count();
			break;
		case Period::nanosec:
			return std::chrono::duration_cast<std::chrono::nanoseconds>(a).count();
			break;
}
return 0;
}


// Класс таймера
class Timer{
private:
  std::chrono::time_point<std::chrono::steady_clock> m_start;
  std::chrono::time_point<std::chrono::steady_clock> m_start_pause;
	std::chrono::time_point<std::chrono::steady_clock> m_end; // Чтобы не захломлять код, объявлю m_end тут. Несколько раз понадобиться в коде
  std::chrono::duration<double> m_elapsed;
  bool m_is_pause_started = 0; // Счетчик того, идет ли сейчас пауза или нет

public:
  void start(){
    m_start = std::chrono::steady_clock::now();}

  void stop(){
    m_is_pause_started = 1; // Счетчик паузы включен
    m_start_pause = std::chrono::steady_clock::now();}


// Теперь идея состоит в том, что мы как бы "передвигаем" счетчик временим к продолжению таймера, но нужно учесть
// то засчитанное время вне паузы
  void resume(){
		m_is_pause_started = 0; // Счетчик паузы выключен
    m_end = std::chrono::steady_clock::now();
    m_start = m_end - (m_start_pause - m_start);
}


  void restart(){
		m_is_pause_started = 0; // Обнуляю счетчик паузы
  	m_start = std::chrono::steady_clock::now();}


// Теперь elapsed выводит тип long long
  long long elapsed(Period format = Period::microsec){
		if (m_is_pause_started) m_elapsed = m_start_pause - m_start;
		else {
			m_end = std::chrono::steady_clock::now();
			m_elapsed = m_end - m_start;}
			m_start = std::chrono::steady_clock::now();
			return form(m_elapsed, format);}
};
