#include "src/Logfile.h"
#include <spdlog/spdlog.h>
using namespace std;

int main() {
	spdlog::error("Have a nice day");
	LogFile logger;
	const char* id = "abcd";
	uint32_t *idPtr = (uint32_t*)id;
	logger.logNameChange(ADC, 4, *idPtr);
	return 0;
}
