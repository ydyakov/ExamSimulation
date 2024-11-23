#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>

// Структура за информацията за залата
struct RoomInfo {
	int rows;                     // Брой редове в залата
	int cols;                     // Брой колони в залата
	int brokenSeatsCount;      // Брой счупени седалки
	std::vector<int> brokenSeats; // Индекси на счупените седалки
};

struct LectorInfo {
	int arrivalMunute;                     // Минути на пристигане на лектора
	int timeForCheck;                     // Време за проверка на една работа
};


// Структура за пристигането на студентите
struct StudentInfo {
	int time;                  
	int facultyNumber;         
	int duration;              
	int course;                
};

// Структура за цялостния вход
struct InputData {
	RoomInfo roomInfo;
	LectorInfo lectorInfo;
	std::vector<StudentInfo> studentsArraival;
};

enum SeatState {
	Broken,
	Free,
	Occupied
};

struct RoomConfiguration {
	int rows;
	int cols;
	int capacity = 0;
	std::vector<std::vector<SeatState>> layout;

	RoomConfiguration(int rows, int cols) : rows(rows), cols(cols), layout(rows, std::vector<SeatState>(cols, SeatState::Free)) {}

	void markSeatsAsBroken(int index)
	{
		if (index > cols * rows)
		{
			return;
		}
		int r = index / cols;
		int c = index % cols;
		layout[r][c] = SeatState::Broken;
	}

	void fillOccupiedSeats(int startRow, int startCols)
	{
		capacity = 0;

		for (int i = startRow; i < rows; i += 2)
		{
			for (int j = startCols; j < cols; j += 2)
			{
				if (layout[i][j] != SeatState::Broken)
				{
					layout[i][j] = SeatState::Occupied;
				}
			}
		}

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if ((layout[i][j] == SeatState::Free) &&
					(i > 0 && j > 0 && (layout[i - 1][j - 1] != SeatState::Occupied)) &&
					(i == 0 || (layout[i - 1][j] != SeatState::Occupied)) &&
					(j == 0 || (layout[i][j - 1] != SeatState::Occupied)) &&
					(i < rows - 1 && j < cols - 1 && (layout[i + 1][j + 1] != SeatState::Occupied)) &&
					(i == rows - 1 || (layout[i + 1][j] != SeatState::Occupied)) &&
					(j == cols - 1 || (layout[i][j + 1] != SeatState::Occupied)))

				{
					layout[i][j] = SeatState::Occupied;
				}
			}
		}

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{
				if (layout[i][j] == SeatState::Occupied)
				{
					capacity++;
				}
			}
		}

	}

	void PrintRoomConfiguratoin(std::ostream& ostreem)
	{
		ostreem << std::endl;
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < cols; j++)
			{

				if (layout[i][j] == SeatState::Occupied)
				{
					ostreem << "O";
				}
				else if (layout[i][j] == SeatState::Free)
				{
					ostreem << "F";
				}
				else
				{
					ostreem << "B";
				}
			}
			ostreem << std::endl;
		}
	}


// ----------------------------------------

	int time = 0;

	// Функция за сравнение на студентите за приоритизирана опашка
	struct CompareStudents {
		bool operator()(const StudentInfo& s1, const StudentInfo& s2) const {
			// Курс 2 има по-висок приоритет
			if (s1.course != s2.course) {
				return s1.course > s2.course;  // По-нисък курс -> по-висок приоритет
			}
			// Ако курсът е еднакъв, по-ранното време има предимство
			return s1.time > s2.time;
		}
	};

	// Дефиниция на приоритизираната опашка за студентите пред залата
	using StudentQueue = std::priority_queue<StudentInfo, std::vector<StudentInfo>, CompareStudents>;

	// Структура за запис на студент в залата
	struct StudentRecord {
		int facultyNumber; 
		int finishTime;    
	};

	// Структура за залата
	struct ExamRoom {
		int capacity;                         // Максимален брой места
		std::vector<StudentRecord> students; // Списък със записаните студенти

		// Конструктор
		ExamRoom(int maxCapacity) : capacity(maxCapacity) {}

		bool addStudent(int facultyNumber, int finishTime) {
			if (students.size() < capacity) {
				students.push_back({ facultyNumber, finishTime });
				return true; // Успешно добавяне
			}
			else {
				std::cout << "Room is full! Cannot add student " << facultyNumber << ".\n";
				return false; // Няма свободни места
			}
		}

		// Премахване на студенти с минимално време
		std::vector<StudentRecord> removeStudentWithMinTime() {
			std::vector<StudentRecord> result;

			if (students.empty()) {
				return result; 
			}

			// Намираме минималното време за приключване
			int minTime = std::min_element(students.begin(), students.end(),
				[](const StudentRecord& a, const StudentRecord& b) {
					return a.finishTime < b.finishTime;
				})->finishTime;

			// Събираме всички студенти с това минимално време
			for (const auto& student : students) {
				if (student.finishTime == minTime) {
					result.push_back(student);
				}
			}

			// Премахваме всички студенти с това минимално време
			students.erase(
				std::remove_if(students.begin(), students.end(),
					[minTime](const StudentRecord& student) {
						return student.finishTime == minTime;
					}),
				students.end());

			return result;
		}

		
	};




};