#ifndef METER_SRV_H
#define METER_SRV_H

#include <stdint.h>
#include <stdbool.h> // bool
#include <stddef.h>   // size_t

void InitServer( void );
void StartServer( void );
int InitMessageQueue( const char * filePath );
int InitServerMessageQueue( void );
void ReportAndExit( const char* errorMsg );
bool GetMessageFromQueue( void * message, long messageType, int queueId );
bool PushMessageToQueue( void * message, long messageType, int queueId );
size_t GetMessageSize( long type );
uint32_t NumberOfMessagesInQueue( int queueId );

#endif // METER_SRV_H