#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <cmath>
#include "timer.hpp"
#include <mutex>
#include <utility>


// Функция, которая считает количество точек, удовлетворяющие условию
void func(size_t N, long long& count_points_inside){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);
  std::mutex g_i_mutex;
  for (int i = 0; i < N; i++) {
    double x = dis(gen);
    double y = dis(gen);
      if (y <= 1/(1 - std::log(x))){
        //Захватываю переменную, чтобы изменить ее
        std::lock_guard<std::mutex> lock(g_i_mutex);
        count_points_inside++;
      }
  }
}



int main(){
  Timer timer;
  const int number_of_threads = 2000; // Количесвто используемых потоков
  const  long long count_points = 1000000; // Количество всех точек
  long long overall_count_of_point_inside = 0; // Общее число точек, выполняющийх условию
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);


  std::vector<std::thread> freasult(number_of_threads); // Вектор из потоков


timer.restart();


for (size_t i = 0; i < number_of_threads; i++) {
  // Запускаю потоки по выполенению фукнции
  freasult.at(i) = std::thread(func, count_points/(number_of_threads), std::ref(overall_count_of_point_inside));
  }
for (size_t i = 0; i < number_of_threads; i++) {
  // ну и джойню их всех после завершения
  freasult.at(i).join();
}



std::cout << '\n'<< "Number is = " << (double)overall_count_of_point_inside/count_points;
std::cout << " Timer is = " <<timer.elapsed() << '\n';
}
