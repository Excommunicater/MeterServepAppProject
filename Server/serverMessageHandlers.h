#ifndef SERVER_MESSAGE_HANDLERS_H
#define SERVER_MESSAGE_HANDLERS_H

void HandleSingleGetRequest( void );
void HandleSingleSetRequest( void );
void HandleResetRequest( void );
void HandleSubscriptionRequest( void );
void HandleSendingNotifications( void );
void HandleNotificationResponse( void );

#endif // SERVER_MESSAGE_HANDLERS_H