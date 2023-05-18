#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <iterator>
#include <forward_list>
#include <numeric>

#include <array>

int main(){


  // 1 task
  int N = 10;
  std::vector<int> v_1(N);
  std::iota(v_1.begin(), v_1.end(), 1);

// 2 task

  std::copy(std::istream_iterator<int>(std::cin), std::istream_iterator<int>(), std::back_inserter(v_1));


// 3 task

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(v_1.begin(), v_1.end(), g);

  // 4 task

  std::sort(v_1.begin(), v_1.end());
  auto last = std::unique(v_1.begin(), v_1.end());
  v_1.erase(last, v_1.end());
  for(auto elem: v_1) std::cout << elem<< ' ';

  // 5 task

  int count_array = std::count_if(v_1.begin(), v_1.end(), [](int i){return i%2 !=0;});
  std::cout << count_array;
  //

  // // 6 task
  auto [min, max] = std::minmax_element(v_1.begin(), v_1.end());
  std::cout << "\nMax elem = " << *min << "; Min elem is " << *max <<'\n';
  //
  // // 7 task
  // // std::cout << std::for_each(v_1.begin(), v_1.end(v_1), [v_1.begin(), v_1.size()](int i){})
  //
  // // 8 task

  // std::vector<int>:: iterator it;
  // std::replace_if(v_1.begin(), v_1.end(), predic, [](int x));
  // std::cout << 's';
  // for(auto elem: v_1) std::cout << elem<< ' ';
  //

  // // 9 task
  std::vector<double> v_2(v_1.size());
  std::mt19937 engine(rd());
  std::uniform_real_distribution<> uniform_distr(1,10);
  std::normal_distribution<> d(-10,10);
  std::generate(v_2.begin(), v_2.end(), d(engine)); // ninth

  //
  // // 10 task
  std::cout << std::accumulate(v_2.begin(), v_2.end(), 0);
  //
  // // 11 task
  //
  std::fill_n(v_2.begin(), 3, 1);
  // for(auto elem: v_2) std::cout << elem << ' ';

  // 12 task
  std::vector<int> v_3(v_1.size());
  std::transform(v_1.begin(), v_1.end(), v_2.begin(), v_3.begin(), [](int value_1, int value_2) {return value_1 - value_2; });


  // 13 task

  std::replace_if(v_3.begin(), v_3.end(), [](auto value){return value < 0; }, 0);


  // 14 task

  v_3.erase(std::remove_if(v_3.begin(), v_3.end(), [](auto value){return value == 0; }), v_3.end());


  // 15 task

  std::reverse(v_3.begin(), v_3.end());

  // 16 task

  const int count_of_top = 3;
  std::partial_sort(v_3.begin(), v_3.begin() + count_of_top, v_3.end(), std::greater{});
  std::vector<int> max_values;
  std::copy_n(v_3.begin(), count_of_top,  std::back_inserter(max_values));


// 17 task

  std::sort(v_1.begin(), v_1.end(), std::less{});
  std::sort(v_2.begin(), v_2.end(), std::less{});


// 18 task
  std::vector<int> v_4;
  std::merge(v_1.begin(), v_1.end(), v_2.begin(), v_2.end(), std::back_inserter(v_4));

// 19 task
  auto range_of_one = std::equal_range(v_4.begin(), v_4.end(), 1);


//
}
