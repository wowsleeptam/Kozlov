#include<iostream>
#include<cstdlib>
#include<random>
#include<ctime>
#include<vector>
#include <set>
#include <cassert>
#include <boost/functional/hash.hpp>
#include <array>
#include "algos.hpp"
#include <fstream>



int main(){
  std::ofstream out;          // поток для записи
  out.open("D:\\3.txt"); // окрываем файл для записи

// Задаем множество строчек, фиксируем нижнюю и верхнюю границы, так же фиксируем границы, которые недопустимы.
  std::set<std::string> set_of_strings;
  std::random_device rd;
  std::mt19937 engine(rd());
  unsigned int N_count;
  const int down_bound = 48;
  const int up_bound = 90;
  const int down_forrbiben_bound = 58;
  const int up_forrbiben_bound = 64;
  std::uniform_int_distribution<> distribution_of_values(down_bound,up_bound);

  std::cin >> N_count;
for (size_t N = 0; N < N_count; N+=100)
{
  while (set_of_strings.size() < N) {
    std::string string;
    int value = 0;
    // создаем строчку из 6 элементов, сторочка содерижт цифры и латин. буквы верхнего регистра
    for (size_t i = 0; i < 6; i++) {
      value = distribution_of_values(engine);
      // Если у меня число попадает в запретный промежуток, то я его перерегенириую. Я не нашел никакого способа, как
      // можно вставить в распределение два отдельные промежутка (в данном случае у числа от 48 до 57, а буквы верхнего
      // регистра от 65 до 90), поэтому я без понятия, как одновременно рассматривать распределение на двуз различных промежутках
      while ((value >= down_forrbiben_bound) && (value <= up_forrbiben_bound)) value = distribution_of_values(engine);
      string += static_cast<char>(value);
    }
    // Вставляю строку в массив
    set_of_strings.insert(string);
    string.clear();
}
// Массив, который состоит из 9 set, в котором хронится хэш. Каждый массив соответствует своей сортировке
std::array<std::set<int>,9> array_of_hash;
  for (auto& elem: set_of_strings){
    const char *ptr = elem.c_str();
    array_of_hash[0].insert(RSHash(ptr, elem.size()));
    array_of_hash[1].insert(JSHash(ptr, elem.size()));
    array_of_hash[2].insert(PJWHash(ptr, elem.size()));
    array_of_hash[3].insert(ELFHash(ptr, elem.size()));
    array_of_hash[4].insert(BKDRHash(ptr, elem.size()));
    array_of_hash[5].insert(SDBMHash(ptr, elem.size()));
    array_of_hash[6].insert(DJBHash(ptr, elem.size()));
    array_of_hash[7].insert(DEKHash(ptr, elem.size()));
    array_of_hash[8].insert(APHash(ptr, elem.size()));
  }


std::cout << N;
out << N;
for (auto elem: array_of_hash){
  // std::cout <<"N is " << N << "  Counts of KALIZII! = " << set_of_strings.size() - elem.size() << std::endl;
  out << ' ' <<set_of_strings.size() - elem.size();
  std::cout <<' ' <<set_of_strings.size() - elem.size();
  }
  std::cout<<'\n';
  out << std::endl;
}
}
