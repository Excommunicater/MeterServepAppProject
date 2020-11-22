//--Local includes----------------------------------------------------
#include "notificationUtils.h"
#include "dataUtils.h" // GetInstatntenousPhaseVoltage()
#include "errorHandling.h"
#include "../commonIncludes/metering_interface.h" // PHASE_CNT
//--------------------------------------------------------------------
#include <stdbool.h>
#include <stdio.h>  // printf()
#include <stdlib.h> // malloc()
#include <time.h>   // time()

//--Defines-----------------------------------------------------------
//#define NU_DBG_PRNT
//--------------------------------------------------------------------

//--Local Structures--------------------------------------------------
typedef struct thresehold
{
    uint32_t value;
    bool isSet;
} thresehold_t;

typedef struct subscriptionRecord
{
    bool isActive;
    uint8_t phase; 
    theseholdType_t type; 
    uint8_t notificationId; 
    int notificationQueue;
    uint32_t sentNotificationMessageId;
    struct subscriptionRecord * pNext;
} subscriptionRecord_t;
//--------------------------------------------------------------------

//--Consts------------------------------------------------------------
const uint8_t MAXIMUM_SUBSCRIPTION_NUMBER = 5U;
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static thresehold_t underVoltageThresehold[PHASE_CNT] = {0};
static thresehold_t overVoltageThresehold[PHASE_CNT] = {0};
static subscriptionRecord_t * pSubscriptionListHead = NULL;
uint8_t actualNumberOfSubscriptions = 0U;
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
void AddNewSubscription( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue );
uint8_t GetNewUniqueNotificationId( void );
subscriptionRecord_t * FindLastElement( void );
bool CheckSubscriptionDuplication( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue );
bool CheckSubscriptionForNotification( subscriptionRecord_t * subscription );
uint32_t GetUniqueNotificationMessageId( void );
#ifdef NU_DBG_PRNT
    void PrintRecordList( void );
#endif
//--------------------------------------------------------------------

shortConfirmationValues_t SetVoltageThresehold( uint8_t phase, theseholdType_t typeToSet, uint32_t valueToSet )
{
    
    if ( ( typeToSet > OVERVOLTAGE ) || ( typeToSet < UNDERVOLTAGE ) )
    {
        ReportAndExit("SetVoltageThresehold - passed bad value to set!");
    }

    shortConfirmationValues_t respVal = ERROR;
    if ( phase < PHASE_CNT )
    {
        if ( typeToSet == UNDERVOLTAGE )
        {
            underVoltageThresehold[phase].value = valueToSet;
            underVoltageThresehold[phase].isSet = true;
        }
        else
        {
            overVoltageThresehold[phase].value = valueToSet;
            overVoltageThresehold[phase].isSet = true;
        }
        respVal = OK;
    }
    else
    {
        respVal = BAD_INSTANCE;
    }
    return respVal;
}

shortConfirmationValues_t ResetVoltageThresehold( uint8_t phase, theseholdType_t typeToReset )
{
    if ( ( typeToReset > OVERVOLTAGE ) || ( typeToReset < UNDERVOLTAGE ) )
    {
        ReportAndExit("ResetVoltageThresehold - passed bad value to set!");
    }
    shortConfirmationValues_t respVal = ERROR;
    if ( phase < PHASE_CNT )
    {
        if ( typeToReset == UNDERVOLTAGE )
        {
            underVoltageThresehold[phase].isSet = false;
        }
        else
        {
            overVoltageThresehold[phase].isSet = false;
        }
        respVal = OK;
    }
    else
    {
        respVal = BAD_INSTANCE;
    }
    return respVal;
}

subscriptionRegistrationStatus_t RegisterSubscription( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue )
{
    printf("RegisterSubscription\r\n");
    #ifdef NU_DBG_PRNT
        PrintRecordList();
    #endif
    subscriptionRegistrationStatus_t retVal = SUBSCRIPTION_LIST_FULL;
    if ( actualNumberOfSubscriptions < MAXIMUM_SUBSCRIPTION_NUMBER )
    {
        if ( ( type > OVERVOLTAGE ) || ( type < UNDERVOLTAGE ) )
        {
            retVal = SUBSCRIPTION_BAD_SUBSCRIPTION_REQUEST;
        }
        else
        {
            if ( !CheckSubscriptionDuplication( phase, type, notificationId, notificationQueue ) )
            {
                AddNewSubscription( phase, type, notificationId, notificationQueue );
                retVal = SUBSCRIPTION_REGISTERED;
            }
            else
            {
                retVal = SUBSCRIPTION_ALREADY_EXIST;
            }
        }
    }
    return retVal;
}

void AddNewSubscription( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue )
{
    #ifdef NU_DBG_PRNT
        printf("AddNewSubscription\r\n");
    #endif

    uint8_t newId = GetNewUniqueNotificationId();

    // Create a new record
    subscriptionRecord_t * pNewRecord = malloc(sizeof(subscriptionRecord_t));
    if ( pNewRecord == (subscriptionRecord_t*)NULL )
    {
        ReportAndExit("AddNewSubscription - Problem during memory allocation!");
    }

    subscriptionRecord_t * pLastRecord = FindLastElement();
    if ( pLastRecord == (subscriptionRecord_t*)NULL )
    {
        // It's first element!
        pSubscriptionListHead = pNewRecord;
    }
    else
    {
        pLastRecord->pNext = pNewRecord;
    }

    actualNumberOfSubscriptions++;
    pNewRecord->isActive            = true;
    pNewRecord->notificationId      = newId;
    pNewRecord->notificationQueue   = notificationQueue;
    pNewRecord->phase               = phase;
    pNewRecord->type                = type;
    pNewRecord->pNext               = (subscriptionRecord_t*)NULL;
    *notificationId = newId;
    #ifdef NU_DBG_PRNT
        printf("Created new subscription!");
        printf("NI:%i; NQ=%i; P=%i; T=%i; TS=%i\r\n", pNewRecord->notificationId, pNewRecord->notificationQueue, pNewRecord->phase, pNewRecord->type, actualNumberOfSubscriptions );
    #endif
}

uint8_t GetNewUniqueNotificationId( void )
{
    uint8_t respVal = 0U;
    subscriptionRecord_t * pTempPointer = pSubscriptionListHead;
    uint8_t iterator = 0U;
    while( pTempPointer != (subscriptionRecord_t*)NULL )
    {
        iterator++;
        if( respVal < pTempPointer->notificationId )
        {
            respVal = pTempPointer->notificationId;
        }
        pTempPointer = pTempPointer->pNext;
    }
    respVal++;
    return respVal;
}

subscriptionRecord_t* FindLastElement( void )
{
    subscriptionRecord_t * pRetVal = NULL;
    subscriptionRecord_t * pTemp = pSubscriptionListHead;

    while ( pTemp != (subscriptionRecord_t*)NULL )
    {
        pRetVal = pTemp;
        pTemp = pRetVal->pNext;
    }
    return pRetVal;
}

bool CheckSubscriptionDuplication( uint8_t phase, theseholdType_t type, uint8_t * notificationId, int notificationQueue )
{
    subscriptionRecord_t * pTemp = pSubscriptionListHead;
    bool retVal = false;

    while ( pTemp != (subscriptionRecord_t*)NULL )
    {
        if (( pTemp->phase == phase ) &&
            ( pTemp->type == type ) && 
            ( pTemp->notificationQueue == notificationQueue ) )
        {
            retVal = true;
            break;
        }
        pTemp = pTemp->pNext;
    }
    return retVal;
}

shortConfirmationValues_t Unsubscribe( uint8_t notificationId )
{
    #ifdef NU_DBG_PRNT
        printf("Unsubscribe - ID=%i\r\n", notificationId);
        void PrintRecordList();
    #endif
    shortConfirmationValues_t retVal = BAD_INSTANCE;
    subscriptionRecord_t * pPrev = (subscriptionRecord_t*)NULL;
    subscriptionRecord_t * pRemove = pSubscriptionListHead;

    // Record to remove is the first one
    if ( ( pRemove != (subscriptionRecord_t*)NULL ) && ( pRemove->notificationId == notificationId ) )
    {
        pSubscriptionListHead = pRemove->pNext;
        free( pRemove );
        retVal = OK;
        #ifdef NU_DBG_PRNT
            printf("Find element (first) and removed\r\n");
            void PrintRecordList( void );
        #endif
        actualNumberOfSubscriptions--;
        return retVal;
    }

    while ( pRemove != (subscriptionRecord_t*)NULL )
    {
        if ( pRemove->notificationId == notificationId )
        {
            pPrev->pNext = pRemove->pNext;
            free( pRemove );
            retVal = OK;
            #ifdef NU_DBG_PRNT
                printf("Find element and removed\r\n");
                void PrintRecordList( void );
            #endif
            actualNumberOfSubscriptions--;
            return retVal;
        }
        else
        {
            pPrev = pRemove;
            pRemove = pRemove->pNext;
        }
    }
    #ifdef NU_DBG_PRNT
        printf("Element not found!\r\n");
    #endif
    return retVal;
}

shortConfirmationValues_t UnsubscribeAll( void )
{
    subscriptionRecord_t * pTemp = pSubscriptionListHead;
    subscriptionRecord_t * pRemove = (subscriptionRecord_t*)NULL;
    while ( pTemp != (subscriptionRecord_t*)NULL )
    {
        pRemove = pTemp;
        pTemp = pTemp->pNext;
        free( pRemove );
        actualNumberOfSubscriptions--;
    }
    pSubscriptionListHead = (subscriptionRecord_t*)NULL;
    return OK;
}

uint32_t GetNumberOfSubscriptions( void )
{
    return actualNumberOfSubscriptions;
}

uint32_t GetNumberOfActiveSubscriptions( void )
{
    uint32_t numberOfActiveSubscription = 0U;
    subscriptionRecord_t * pTemp = pSubscriptionListHead;
    while ( pTemp != (subscriptionRecord_t*)NULL )
    {
        if ( pTemp->isActive == true )
        {
            numberOfActiveSubscription++;
        }
        pTemp = pTemp->pNext;
    }
    return numberOfActiveSubscription;
}

bool PopNotification( notification_t * notification )
{
    bool status = false;
    notification_t retVal = {0};
    subscriptionRecord_t * pTemp = pSubscriptionListHead;

    while ( pTemp != (subscriptionRecord_t*)NULL )
    {
        if ( CheckSubscriptionForNotification( pTemp ) )
        {
            // Block checking this notification until client send OK response for this particular notification
            pTemp->isActive                  = false;
            pTemp->sentNotificationMessageId = GetUniqueNotificationMessageId();
            #if SERVER_64_BIT == true
                retVal.timeStamp  = (uint64_t)time(NULL);
            #elif
                retVal.timeStamp  = (uint32_t)time(NULL);
            #endif
            retVal.notificationID        = pTemp->notificationId;
            retVal.queueToSend           = pTemp->notificationQueue;
            retVal.notificationMessageId = pTemp->sentNotificationMessageId;
            memcpy( notification, &retVal, sizeof(notification_t) );
            status = true;
            break;
        }
        pTemp = pTemp->pNext;
    }
    return status;
}


bool CheckSubscriptionForNotification( subscriptionRecord_t * subscription )
{
    bool retVal = false;
    if ( subscription->isActive )
    {
        uint8_t phaseToCheck = subscription->phase;

        shortConfirmationValues_t status;
        uint32_t instatntenousPhaseVoltage = GetInstatntenousPhaseVoltage( &status, phaseToCheck );
        if ( status != OK )
        {
            ReportAndExit("CheckSubscriptionForNotification - Problem getting Instatntenous Phase Voltage...");
        }

        if ( ( subscription->type == UNDERVOLTAGE ) && ( underVoltageThresehold[phaseToCheck].isSet ) )
        {
            if ( instatntenousPhaseVoltage < underVoltageThresehold[phaseToCheck].value )
            {
                retVal = true;
            }
        }
        else if ( ( subscription->type == OVERVOLTAGE ) && ( overVoltageThresehold[phaseToCheck].isSet ) )
        {
            if ( instatntenousPhaseVoltage > overVoltageThresehold[phaseToCheck].value )
            {
                retVal = true;
            }
        }
    }
    return retVal;
}

uint32_t GetUniqueNotificationMessageId( void )
{
    static uint32_t id = 1;
    id++;
    return id;
}


#ifdef NU_DBG_PRNT
    void PrintRecordList( void )
    {
        subscriptionRecord_t * pTemp = pSubscriptionListHead;
        uint8_t i = 0U;
        printf("PrintRecordList - Number of records %i\r\n", actualNumberOfSubscriptions);
        while ( pTemp != (subscriptionRecord_t*)NULL )
        {
            printf("Record No.:%i ", i);
            printf("IsActive=%i; phase=%i; type=%i; notificationId=%i; notificationQueue=%i; pNext=%i\r\n", (uint8_t)pTemp->isActive, pTemp->phase, pTemp->type, pTemp->notificationId, pTemp->notificationQueue, pTemp->pNext);
            pTemp = pTemp->pNext;
            i++;
        }
        printf("\r\n\n");
    }
#endif
