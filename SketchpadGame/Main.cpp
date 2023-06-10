#include <iostream>
#include "Engine.h"

int main() {
	Engine& engine = Engine::getInstance();
	engine.run();	
	return 0;
}
