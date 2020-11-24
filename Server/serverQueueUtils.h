#ifndef SERVER_QUEUE_UTILS_H
#define SERVER_QUEUE_UTILS_H

#include <stdbool.h>
#include <stdint.h> 

void InitServerMessageQueue( void );
int GetServerQueueId( void );
uint32_t GetNumberOfMessagesInServerQueue( void );
bool GetMessageFromServerQueue( void * message, long messageType );
void CleanServerQueue( void );

#endif //QUEUE_UTILS_H