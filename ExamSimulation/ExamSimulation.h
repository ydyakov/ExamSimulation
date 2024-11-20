#include <iostream>
#include <vector>
#include <string>
#include <tuple>

// Структура за информацията за залата
struct RoomInfo {
    int N;                     // Брой редове в залата
    int M;                     // Брой колони в залата
    int L;                     // Минути на пристигане на лектора
    int C;                     // Време за проверка на една работа
    int brokenSeatsCount;      // Брой счупени седалки
    std::vector<int> brokenSeats; // Индекси на счупените седалки
};

// Структура за информацията за студент
struct StudentInfo {
    int time;                  // Минути след началото на изпита
    int facultyNumber;         // Последните три цифри на факултетния номер
    int examDuration;          // Време за изпита
    int course;                // Курс на студента (2, 3 или 4)
};

// Структура за събития
struct Event {
    std::string type;          // Тип на събитието: "enter" или "exit"
    int time;                  // Време на събитието
    int facultyNumber;         // Факултетен номер (само за "enter")
    int duration;              // Продължителност на изпита (само за "enter")
    int course;                // Курс на студента (само за "enter")
};

// Структура за цялостния вход
struct InputData {
    RoomInfo roomInfo;                 // Информация за залата
    std::vector<StudentInfo> students; // Информация за студентите
    std::vector<Event> events;         // Списък със събития
};