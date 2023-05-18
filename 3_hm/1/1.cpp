#include<iostream>
#include<cstdlib>
#include<random>
#include<ctime>
#include<vector>
#include <unordered_set>
#include "timer.hpp"
#include <map>
#include <fstream>

// Компаратор для с функции sort
bool cmp(const std::pair<int, double> l, std::pair<int, double> r){return l.first < r.first;}

int main(){
  // Запись данных в файл
  std::ofstream out;          // поток для записи
  out.open("D:\\hello.txt"); // окрываем файл для записи
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_real_distribution<> uniform_distr(1,4);

// Величины
  unsigned int count;
  std::cin >> count;

  for (size_t N = 0; N < count; N+=50) {
// Задаем массив ключей от 0 до N-1 и перемешиваем его. Также задаем массив, из которого будем брать элементы. Создаем
// map и vector
  std::vector<int> array_of_keys(N);
  std::iota(array_of_keys.begin(), array_of_keys.end(), 0);
  std::shuffle(array_of_keys.begin(), array_of_keys.end(), engine);
  std::unordered_set<double> array_start;
  std::map<int, double> array_map;
  std::vector<std::pair<int, double>> array_vector;

// Заполняю множество рандомными числами
  while (array_start.size() < N) {
    array_start.insert(uniform_distr(engine));}

  Timer timer;
// Вставка в std::map
  size_t iterator = 0;
  timer.start();
  for (auto elem: array_start)  {array_map.insert({iterator, elem}); iterator++;}
  long long  period_map_fill = timer.elapsed();

// Вставка в вектор
  iterator = 0;
  timer.restart();
  for (auto elem: array_start){
    array_vector.push_back(std::pair<int, double>(array_of_keys.at(iterator),elem));
    iterator++;}
  long long  period_vector_fill = timer.elapsed();

// Сортировка вектора
  timer.restart();
  sort(array_vector.begin(), array_vector.end(), cmp);
  long long period_vector_sort = timer.elapsed();
  std::cout << N  << ' '<< period_map_fill << ' ' << period_vector_fill << ' '<< period_vector_sort   <<std::endl;



  out << N  << ' '<< period_map_fill << ' ' << period_vector_fill << ' '<< period_vector_sort   <<std::endl;
}

}
