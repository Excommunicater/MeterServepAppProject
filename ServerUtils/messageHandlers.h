#ifndef MESSAGE_HANDLERS_H
#define MESSAGE_HANDLERS_H

void HandleSingleGetRequest( void );
void HandleSingleSetRequest( void );
void HandleResetRequest( void );
void HandleSubscriptionRequest( void );
void HandleSendingNotifications( void );
void HandleNotificationResponse( void );

#endif // MESSAGE_HANDLERS_H