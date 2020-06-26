#pragma once
#include "SharedMemory.h"
#include "modbus.h"

/** Modbus server wrapper that will handle network requests */
class Server {
    modbus_t *modbus_new_tcp(const char *ip, int port);
    //read and write register data from modbus
    void modbus_free(modbus_t *ctx);
};
