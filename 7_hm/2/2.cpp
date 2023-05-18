#include <algorithm>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include "timer.hpp"
#include <fstream>



template < typename Iterator, typename T >
struct accumulate_block
{
	void operator()(Iterator first, Iterator last, T & result)
	{
		result = std::accumulate(first, last, result); // !
	}
};

template < typename Iterator, typename T >
T parallel_accumulate(Iterator first, Iterator last, T init, std::size_t num_threads)
{
	const std::size_t length = std::distance(first, last);

	if (!length)
		return init;

	const std::size_t block_size = length / num_threads;

	std::vector < T > results(num_threads);
	std::vector < std::thread > threads(num_threads - 1);

	Iterator block_start = first;

	for (std::size_t i = 0; i < (num_threads - 1); ++i)
	{
		Iterator block_end = block_start;
		std::advance(block_end, block_size);

		threads[i] = std::thread(
			accumulate_block < Iterator, T > (),
			block_start, block_end, std::ref(results[i])); // !

		block_start = block_end;
	}

	accumulate_block < Iterator, T > ()(block_start, last, results[num_threads - 1]);

	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));

	return std::accumulate(results.begin(), results.end(), init);
}

int main(int argc, char ** argv)
{
	std::ofstream out;          // поток для записи
	out.open("D:\\Unik\\program\\hm\\4 sem\\7_hm\\2\\txt.txt");
	std::vector < long long > v(100000000);
  Timer timer;
  std::iota(v.begin(), v.end(), 1);
  timer.start();
for (size_t i = 1; i <= 7000  ; i += 10) {
  timer.restart();
  parallel_accumulate(v.begin(), v.end(), 0, i);
  out << i << ' ' << timer.elapsed() << std::endl;

}
}




Вооо, теперь все чики-пуки. Примерно при 3000 потоках у нас время выполнения операции начинает возрастать, а не убывать.
Это может зависеть от того, что у процесса ограниченный ресурс и для выделения потоков ему необходимо больше времени, нежели он подсчитает
это в одном потоке. Также, хотя я не уверен, тут может косвенно играть роль состояние гонки, что потоки ждут освобождение данных, занятых другим потоком
