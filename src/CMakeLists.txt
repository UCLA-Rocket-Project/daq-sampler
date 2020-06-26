set(CMAKE_CXX_STANDARD 17)
add_library(src STATIC SharedMemory.h sampler/SamplerThread.h sampler/SamplerThread.cpp network/Server.cpp network/Server.h sampler/LTC2448.cpp sampler/LTC2448.h sampler/LTC2984.cpp sampler/LTC2984.h)
