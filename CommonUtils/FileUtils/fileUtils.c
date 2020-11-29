//--External includes-------------------------------------------------
#include <stdio.h>   
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
//--------------------------------------------------------------------

//--Project includes--------------------------------------------------
#include "fileUtils.h"
#include "errorHandling.h"
//--------------------------------------------------------------------

//--Defines-----------------------------------------------------------
#define DATA_FILE_PATH "./Data.bin"
//--------------------------------------------------------------------

//--Private Function Declaration--------------------------------------
bool IsFileExist( void );
size_t GetFileSize( void );
void CreateFile( void );
//--------------------------------------------------------------------

//--File Scope Global Variables---------------------------------------
static FILE * pFile;
//--------------------------------------------------------------------

void StoreNonVolatileData( void * pData, size_t dataSize )
{
    // Open and write from beginning
    pFile = fopen( DATA_FILE_PATH, "wb+");

    if ( IsFileExist() == false )
    {
        ReportAndExit("StoreNonVolatileData - File do not exist, or cannot open it...\r\n");
    }
    if ( fwrite( pData, dataSize, 1, pFile ) != 1 )
    {
        ReportAndExit("StoreNonVolatileData - Problem storing structure to file\r\n");
    }
    if ( fclose( pFile ) == EOF )
    {
        ReportAndExit("StoreNonVolatileData - Error closing the file\r\n");
    }
    if ( GetFileSize() < dataSize )
    {
        ReportAndExit("StoreNonVolatileData - file after save smaller than bytes size\r\n");
    }
}

void RestoreNonVolatileData( void * pData, size_t dataSize )
{
    if ( IsFileExist() == false )
    {
        // File do not exist - create one!
        CreateFile();
        return;
    }
    if ( GetFileSize() < dataSize )
    {
        printf("WARNING! File is corrupted... Remove corrupted one and create new one\r\n");
        remove(DATA_FILE_PATH);
        CreateFile();
        return;
    }

    // Read binary from beginning
    pFile = fopen(DATA_FILE_PATH,"rb+");

    if ( pFile == (FILE*)NULL )
    {
        ReportAndExit("RestoreNonVolatileData - Unexpected error opening file, get null pointer\r\n");
    }

    if ( fread( pData, dataSize, 1, pFile ) != 1 )
    {
        ReportAndExit("RestoreNonVolatileData - Unexpected error opening file, read less elements than expected\r\n");
    }
    if ( fclose( pFile ) == EOF )
    {
        ReportAndExit("RestoreNonVolatileData - Error closing the file\r\n");
    }

}

size_t GetFileSize( void )
{
    struct stat st;
    stat(DATA_FILE_PATH, &st);
    return (size_t)st.st_size;
}


bool IsFileExist( void )
{
    bool retVal = false;
    if( access( DATA_FILE_PATH, F_OK ) != -1 ) 
    {
        // file exists
        retVal = true;
    } 
    return retVal;
}

void CreateFile( void )
{
    pFile = fopen( DATA_FILE_PATH, "wb+");
    if ( pFile == (FILE*)NULL )
    {
        ReportAndExit("CreateFile - Unexpected error opening file, get null pointer\r\n");
    }
    if ( fclose( pFile ) == EOF )
    {
        ReportAndExit("CreateFile - Error closing the file\r\n");
    }
}

