#include <iostream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <thread>
#include <chrono>




using namespace std::chrono_literals;

void view_directory(const std::filesystem::path & path, std::ostringstream & sout)
{
	if (std::filesystem::exists(path) && std::filesystem::is_directory(path)){
		for (const auto & entry : std::filesystem::directory_iterator(path)){






      std::filesystem::file_time_type ftime = std::filesystem::last_write_time(entry);
			auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(ftime);
			std::chrono::system_clock::to_time_t(sctp);
		  std::cout <<sctp << ' '<< entry.path().filename().string() <<"  "<< entry.path().extension().string()  <<"  "<< '\n';
		}
	}
}



int main(){
  std::ostringstream sout;
  auto path = std::filesystem::current_path();
	// std::cout << compute_file_size("./Source.cpp") << std::endl;
	view_directory(path, sout);

  std::cout << sout.str();

}
