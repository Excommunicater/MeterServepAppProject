//--External includes-------------------------------------------------
#include <stdio.h>              // pritnf()
#include <sys/stat.h>           // mkfifo()
#include <sys/ipc.h>            // key_t
#include <sys/msg.h>            // msgget()
#include <stdlib.h>             // exit()
//--------------------------------------------------------------------

//--Local includes----------------------------------------------------
#include "../ServerUtils/queueUtils.h"        //InitServerMessageQueue(), GetNumberOfMessagesInQueue()
#include "../ServerUtils/errorHandling.h"     //ReportAndExit()
#include "../commonIncludes/serverMessages.h" 
#include "../commonIncludes/projectGenerals.h" 
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define APP_PATH "./ApplicationStart/meter_app.c"
#define APP_REV  666

//--------------------------------------------------------------------



//--Global Variables--------------------------------------------------
//--------------------------------------------------------------------

//--Function Declaration----------------------------------------------

//--------------------------------------------------------------------

int main( void )
{
    return 0;
}

