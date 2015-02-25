#include <iostream>
#include "main.hpp"

using namespace std;

Engine engine(80, 50);

int main() {
	try{
		engine.load();
		while ( !TCODConsole::isWindowClosed() ) {

			engine.update();

			engine.render();
			TCODConsole::flush();

		}
		//engine.save();
		return 0;
	}
	catch(...){
		cerr << "An error occurred with main()"  << endl;
		throw 0;
	}
}
