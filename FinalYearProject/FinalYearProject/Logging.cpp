#include "Logging.h"

Logging::Logging()
{
}

Logging::~Logging()
{
}

/// <summary>
/// Completely clear CSV file for fresh data
/// </summary>
void Logging::clearCSVFile()
{
	std::ofstream csv;
	std::string filename = "Test.csv";
	csv.open("Test.csv", ios::out | ios::trunc);
	csv.close();
}

/// <summary>
/// Add in headers after clearing CSV file
/// </summary>
void Logging::Initialize()
{
	clearCSVFile();
	csv.open("Test.csv", ios::app);
	csv << "Algorithm" << "," << "Start Node" << "," << "Goal Node" << "," << "Time (Milliseconds)" << "," << "Path Length" << "," << "Cell Expansions" << "," << "Inflation Value" << std::endl;
	csv.close();
}

/// <summary>
/// Function used for each data log to the CSV file
/// </summary>
void Logging::LogLineToCSVFile(std::string algorithm, int startNode, int goalNode, int time, int pathLength, int cellExpansions, int inflation)
{
	csv.open("Test.csv", ios::app);
	csv << algorithm << "," << startNode << "," << goalNode << "," << time << "," << pathLength << "," << cellExpansions << "," << inflation << "\n";
	//csv << "\n";
	csv.close();
}
