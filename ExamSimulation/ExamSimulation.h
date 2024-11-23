#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

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