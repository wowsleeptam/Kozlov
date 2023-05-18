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

 // Определяю аллокатор для строк и сами строки для межпроц. взаим.
using namespace std::chrono_literals;
using Allocator_string = boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>;
using String = boost::interprocess::basic_string<char, std::char_traits<char>, Allocator_string>;

// Создаю структуру, хотя храни в себе само сообщение и его id. ID - это глобальный номер сообещния, то есть
// его порядковый номер в общий памяти. Ну и также несколько конструкторов на всякий случай
struct Message
{
    String text;
    int id;
    Message(String& text, int id) : text(text), id(id){};
    Message(const Message &message) : text(message.text), id(message.id){};
    Message(String& text) : text(text), id(0){};
    Message() = default;
    ~Message() {};
};


// Создаю вектор для хранения истории сообщений и создаю аллокатор дня него
using message_allocator = boost::interprocess::allocator<Message, boost::interprocess::managed_shared_memory::segment_manager>;
using Vector = boost::interprocess::vector<Message, message_allocator>;

// Тупо названия, тупо кайф
const std::string shared_memory_name_1("2_1");
const std::string shared_memory_name_2("2_2");
const std::string cv_mutex_name("2_2");
const std::string message_mutex_name("2_3");
const std::string condition_name("2_4");

// Создаю всякие другие штуки, тут не отличается ничем от демонстрационного кода
boost::interprocess::managed_shared_memory shared_memory_1(boost::interprocess::open_or_create, shared_memory_name_1.c_str(), 102400);
boost::interprocess::named_mutex cv_mutex(boost::interprocess::open_or_create, cv_mutex_name.c_str());
boost::interprocess::named_mutex message_mutex(boost::interprocess::open_or_create, message_mutex_name.c_str());
boost::interprocess::named_condition condition(boost::interprocess::open_or_create, condition_name.c_str());


// А это вот загадка вселенной. Порыскав в инете, я обнаружил, что это существо нужно использовать, когда определяешь переменную типа String( ) 91 строка
// Как я понял, происходят некоторые проблемы с выделением памяти, поэтому на помощь прибегает не черный плащ, а аллокатор

Allocator_string allocator_string(shared_memory_1.get_segment_manager());



// Объявляю ридер, и райтер, и функцию get_message, которая возвращает строку сообщения и его номер, id . Потом напишу, что такое v
void writer(std::vector<int> &v);
void reader(std::vector<int> &v);
std::pair<std::string, int> get_message();

int main() {
  //Если нужно очистить память - раскомментируй это, как Ленин завещал!

    // boost::interprocess::shared_memory_object::remove(shared_memory_name_1.c_str());
    // boost::interprocess::named_mutex::remove(cv_mutex_name.c_str());
    // boost::interprocess::named_mutex::remove(message_mutex_name.c_str());
    // boost::interprocess::named_condition::remove(condition_name.c_str());


// Создаю разделяему память для вектора из сообщений
    shared_memory_1.find_or_construct<Vector>("Data")(shared_memory_1.get_segment_manager());
// Это как раз v, о котором я писал выше. Этот вектор хранит в себе локальные номера сообещний, то есть те номера, которые были написаны
// Именно в этой программе. Например, есть 2 проги - в 1-ой написал 1, 2, 3 сообещние, а во 2-ой проге написали 4. Тогда у второй проги будет хранится
// id 4-го сообщения
    std::vector<int> vector_of_strings;

    // Лучше уж pair создать, то тоже сойдет)
    std::vector<std::thread> v(2);


    {
// Нахожу наш вектор в разделяемой памяти
      std::unique_lock lock(cv_mutex);
      auto [ptr_data, size] = shared_memory_1.find<Vector>("Data");
      if (ptr_data)
      {
        // Если этот вектор пустой, то я ввожу первое сообщения и посылаю его в наш вектор истории
        if ((*ptr_data).empty())
        {
          // Вот тут как раз и объявился аллоктор
          String message_string(allocator_string);
          std::string message_str = "Start message";
          // Не знаю, как это грамотней сделать, но здесь просто скопировал std::string в String
          message_string = message_str;
          // У первого сообщения порядковый номер 0
          Message message(message_string, 0);
          ptr_data->push_back(message);
        }
      }
    }

// Запускаю наши потоки родные
    v.at(0) = std::thread(writer, std::ref(vector_of_strings));
    v.at(1) = std::thread(reader, std::ref(vector_of_strings));
    v.at(0).join();
    v.at(1).join();
// Ну и очищаю память
    boost::interprocess::shared_memory_object::remove(shared_memory_name_1.c_str());
    boost::interprocess::shared_memory_object::remove(shared_memory_name_2.c_str());
    boost::interprocess::named_mutex::remove(cv_mutex_name.c_str());
    boost::interprocess::named_mutex::remove(message_mutex_name.c_str());
    boost::interprocess::named_condition::remove(condition_name.c_str());
    std::cout << "Thaks for writing. See u later";
}



// Ридер
void reader(std::vector<int> &v) {
    std::string str;
    while (str != "s") {
      // Условная "блокировка", которая ждет команды от райтера
        std::unique_lock lock(cv_mutex);
        condition.wait(lock);
        // Вызываю фукнцию, которая возвращает сообещние и его id
        auto [message, position] = get_message();


        // Если такой позиции нет, то завершаем цикл. В get_message я обработал так, чтобы возвращалось id = -1, если у нас вектор пуст
        if (position == -1){
          std::cout << "Fail to read\n";
          break;
        }
        // Здесь я рассматриваю 3 случая:
        // Чтобы пользователь не получал свое же сообещние, он проверяет id сообещния и свой локальный списко vector_of_strings. Если есть совпадение,
        // То он не выводит никакое сообещние
        else if ((std::binary_search(v.begin(), v.end(), position))&&(message != "history")){
          str = message;
          continue;
        }
        // Если пользователь вводит history, то только ему, а не другим программам, выводит историю, то есть мы дополнительно с history проверяем
        // Есть ли в нашем локальном списке  vector_of_strings id данной строки.
        else if ((message == "history") && (std::binary_search(v.begin(), v.end(), position))) {
          auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
          for(auto elem: *ptr_data)
            std::cout <<elem.id << " - " <<elem.text << '\n';
          str = message;
        }
        // Ну а это просто выводит сообщение, ничего такого
        else{
          // for(auto elem: v)
          //   std::cout << elem;
          std::cout <<"New message: "<<message << '\n';
          str = message;
      }
  }
  // Ну и также мы везде добавляем str = message, чтобы у нас цикл продолжал работать до тех пор, пока пользователь не получится s

}

// get_message
std::pair<std::string, int> get_message() {
    std::lock_guard lock(message_mutex);
    // Получаем указатели на вектор и позицтю, которую мы меняли
    auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
    if (ptr_data)
    {
      // Если все ок, то просто позвращается пара из текста и его id
      Message message((*ptr_data).back());
      return std::pair<std::string, int>(message.text, message.id);
    }
    // Если не ок, то возвращается это чудо, то есть id ошибки = -1
    else return std::pair<std::string, int>("Fail to read", -1);
}


// Райтер
void writer(std::vector<int> &v) {
    std::string message_str;
    while (message_str != "s") {
      std::cout << "Write smth: ";
      // Сначала ждем строку от пользователя
      std::getline(std::cin, message_str);
      // Затем определяем строку типа String, чтобы впихнуть в конструктор Message
      String message_string(allocator_string);
      std::lock_guard lock(message_mutex);
      auto [ptr_data, size_data] = shared_memory_1.find<Vector>("Data");
      if(ptr_data){
        // Если все ок, то
        // Здесь меняем тип с std::string, в String
        message_string = message_str;
        // Возвращаем id последенго элемента в векторе.
        int received_message_id = (*ptr_data).back().id;
        // Ну и создаем нашу структуру, в конструктор пихаем сообещние типа String и id на 1 больше, чем предыдущий
        Message message(message_string, received_message_id + 1);
        // Пихаем в разделяемый вектор
        ptr_data->push_back(message);
        // Пихаем id сообщения в локальнйы список, чтобы сообещние не выводилось этому эу пользователю, который его и ввел
        v.push_back((*ptr_data).back().id);
        condition.notify_all();
      }
      else {
        std::cout << "Fail to write\n";
        condition.notify_all();
        break;
      }
  }
}
