//--External includes-------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "errorHandling.h"
//--------------------------------------------------------------------

void ReportAndExit( const char* errorMsg )
{
    perror(errorMsg);
    exit(-1);
}