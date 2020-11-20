#ifndef MESSAGE_UTILS_H
#define MESSAGE_UTILS_H

#include <stddef.h>  // size_t
#include <stdbool.h> // bool
#include <stdint.h>  // uint32_t

size_t GetMessageSize( long type );
bool ResponseUint32( uint32_t valueToResponse, uint8_t status, long responseQueue, uint32_t requestId );

#endif // MESSAGE_UTILS_H