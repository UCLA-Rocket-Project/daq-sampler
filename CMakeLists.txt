cmake_minimum_required (VERSION 3.8)
project ("daq-sampler" CXX)
set(CMAKE_CXX_STANDARD 17)

# seek out dependencies (must be pre-installed)
find_package(PkgConfig REQUIRED)
pkg_check_modules(modbus REQUIRED IMPORTED_TARGET libmodbus)
pkg_check_modules(mraa REQUIRED IMPORTED_TARGET mraa)

# seek out dependencies in the local folders
add_subdirectory(spdlog)
include_directories(spdlog/include)
include_directories(include/)
add_subdirectory(src)
add_subdirectory(test)

# build executable
add_executable (daq-sampler main.cpp)

# perform linking
target_link_libraries(daq-sampler PkgConfig::modbus PkgConfig::mraa spdlog src)

