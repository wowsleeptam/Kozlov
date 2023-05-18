#include <iostream>
#include <thread>
#include <future>
#include <random>
#include <cmath>
#include <mutex>
#include <utility>
#include <string>
#include <set>

void func(std::string sub, std::string user_str, int begin, int size, std::set<int>& positions ){
  // находим первую позицию вхождения нашей строки в сабстроке
  int pos = sub.find(user_str);
  // блокируем
  std::mutex g_i_mutex;
  std::lock_guard<std::mutex> lock(g_i_mutex);

  // Пока у нас до сих пор есть вхождения в строку
  while (pos != std::string::npos){
    // мы добавляем конец строки относительного глобального начала строки
    positions.insert(begin + pos + user_str.size());
    // Если есть еще какие-то вхождения, то мы их тоже обрабатываем
    pos = sub.find(user_str, pos + user_str.size());
  }
}




int main(){
  const int number_of_threads = 3;
  const long count_of_letters = 20;
  // Строка из рандомных символов
  std::string string;
  // Пользовательская строка
  std::string user_str;
  // Строка, я потом ее определю
  std::string sub_str;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 3);



// Заполняю строку рандомными символами
  int letter = dis(gen);
  for (size_t i = 0; i < count_of_letters; i++) {
    letter = dis(gen);
  switch (letter) {
    case 0:
      string += 'A';
      break;
    case 1:
      string += 'C';
      break;
    case 2:
      string += 'G';
      break;
    case 3:
      string += 'T';
      break;
    }
  }


  std::cout << string << '\n';
  // Вволим вектор из потоков
  std::vector<std::thread> freasult(number_of_threads);

  std::getline(std::cin, user_str);
  // Одна итерация - количество символов в одной части. Всего частей number_of_threads+2.
  int one_iteration = count_of_letters/(number_of_threads+2);
  // Множество, в которое мы будем складывать позиции
  std::set<int> positions;
  for (size_t i = 1; i < number_of_threads + 1; i++) {
// Сабстрока - строка, в которую мы передаем одну часть + user_str.size() в начале и + user_str.size() в конце
    sub_str = string.substr(i*one_iteration - user_str.size(), one_iteration + 2*user_str.size());
    // В поток передаем начальную позицю нашей части, начальную позицию этой части, размер нашей сабстроки и множество, в которое мы будем складывать
    // Наши позиции
    freasult.at(i-1) = std::thread(func, sub_str, user_str, i*one_iteration - user_str.size(), one_iteration + 2*user_str.size(), std::ref(positions));
  }


  for (size_t i = 0; i < number_of_threads; i++) {
    freasult.at(i).join();
  }


  // Проворачиваем все те же мохинации с первой частью и последней
  sub_str = string.substr(0, one_iteration + user_str.size());
  int pos = sub_str.find(user_str);
  // std::cout << sub_str << '\n';
  while (pos != std::string::npos){
    positions.insert(pos + user_str.size());
    pos = sub_str.find(user_str, pos + user_str.size());
  }

  sub_str = string.substr(string.size() - one_iteration, one_iteration);
  pos = sub_str.find(user_str);
  while (pos != std::string::npos){

    positions.insert(string.size() - one_iteration + pos + user_str.size());
    pos = sub_str.find(user_str, pos + user_str.size());
  }
// Выводим все позиции
  for (auto elem: positions){
    std::cout << elem << ' ';
  }

}
