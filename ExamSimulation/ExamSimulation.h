#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <queue>
#include <stack>

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
};

// ----------------------------------------

// обвивка за queue с приоритетна функционалност
struct StudentQueueWrapper
{
private:
	std::queue<StudentInfo> queue;

public:
	// Функция за добавяне на елементи в опашката с приоритет
	void enqueue(const StudentInfo& student) {
		// Ако опашката е празна, просто добавяме елемента
		if (queue.empty()) {
			queue.push(student);
			return;
		}

		// Помощен контейнер за временно съхранение
		std::queue<StudentInfo> tempQueue;
		bool inserted = false;

		// Прехвърляне на елементите, спазвайки приоритета
		while (!queue.empty()) {
			const StudentInfo& current = queue.front();

			if (!inserted && (student.course == 2 && student.course < current.course))
			{
				tempQueue.push(student);
				inserted = true;
			}

			tempQueue.push(current);
			queue.pop();
		}

		// Ако елементът не е добавен (т.е. има най-нисък приоритет), добавяме го накрая
		if (!inserted) {
			tempQueue.push(student);
		}

		// Прехвърляне обратно в основната опашка
		queue = std::move(tempQueue);
	}

	// Функция за премахване на първия елемент
	StudentInfo dequeue() {
		if (queue.empty()) {
			throw std::runtime_error("Queue is empty!");
		}
		StudentInfo frontStudent = queue.front();
		queue.pop();
		return frontStudent;
	}

	// Проверка дали опашката е празна
	bool isEmpty() const {
		return queue.empty();
	}

	// Връщане на размера на опашката
	size_t size() const {
		return queue.size();
	}
};

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
			return false; // Няма свободни места
		}
	}

	int minTime()
	{
		int minTime = std::min_element(students.begin(), students.end(),
			[](const StudentRecord& a, const StudentRecord& b) {
				return a.finishTime < b.finishTime;
			})->finishTime;

		return minTime;
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

struct ExamResult
{
	int StudentFinushTime;
	int LectorFinichTime;
	std::stack<StudentRecord> stackResult;
};

ExamResult ProcessExam(InputData inputData, int roomCapacity)
{

	ExamResult examResult;

	int GlobalTime = 0;
	int LectorTakanTo = 0;
	int LectorStartToWork = 0;
	bool LectorIsActive = false;
	
	// 1. Опапка с вдодните данне
	std::queue<StudentInfo> studentArrivalData;
	for (StudentInfo studentInfo : inputData.studentsArraival)
	{
		studentArrivalData.push(studentInfo);
	}

	// 2. Приоритизирана опашка, за студентите пред залата
	StudentQueueWrapper studentQueue;

	// 3. обект wrapper с приоретизирана опаша за студентите в залата
	ExamRoom examRoom(roomCapacity);

	// 4. Stack за непроверените работи
	std::stack<StudentRecord> unckeckExdam;

	// 5. Stack за проверените работи
	std::stack<StudentRecord> readyExdam;

	// procesing students exam
	while (!studentArrivalData.empty() || !studentQueue.isEmpty() || !examRoom.students.empty())
	{
		// полълване на студентската опашка / попълваме само тези които са пристугнали до този мемент (GlobalTime)
		while (!studentArrivalData.empty() && studentArrivalData.front().time <= GlobalTime )
		{
			StudentInfo studentInfo = studentArrivalData.front();
			studentArrivalData.pop();
			studentQueue.enqueue(studentInfo);
		}
		
		// запълваме свободни места в залата / докато иам места или докато има студенти на опашката
		while (!studentQueue.isEmpty() && examRoom.capacity > examRoom.students.size())
		{
			StudentInfo studentInfo = studentQueue.dequeue();
			examRoom.addStudent(studentInfo.facultyNumber, studentInfo.duration + GlobalTime);
		}

		// изчисляваме GlobalTime 
		int nextStudentTimeArrival = studentArrivalData.empty() ? examRoom.minTime() : studentArrivalData.front().time;
		int mixFinishTimeForWorkingStudent = examRoom.students.size() == 0 ? GlobalTime : examRoom.minTime();
		int maxCurrentTime = std::min(nextStudentTimeArrival, mixFinishTimeForWorkingStudent);

		GlobalTime = maxCurrentTime;

		// предаваме работите които са ориключили в това време 
		if (maxCurrentTime == mixFinishTimeForWorkingStudent)
		{
			std::vector<StudentRecord> studentFinishedExam = examRoom.removeStudentWithMinTime();
			
			// update unchecket to checked exam 
			if (!LectorIsActive && GlobalTime >= inputData.lectorInfo.arrivalMunute)
			{
				LectorIsActive = true;
				LectorTakanTo = GlobalTime + studentFinishedExam.size() * inputData.lectorInfo.timeForCheck;
				LectorStartToWork = GlobalTime;
			}
			else if (LectorIsActive && (GlobalTime - LectorStartToWork) > 0)
			{
				int lectorTimeInterval = GlobalTime - LectorStartToWork;
				int checkedCount = lectorTimeInterval / inputData.lectorInfo.timeForCheck;
				int count = 1;
				while (count <= checkedCount && !unckeckExdam.empty())
				{
					StudentRecord studentRecord = unckeckExdam.top();
					studentRecord.finishTime = LectorStartToWork + count * inputData.lectorInfo.timeForCheck;
					readyExdam.push(studentRecord);
					unckeckExdam.pop();
					count++;
				}

				bool holdExamInHands = !unckeckExdam.empty() && lectorTimeInterval % inputData.lectorInfo.timeForCheck != 0;
				if (holdExamInHands)
				{
					StudentRecord studentRecord = unckeckExdam.top();
					studentRecord.finishTime = LectorStartToWork +  (checkedCount + 1)  *  inputData.lectorInfo.timeForCheck;
					readyExdam.push(studentRecord);
					unckeckExdam.pop();
					LectorStartToWork = LectorStartToWork + (checkedCount + 1) * inputData.lectorInfo.timeForCheck;
				}
				else
				{
					if (!unckeckExdam.empty())
					{
						LectorStartToWork = LectorStartToWork + (checkedCount) * inputData.lectorInfo.timeForCheck;
					}
					else
					{
						LectorStartToWork = GlobalTime;
					}

				}

			}
				
			// оставяне за проверка
			for (StudentRecord studentRecord : studentFinishedExam)
			{
				unckeckExdam.push(studentRecord);
			}
		}

		
		
	}

	examResult.StudentFinushTime = GlobalTime;
	// update rest of unchecket to checked exam when all studen finish exam
	int count = 1;
	while (!unckeckExdam.empty())
	{
		StudentRecord studentRecord = unckeckExdam.top();
		studentRecord.finishTime = LectorStartToWork + count * inputData.lectorInfo.timeForCheck;
		readyExdam.push(studentRecord);
		unckeckExdam.pop();
		count++;
	}

	examResult.LectorFinichTime = readyExdam.top().finishTime;
	examResult.stackResult = readyExdam;

	return examResult;
}