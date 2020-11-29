#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stddef.h>

void StoreNonVolatileData( void * pData, size_t dataSize );
void RestoreNonVolatileData( void * pData, size_t dataSize );

#endif //FILE_UTILS_H