#include "Logfile.h"
#include <chrono>
#include <date.h>

using namespace std;
using namespace chrono;

LogFile::LogFile() {
	// ensure there is always a file to log to
	resetLogFile();
}

void LogFile::resetLogFile() {
	lock_guard<std::mutex> lock(mutex);
	file = fstream(generateLogfilePath(), ios_base::out | ios_base::app);
	this->file = move(file);
}
void LogFile::log(LogTrigger trigger, SubCategory cat, string values) {
	lock_guard<std::mutex> lock(mutex);
	auto now = system_clock::now();
	auto nowMs = duration_cast<milliseconds>(now.time_since_epoch()).count();
	file << (char)trigger << ',' << nowMs << ',' << (char)cat << ',' << values << '\n';
}

string LogFile::generateLogfilePath() {
	auto now = system_clock::now();
	return "/opt/daq/" + date::format("%FT%TZ.csv", date::floor<milliseconds>(now));
}