#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

#include <stdbool.h> // bool
#include <stdint.h>  // uint32_t

int InitMessageQueue( const char * filePath );
void InitServerMessageQueue( void );
bool GetServerQueueStatus( void );
bool GetServerQueueId( int * serverQueueId );
bool GetMessageFromQueue( void * message, long messageType, int queueId );
uint32_t GetNumberOfMessagesInQueue( int queueId );
bool GetMessageFromServerQueue( void * message, long messageType );
bool PushMessageToQueue( void * message, long messageType, int queueId );
void RemoveQueue( int queueId );
void CleanAppQueue( int queueId );
void CleanSrvQueue( void );
//void LookForMessageInQueue( int queue );

#endif //QUEUE_UTILS_H