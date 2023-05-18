#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <cmath>


long long func(size_t N){
  long long count_points_inside = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);
  // Последовательное
  for (int i = 0; i < N; i++) {
    double x = dis(gen);
    double y = dis(gen);
      if (y <= 1/(1 - std::log(x)))
          count_points_inside++;
  }

  return count_points_inside;
}



int main(){
  const int number_of_threads = 4;
  const  long long int count_points = 100000000;
  long long count_points_inside = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dis(0, 1.0);
// Параллельное
  for (int i = 0; i < count_points; i++) {
    double x = dis(gen);
    double y = dis(gen);
      if (y <= 1/(1 - std::log(x)))
          count_points_inside++;
  }
  std::cout<< "Numer is " << 1 * (double)count_points_inside/count_points;





  long long overall_count_of_point_inside = 0;


  for (size_t i = 0; i < number_of_threads - 1; i++) {
  std::future<long long> freasult = std::async(std::launch::async, func, count_points/number_of_threads);
  overall_count_of_point_inside += freasult.get();
}
 overall_count_of_point_inside += func(count_points/(4 * number_of_threads));

std::cout << '\n'<< (double)overall_count_of_point_inside/count_points;
}
