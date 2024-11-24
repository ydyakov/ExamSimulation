#include "ExamSimulation.h"
#include "ProcessExam.h"


InputData readInputData(const std::string& filename) {
    InputData inputData;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Unable to open file";
    }

    std::string line;
    // Четене на първия ред (размери на залата, време на лектора, време за проверка)
    std::getline(file, line);
    std::istringstream streemLine(line);
    streemLine >> inputData.roomInfo.cols >> inputData.roomInfo.rows >>  inputData.lectorInfo.arrivalMunute >> inputData.lectorInfo.timeForCheck;

    // Четене на броя счупени седалки
    std::getline(file, line);
    streemLine = std::istringstream(line);
    streemLine >> inputData.roomInfo.brokenSeatsCount;

    // Четене на индексите на счупените седалки
    if (inputData.roomInfo.brokenSeatsCount > 0) {
        std::getline(file, line);
        streemLine = std::istringstream(line);
        int seatIndex;
        while (streemLine >> seatIndex) {
            inputData.roomInfo.brokenSeats.push_back(seatIndex);
        }
    }

    // Четене на събитията
    while (std::getline(file, line)) {
        if (line == "end") break;

        streemLine = std::istringstream(line);
        std::string eventType;
        streemLine >> eventType;

        if (eventType == "enter") {
            StudentInfo studentInfo;
            streemLine >> studentInfo.time >> studentInfo.facultyNumber >> studentInfo.duration >> studentInfo.course;
            inputData.studentsArraival.push_back(studentInfo);
        }
    }

    file.close();
    return inputData;
}

RoomConfiguration CenerateOptimalRoomConfiguratin(InputData& inputData)
{
    RoomConfiguration optimal(inputData.roomInfo.cols, inputData.roomInfo.rows);
    
    for (int i = 0; i <= 1; i++)
    {
        for (int j = 0; i <= 1; i++)
        {
            RoomConfiguration roomConfiguratn(inputData.roomInfo.cols, inputData.roomInfo.rows);
            for (int index : inputData.roomInfo.brokenSeats)
            {
                roomConfiguratn.markSeatsAsBroken(index);
            }
            roomConfiguratn.fillOccupiedSeats(i, j);
            if (optimal.capacity < roomConfiguratn.capacity)
            {
                optimal = roomConfiguratn;
            }
        }
    }

    return optimal;
}


int main() {
    try {
        InputData inputData = readInputData("input.txt");
        RoomConfiguration optimalRoomConfiguration = CenerateOptimalRoomConfiguratin(inputData);
        optimalRoomConfiguration.PrintRoomConfiguratoin(std::cout);


        // Примерен достъп до данни
        std::cout << "Room dimensions: " << inputData.roomInfo.rows << "x" << inputData.roomInfo.cols << std::endl;
        std::cout << "Lecturer arrival: " << inputData.lectorInfo.arrivalMunute << " minutes" << std::endl;
        std::cout << "Lecturer time to check: " << inputData.lectorInfo.timeForCheck << " minutes" << std::endl;
        std::cout << "Broken seats count: " << inputData.roomInfo.brokenSeatsCount << std::endl;

        for (const auto& seat : inputData.roomInfo.brokenSeats) {
            std::cout << "Broken seat index: " << seat << std::endl;
        }

        for (const auto& event : inputData.studentsArraival) {
            std::cout << "Event: at time " << event.time;
            std::cout << ", Faculty Number: " << event.facultyNumber
                      << ", Duration: " << event.duration
                      << ", Course: " << event.course;
            
            std::cout << std::endl;
        }

        ExamResult examResult = ProcessExam(inputData, optimalRoomConfiguration.capacity);
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    

    return 0;
}


