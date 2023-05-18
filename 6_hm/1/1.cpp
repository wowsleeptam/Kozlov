#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <vector>

bool check_for_special(std::string symbol, const size_t& pos){
  return ((symbol.at(pos)!= '$')&&(symbol.at(pos)!='@'));
}

int main() {
// Открываем файл
    // std::vector <std::string> vector_of_lines;
    std::string line;
    std::ifstream file("D:\\Unik\\program\\hm\\4 sem\\6_hm\\1\\txt.txt");
    bool flag_1, flag_2;
    int flag_int = 0;
    std::vector<int> v;
    size_t pos, end_pos;



    if (file.is_open()) {
        while (std::getline(file, line)) {

            flag_2 = false;
            pos = line.find("//");
            for (size_t i = 0; i < count; i++) {
              /* code */
            }





            if (pos != std::string::npos){
              while (line.find("//") != std::string::npos){
                std::cout << line << '\n';
                for (size_t i = 0; i < v.size(); i+=2) {
                  // std::cout << v[i] << ' ' << v[i + 1] << ' ' << pos;
                  if ((v[i] < pos)&&(v[i+1] > pos)){
                    flag_2 = true;
                }
              }
                if (flag_2 != true){
                std::string::iterator itr = (line.begin() + pos);
                line.erase(itr, line.end());
              }

            }
        }

            else {
// Иначе находим /* и ищем */. удалаяем промежуток между ними
                pos = line.find("/*");
                end_pos = line.find("*/");
                if (pos != std::string::npos)
                  flag_1 = true;
                if ((end_pos != std::string::npos)&&(pos != std::string::npos)){
                  line.erase(pos, end_pos-pos+2);

                }
                if ((pos == std::string::npos)&&(flag_1 == true)){
                  flag_1 = false;
                  line.erase();
                }
                if ((pos != std::string::npos)&&(end_pos == std::string::npos))
                  line.erase(pos);

                }
                std::cout << line << std::endl;
                v.clear();
        }
// Добавить сюда алгос, который бы делал erase от // и до конца
        file.close();}
        else {
        std::cout << "Fail to open the file" << std::endl;
    }

    return 0;
}
