#include "Logfile.h"
#include <chrono>
#include <date.h>
#include <spdlog/spdlog.h>
#include <filesystem>

using namespace std;
using namespace chrono;
namespace fs = filesystem;

// oh boy wouldn't it suck if IO was our performance bound
const string LOG_FOLDER = "/opt/daq/";
LogFile::LogFile() {
	// ensure there is always a file to log to
	if(!fs::is_directory(LOG_FOLDER) || !fs::exists(LOG_FOLDER)) {
		spdlog::warn("No log folder found, creating the directory {}", LOG_FOLDER);
		fs::create_directory(LOG_FOLDER);
	}
	resetLogFile();
}

void LogFile::logNameChange(LogTrigger trigger, uint8_t channel, uint32_t newId) {
	lock_guard<std::mutex> lock(mutex);
	logHeader(trigger, NAME);
	file << ',' << (int)channel << ',';
	logId(newId);
	file << '\n';
}
void LogFile::logScaleChange(LogTrigger trigger, uint32_t id, float newScale) {
	lock_guard<std::mutex> lock(mutex);
	logHeader(trigger, SCALE);
	file << ',';
	logId(id);
	file << ',' << newScale << '\n';
}
void LogFile::logOffsetChange(LogTrigger trigger, uint32_t id, float newOffset) {
	lock_guard<std::mutex> lock(mutex);
	logHeader(trigger, OFFSET);
	file << ',';
	logId(id);
	file << ',' << newOffset << '\n';
}
void LogFile::logNewValue(LogTrigger trigger, uint32_t id, float calibrated, float uncalibrated) {
	lock_guard<std::mutex> lock(mutex);
	logHeader(trigger, VALUE);
	file << ',';
	logId(id);
	file << ',' << calibrated << ',' << uncalibrated << '\n';
}

void LogFile::resetLogFile() {
	lock_guard<std::mutex> lock(mutex);
	file = fstream(generateLogfilePath(), ios_base::out | ios_base::app);
	file << std::scientific; // use scientific notation to save space
}
void LogFile::logHeader(LogTrigger trigger, SubCategory cat) {
	auto now = system_clock::now();
	auto nowMs = duration_cast<milliseconds>(now.time_since_epoch()).count();
	file << (char)trigger << ',' << nowMs << ',' << (char)cat;
}
string LogFile::generateLogfilePath() {
	auto now = system_clock::now();
	return LOG_FOLDER + date::format("%FT%TZ.csv", date::floor<milliseconds>(now));
}
// look at this actual meme coding
void LogFile::logId(uint32_t id) {
	uint8_t buf[5];
	((uint32_t*) buf)[0] = id;
	buf[4] = 0;
	file << buf;
}