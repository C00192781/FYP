#pragma once

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Logging
{
public:
	Logging();
	~Logging();

	void clearCSVFile();
	void Initialize();
	void LogLineToCSVFile(std::string algorithm, int startNode, int goalNode, int time, int pathLength);

private:

	std::ofstream csv;
};
