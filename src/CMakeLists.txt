set(CMAKE_CXX_STANDARD 17)
add_library(src STATIC SharedMemory.h sampler/SamplerThread.h sampler/SamplerThread.cpp sampler/LTC2448.cpp sampler/LTC2448.h Logfile.h Logfile.cpp)

# For some reason I can't put this line in the parent directory's CMakeLists.txt
target_link_libraries(src stdc++fs)
