#include <iomanip>
#include <iostream>
#include <vector>

int main()
{
    int value;

    std::vector<int> v{1,2,3,4}; // capacity = 4
    v.push_back(1); // capacity = 8
    std::cout << "Actual occupated memory = " << v.capacity();
    float v_capacity_first = v.capacity();

    std::cout << "\nWrite value of the vector\n";
    while(v.capacity() == v_capacity_first){
      std::cin >> value;
      v.push_back(value);
    }
    std::cout <<"Memory is overloaded. New memory is = " <<v.capacity() << ". Fraction = " << v.capacity() / v_capacity_first;


    unsigned int count = 0;
    std::cout << "\nHow much memory do you want to reserve? \n";


    while (count < v.capacity()){
      std::cin >> count;
      if (count < v.capacity()) std::cout<<"Reserved memory is lower than actual. Write again: \n";
    }
    v.reserve(count);
    v_capacity_first = v.capacity();
    std::cout << "Now memory is = " << v.capacity() << "\nWrite value of the vector\n";
    while(v.capacity() == v_capacity_first){
      std::cin >> value;
      v.push_back(value);
    }
      std::cout <<"Memory is overloaded. New memory is = " <<v.capacity() << ". Fraction = " << v.capacity() / v_capacity_first;



      // unsigned long long max_reserve = 4294967295;
      // v.reserve(max_reserve+123123123123);
      // std::cout <<'\n'<< v.capacity();
}
