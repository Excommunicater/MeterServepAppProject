#include "errorHandling.h"
#include <stdio.h>
#include <stdlib.h>             // exit()

void ReportAndExit( const char* errorMsg )
{
    perror(errorMsg);
    exit(-1);
}