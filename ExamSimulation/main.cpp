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

// ---------------------------------------------------------

#include <fstream>
#include <sstream>
#include <iostream>

InputData readInputData(const std::string& filename) {
    InputData inputData;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Unable to open file";
    }

    std::string line;
    // Четене на първия ред (размери на залата, време на лектора, време за проверка)
    std::getline(file, line);
    std::istringstream ss(line);
    ss >> inputData.roomInfo.N >> inputData.roomInfo.M >> inputData.roomInfo.L >> inputData.roomInfo.C;

    // Четене на броя счупени седалки
    std::getline(file, line);
    inputData.roomInfo.brokenSeatsCount = std::stoi(line);

    // Четене на индексите на счупените седалки
    if (inputData.roomInfo.brokenSeatsCount > 0) {
        std::getline(file, line);
        std::istringstream seatStream(line);
        int seatIndex;
        while (seatStream >> seatIndex) {
            inputData.roomInfo.brokenSeats.push_back(seatIndex);
        }
    }

    // Четене на събитията
    while (std::getline(file, line)) {
        if (line == "end") break;

        std::istringstream eventStream(line);
        std::string eventType;
        eventStream >> eventType;

        if (eventType == "enter") {
            Event event;
            event.type = "enter";
            eventStream >> event.time >> event.facultyNumber >> event.duration >> event.course;
            inputData.events.push_back(event);
        }
        else if (eventType == "exit") {
            Event event;
            event.type = "exit";
            eventStream >> event.time >> event.facultyNumber;
            inputData.events.push_back(event);
        }
    }

    file.close();
    return inputData;
}



int main() {
    try {
        InputData inputData = readInputData("input.txt");

        // Примерен достъп до данни
        std::cout << "Room dimensions: " << inputData.roomInfo.N << "x" << inputData.roomInfo.M << std::endl;
        std::cout << "Lecturer arrival: " << inputData.roomInfo.L << " minutes" << std::endl;
        std::cout << "Broken seats count: " << inputData.roomInfo.brokenSeatsCount << std::endl;

        for (const auto& seat : inputData.roomInfo.brokenSeats) {
            std::cout << "Broken seat index: " << seat << std::endl;
        }

        for (const auto& event : inputData.events) {
            std::cout << "Event: " << event.type << " at time " << event.time;
            if (event.type == "enter") {
                std::cout << ", Faculty Number: " << event.facultyNumber
                    << ", Duration: " << event.duration
                    << ", Course: " << event.course;
            }
            std::cout << std::endl;
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}


