#include "Engine.hpp"
#include "LoggerWrapper.hpp"

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
		LoggerWrapper::Error("An error occurred with main()");
		throw 0;
	}
}
