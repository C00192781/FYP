#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "Demo.h"
#include "SFML\Graphics.hpp"


using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

// Written By: Kevin Boylan
// Submitted: 22/04/2018
// Application that allows you to select between two dynamic search algorithms: LPA*, AD*
// and a plain search algorithm for comparison: A* (uses UCS to calculate heuristics) 


// CPP file where loops are handled - Demo.cpp

int main(int argc, char *argv[]) {

	Demo* demo = new Demo();

	demo->Initialize();

	while (demo->IsRunning())
	{
		//demo->Render();
		demo->Update();
		
	}

	system("PAUSE");	
}