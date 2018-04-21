#include <iostream>
#include <fstream>
#include <string>
#include <utility>  
#include <map> 

#include "Graph.h"
#include "SFML\Graphics.hpp"

#include "Demo.h"

//#include <stdio.h>
//#include <unistd.h>


using namespace std;
using std::pair;

#define screenWidth 1080
#define screenHeight 720

//typedef GraphArc<std::string, int> Arc;
//typedef GraphNode<pair<std::string, int>, int> Node;

int main(int argc, char *argv[]) {

	Demo* demo = new Demo();

	demo->Initialize();

	while (demo->IsRunning())
	{
		//demo->Render();
		demo->Update();
		
	}

	//return 0;
	system("PAUSE");

	
}