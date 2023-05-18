#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>


using namespace std::chrono_literals;
using Allocator_string = boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>;
using String = boost::interprocess::basic_string<char, std::char_traits<char>, Allocator_string>;

struct Message
{
    String text;
    int id;
    Message(String& text, int id) : text(text), id(id){};
    Message(const Message &message) : text(message.text), id(message.id){};
    Message(String& text) : text(text), id(0){};
    Message() = default;
};

using message_allocator = boost::interprocess::allocator<Message, boost::interprocess::managed_shared_memory::segment_manager>;
using Vector = boost::interprocess::vector<Message, message_allocator>;


const std::string shared_memory_name_1("2_1");
const std::string shared_memory_name_2("2_2");
const std::string cv_mutex_name("2_2");
const std::string message_mutex_name("2_3");
const std::string condition_name("2_4");


boost::interprocess::managed_shared_memory shared_memory_1(boost::interprocess::open_or_create, shared_memory_name_1.c_str(), 102400);
boost::interprocess::managed_shared_memory shared_memory_2(boost::interprocess::open_or_create, shared_memory_name_2.c_str(), 1024);
boost::interprocess::named_mutex cv_mutex(boost::interprocess::open_or_create, cv_mutex_name.c_str());
boost::interprocess::named_mutex message_mutex(boost::interprocess::open_or_create, message_mutex_name.c_str());
boost::interprocess::named_condition condition(boost::interprocess::open_or_create, condition_name.c_str());

Allocator_string allocator_string(shared_memory_1.get_segment_manager());




void writer(std::vector<int> &v);
void reader(std::vector<int> &v);
std::pair<std::string, int> get_message();

int main() {
    // boost::interprocess::shared_memory_object::remove(shared_memory_name_1.c_str());
    // boost::interprocess::shared_memory_object::remove(shared_memory_name_2.c_str());
    // boost::interprocess::named_mutex::remove(cv_mutex_name.c_str());
    // boost::interprocess::named_mutex::remove(message_mutex_name.c_str());
    // boost::interprocess::named_condition::remove(condition_name.c_str());


    boost::interprocess::managed_shared_memory shared_memory_1(boost::interprocess::open_or_create, shared_memory_name_1.c_str(), 1024);
    boost::interprocess::managed_shared_memory shared_memory_2(boost::interprocess::open_or_create, shared_memory_name_2.c_str(), 1024);
    boost::interprocess::named_mutex cv_mutex(boost::interprocess::open_or_create, cv_mutex_name.c_str());
    boost::interprocess::named_mutex message_mutex(boost::interprocess::open_or_create, message_mutex_name.c_str());
    boost::interprocess::named_condition condition(boost::interprocess::open_or_create, condition_name.c_str());

    shared_memory_1.find_or_construct<Vector>("Data")(shared_memory_1.get_segment_manager());
    shared_memory_2.find_or_construct<int>("Position")(0u);
    std::vector<int> vector_of_strings;
    std::vector<std::thread> v(2);


    {
      std::unique_lock lock(cv_mutex);
      auto [ptr_data, size] = shared_memory_1.find<Vector>("Data");
      if ((*ptr_data).empty())
      {
        String message_string(allocator_string);
        std::string message_str = "Start message";
        message_string = message_str;
        Message message(message_string, 0);
        ptr_data->push_back(message);
      }
    }
    v.at(0) = std::thread(writer, std::ref(vector_of_strings));
    v.at(1) = std::thread(reader, std::ref(vector_of_strings));
    v.at(0).join();
    v.at(1).join();

    boost::interprocess::shared_memory_object::remove(shared_memory_name_1.c_str());
    boost::interprocess::shared_memory_object::remove(shared_memory_name_2.c_str());
    boost::interprocess::named_mutex::remove(cv_mutex_name.c_str());
    boost::interprocess::named_mutex::remove(message_mutex_name.c_str());
    boost::interprocess::named_condition::remove(condition_name.c_str());

}

void reader(std::vector<int> &v) {
    std::string str;
    while (str != "s") {
        std::unique_lock lock(cv_mutex);
        condition.wait(lock);
        auto [message, position] = get_message();
        if ((std::binary_search(v.begin(), v.end(), position))&&(message != "history")){
          str = message;
          continue;
        }
        else if ((message == "history") && (std::binary_search(v.begin(), v.end(), position))) {
          auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
          for(auto elem: *ptr_data)
            std::cout <<elem.id << " - " <<elem.text << '\n';
          str = message;
        }
        else{
          // for(auto elem: v)
          //   std::cout << elem;
          std::cout <<"New message: "<<message << '\n';
          str = message;
      }
  }

}


std::pair<std::string, int> get_message() {
    std::lock_guard lock(message_mutex);
    // Получаем указатели на вектор и позицтю, которую мы меняли
    auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
    auto [ptr_pos, size_pos] = shared_memory_2.find<int>("Position");
    Message message((*ptr_data).back());
    return std::pair<std::string, int>(message.text, message.id);
}

void writer(std::vector<int> &v) {
    std::string message_str;
    while (message_str != "s") {
      std::getline(std::cin, message_str);
      String message_string(allocator_string);
      std::lock_guard lock(message_mutex);
      auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
      auto [ptr_pos, size_pos] = shared_memory_2.find<int>("Position");
      message_string = message_str;
      Message received_message = (*ptr_data).back();
      Message message(message_string, received_message.id+1);
      ptr_data->push_back(message);
      v.push_back((*ptr_data).back().id);
      condition.notify_all();
  }
  std::this_thread::sleep_for(10ms);
  std::cout << "Thaks for writing. See u later";
}
