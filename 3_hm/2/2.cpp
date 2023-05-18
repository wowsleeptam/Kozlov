#include<iostream>
#include<cstdlib>
#include<random>
#include<ctime>
#include<vector>
#include <set>
#include <cassert>
#include <boost/functional/hash.hpp>
#include<unordered_set>
#include <functional>
#include <fstream>


// Структура Student, которая включаяет в себе поля: m_age, m_mark и m_name. Конструктор структуры имеется!
struct Student{
  int m_age;
  double m_mark;
  std::string m_name;
  Student(int age, double mark, std::string name): m_age(age), m_mark(mark), m_name(name){}


// Оператор сравнения для величин нашей структуры (понадобится для unordered_set)
  bool operator==(const Student& student) const
  {
    if (m_age == student.m_age && m_mark == student.m_mark && m_name == student.m_name ) return true;
    else return false;
  }
};

// Хэш-фукнция для величин нашей структуры (понадобится для unordered_set)
struct HashFunction{
  std::size_t seed = 0;
  size_t operator()(const Student& student) const{
        std::size_t seed = 0;
        boost::hash_combine(seed, student.m_age);
        boost::hash_combine(seed, student.m_mark);
        boost::hash_combine(seed, student.m_name);
    return seed;
    ;}
  };


int main(){
  std::ofstream out;          // поток для записи
  out.open("D:\\2.txt"); // окрываем файл для записи
  // Объявления величин. Сколько раз будет повторяться цикл, объявления рандома и списка имен студентов
  std::random_device rd;
  std::mt19937 engine(rd());
  unsigned int N_count;
  std::cin >> N_count;
  std::vector<std::string> names{"Alexey" , "Uriy", "Kate", "Liza", "Umar", "Ruslan", "Misha", "Dima", "Anna", "Artem"};

// Распределения величин (имя, средний бал и величина)
  std::uniform_int_distribution<> int_values(20,30);
  std::uniform_int_distribution<> int_names(1,names.size()-1);
  std::uniform_real_distribution<> real_values(30,32);
// Начало N-ой итерации
  for (size_t N = 0; N < N_count; N+=50) {


// Задание unordered_set, в которая состоит из структуры Student и в которой мы определил хэш-фукнцию
  std::unordered_set<Student, HashFunction> array_of_students;
// Множество хэша, которые мы подсчитаем для каждного члена позже
  std::set<long> set_of_hash;

// Заполняем массив студентов уникальными значениями. Уникальность будет гарантировать хэш функция, которую определили выше
  while (array_of_students.size() < N){
    array_of_students.insert(Student(int_values(engine), real_values(engine), names.at(int_names(engine))));
    }

// Вычисление хэша объектов и добавление в множество хэшов
  for(auto elem: array_of_students){
    std::size_t seed = 0;
    boost::hash_combine(seed, elem.m_age);
    boost::hash_combine(seed, elem.m_mark);
    boost::hash_combine(seed, elem.m_name);
    set_of_hash.insert(seed);
  }

  // Подсчитываем коллизия путем сравнивания количества студентов и количества их уникального хеша
  // std::cout <<"N is " << N << "  Counts of KALIZII! = " << array_of_students.size() - set_of_hash.size() << std::endl;
  std::cout << N << ' ' << array_of_students.size() - set_of_hash.size() << std::endl;
  out << N << ' ' << array_of_students.size() - set_of_hash.size() << std::endl;

  }

}
