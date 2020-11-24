#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

#include <stdbool.h>
#include <stdint.h>

int InitMessageQueue( const char * filePath );
bool PushMessageToQueue( void * message, long messageType, int queueId );
bool GetMessageFromQueue( void * message, long messageType, int queueId );

uint32_t GetNumberOfMessagesInQueue( int queueId );
void RemoveQueue( int queueId );
void CleanQueue( int queueId );

#endif //QUEUE_UTILS_H