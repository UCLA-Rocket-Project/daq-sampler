set(CMAKE_CXX_STANDARD 17)
add_library(src STATIC SharedMemory.h sampler/SamplerThread.h sampler/SamplerThread.cpp sampler/LTC2448.cpp sampler/LTC2448.h Logfile.h Logfile.cpp)
