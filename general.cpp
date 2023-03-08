#include <iostream>
#include "general.h"

[[ noreturn ]] void Error (std::string message) {
	std::cerr << "Error: " << message << std::endl;
	exit(1);
}
