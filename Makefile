
#
#	Makefile 
#

DEBUG = -g
CC = qcc
LD = qcc


TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntox86
#TARGET = -Vgcc_ntoarmv7le
#TARGET = -Vgcc_ntoaarch64le


CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS+= $(DEBUG) $(TARGET)
BINS = server bgh_yg_UseCases Menu_bgh_yg_UseCases mkhUseCases UseCases67 stopServer
all: $(BINS)

clean:
	rm -f *.o $(BINS);
#	cd solutions; make clean


server.o: server.c server.h time.h supervisor.h techSupport.h shift.h

