#include "Logging.h"

Logging::Logging()
{
}

Logging::~Logging()
{
}

void Logging::clearCSVFile()
{
	std::ofstream csv;
	//std::ofstream fs;

	std::string filename = "Test.csv";

	csv.open("Test.csv", ios::out | ios::trunc);

	csv.close();
}

void Logging::Initialize()
{
	clearCSVFile();

	std::ofstream csv;

	csv.open("Test.csv", ios::app);

	//csv << "Column A" << "," << "Column B" << "Column C" << std::endl;
	csv << "Algorithm A" << "," << "Time Taken" << std::endl;


	//csv << 44 << "," << 44 << "," << 788 << std::endl;

	csv.close();
	//check = true;
}
