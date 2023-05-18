#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>
#include <sstream>

int main() {
// Открываем файл
    // std::vector <std::string> vector_of_lines;
    std::string line, word_1;
    std::ifstream file("D:\\Unik\\program\\hm\\4 sem\\6_hm\\2\\txt.txt");
    int number_of_string, number_of_symbol;
    // Ввод номер строки и номера символа
    std::cin >> number_of_string >> number_of_symbol;

    // Достаем строку и смотрим, на какую длину нам нужно продвинуться
    getline(file, line);
    file.seekg((number_of_string - 1) * line.length());
    // Итератор по файлу

    std::istream_iterator<std::string> iterator(file);

    line = *++iterator;

    line.erase(0, number_of_symbol - 1);
    std::cout << line;

    }
