cmake_minimum_required (VERSION 3.8)
project ("daq-sampler")
set(CMAKE_CXX_STANDARD 17)

# seek out dependencies (must be pre-installed)
find_package(PkgConfig REQUIRED)
pkg_check_modules(modbus REQUIRED IMPORTED_TARGET libmodbus)

# build executable
add_executable (daq-sampler "src/main.cpp")

# perform linking
target_link_libraries(daq-sampler PkgConfig::modbus)

