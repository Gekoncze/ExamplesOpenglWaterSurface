#include "Logger.h"
#include <fstream>
#include <iostream>

using namespace cz_mg_logger;

std::string Logger::filename = "log.txt";

void Logger::log(std::string message)
{
	std::ofstream log;
	log.open(filename, std::ios_base::app);
	log << message << std::endl;
	log.close();
}
