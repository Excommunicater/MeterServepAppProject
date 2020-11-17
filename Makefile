# Compiler
CC=gcc
# Includes
INC=-I./includes
# Compilation Flags
CFLAGS=-c -Wall $(INC)

DEBUG=-D name=DEBUG

all: metering_hw serverStart appStart

metering_hw: metering_hw.o
	$(CC) metering_hw.o -o metering_hw

serverStart: serverStart.o meter_srv.o
	$(CC) serverStart.o meter_srv.o -o serverStart

#serverStartDebug: serverStart.o meter_srvDEBUG.o
#	$(CC) serverStart.o meter_srvDEBUG.o -o serverStart

appStart: meter_app.o meter_srv.o 
	$(CC) meter_app.o meter_srv.o -o appStart
	$(CC) meter_app.o meter_srv.o -o meter_app

meter_svr.o: meter_srv.c
	$(CC) $(CFLAGS) meter_srv.c

#meter_srvDEBUG.o: meter_srv.c
#	$(CC) $(CFLAGS) $(DEBUG) meter_srv.c -o meter_srvDEBUG.o

metering_hw.o: metering_hw.c
	$(CC) $(CFLAGS) metering_hw.c

serverStart.o: serverStart.c
	$(CC) $(CFLAGS) serverStart.c

meter_app.o: meter_app.c
	$(CC) $(CFLAGS) meter_app.c

clean:
	rm -rf *.o metering_hw serverStart appStart meterhw.fifo meter_app
