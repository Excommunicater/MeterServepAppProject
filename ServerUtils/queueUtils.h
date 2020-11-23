#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

#include <stdbool.h>
#include <stdint.h> 

int InitMessageQueue( const char * filePath );
void InitServerMessageQueue( void );
int GetServerQueueId( void );
bool GetMessageFromQueue( void * message, long messageType, int queueId );
uint32_t GetNumberOfMessagesInQueue( int queueId );
uint32_t GetNumberOfMessagesInServerQueue( void );
bool GetMessageFromServerQueue( void * message, long messageType );
bool PushMessageToQueue( void * message, long messageType, int queueId );
void RemoveQueue( int queueId );
void CleanAppQueue( int queueId );
void CleanSrvQueue( void );

#endif //QUEUE_UTILS_H