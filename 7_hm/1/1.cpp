#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <cmath>
#include "timer.hpp"

long long func(size_t N){
  long long count_points_inside = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);
  std::cout << "w ";
  // Последовательное
  for (int i = 0; i < N; i++) {
    double x = dis(gen);
    double y = dis(gen);
      if (y <= 1/(1 - std::log(x)))
          count_points_inside++;

  }
  std::cout << count_points_inside << '\n';
  return count_points_inside;
}



int main(){
  Timer timer;
  const int number_of_threads = 8;
  const  long long int count_points = 10000;
  long long count_points_inside = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);
// Последовательное
timer.start();
  for (int i = 0; i < count_points; i++) {
    double x = dis(gen);
    double y = dis(gen);
      if (y <= 1/(1 - std::log(x)))
          count_points_inside++;
  }
  std::cout<< "Numer is " << 1 * (double)count_points_inside/count_points;
std::cout << " Timer is = " <<timer.elapsed() << '\n';



  long long overall_count_of_point_inside = 0;

timer.restart();
std::vector<std::future<long long>> freasult(number_of_threads);


  for (size_t i = 0; i < number_of_threads - 1; i++)
  // Добавил std::launch::async. Плюс работает только 7 потока, а 8-ой работает основной
    freasult[i] = std::async(std::launch::async, func, count_points/(number_of_threads));


  overall_count_of_point_inside += func(count_points/number_of_threads);
  std::cout << "basic thread";

  for (size_t i = 0; i < number_of_threads - 1; i++)
    overall_count_of_point_inside += freasult[i].get();
std::cout << '\n'<< "Number is = " << (double)overall_count_of_point_inside/count_points;
std::cout << " Timer is = " <<timer.elapsed() << '\n';
}
