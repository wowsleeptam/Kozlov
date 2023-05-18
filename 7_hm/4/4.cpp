#include <iostream>
#include <algorithm>
#include <chrono>
#include <random>
#include <ratio>
#include <vector>
#include <thread>
#include <execution>
#include "timer.hpp"

int main(){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 100);
  Timer timer;


// Сортировка массива. Время у параллельного алгоритма меньше, чем у обычного
  const int count_of_array = 1000000000;
  std::vector<int> v_1, v_2;
  for (size_t i = 0; i < count_of_array; i++) {
    v_1.push_back(dis(gen));
  }
  std::copy(v_1.begin(), v_1.end(), std::back_inserter(v_2));


  // Ставим вектора в обратном порядке
  std::cout << "Timer for reverse\n";
  timer.start();
  timer.restart();
  std::reverse(std::execution::seq, v_1.begin(), v_1.end());
  std::cout << "Time for seq = " << timer.elapsed() << '\n';
  timer.restart();
  std::reverse(std::execution::par, v_2.begin(), v_2.end());
  std::cout << "Time for par = " << timer.elapsed() << '\n';
  std::cout << std::endl;


// Сортировка
  std::cout << "Timer for sort\n";
  timer.restart();
  std::sort(std::execution::seq, v_1.begin(), v_1.end());
  std::cout << "Time for seq = " << timer.elapsed() << '\n';
  timer.restart();
  std::sort(std::execution::par, v_2.begin(), v_2.end());
  std::cout << "Time for par = " << timer.elapsed() << '\n';
  std::cout << std::endl;




  // Заполненеи массива -1
  std::cout << "Timer for fill\n";
  timer.restart();
  std::fill(std::execution::seq, v_1.begin(), v_1.end(), -1);
  std::cout << "Time for seq = " << timer.elapsed() << '\n';
  timer.restart();
  std::fill(std::execution::par, v_2.begin(), v_2.end(), -1);
  std::cout << "Time for par = " << timer.elapsed() << '\n';




}
