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
    void logNameChange(LogTrigger trigger, uint8_t channel, uint32_t newId);
    void logScaleChange(LogTrigger trigger, uint32_t id, float newScale);
    void logOffsetChange(LogTrigger trigger, uint32_t id, float newOffset);
    void logNewValue(LogTrigger trigger, uint32_t id, float calibrated, float uncalibrated);
private:
	std::string generateLogfilePath();
    std::mutex mutex;
    std::fstream file;
    // WARNING: this method does not seek the lock
    void logHeader(LogTrigger trigger, SubCategory cat);
    void logId(uint32_t id);
};