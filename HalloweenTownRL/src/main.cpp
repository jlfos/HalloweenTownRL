#include <iostream>
#include "Engine.hpp"


Engine engine(80, 57);

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
		std::cerr << "An error occurred with main()"  << std::endl;
		throw 0;
	}
}
