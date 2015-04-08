#include <iostream>
#include "main.hpp"

using namespace std;

Engine engine(80, 50);

int main() {
	try{
		engine.Load();
		while ( !TCODConsole::isWindowClosed() ) {

			engine.Update();

			engine.Render();

		}
		//engine.save();
		return 0;
	}
	catch(...){
		cerr << "An error occurred with main()"  << endl;
		throw 0;
	}
}
