#pragma once
#include <mutex>
#include <fstream>
#include <memory>
#include <string>

enum LogTrigger {
    ADC = 'A',
    Thermocouple = 'T'
};
enum SubCategory {
    NAME = 'N',
    SCALE = 'S',
    OFFSET = 'O',
    VALUE = 'V'
};

class LogFile {
public:
	LogFile();
    void resetLogFile();
    void log(LogTrigger trigger, SubCategory cat, std::string values);
private:
	std::string generateLogfilePath();
    std::mutex mutex;
    std::fstream file;
};