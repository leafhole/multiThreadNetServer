GLIB_INC = $(shell pkg-config --cflags glib-2.0)
GLIB_LD = $(shell pkg-config --libs glib-2.0)

CC=g++
CFLAGS= -I ../include/  $(GLIB_INC)   -Wall -g -O0 -D_LINUX_ -Wno-deprecated
NET_SERVER = ../bin/netServer
LINKCC = $(CC) $(LDFLAGS)
LIBDIR = ../lib
LDFLAGS = -L$(LIBDIR)  $(GLIB_LD)  -L/usr/lib  -llog4cplus -levent -lpthread -O0


objects = connAttrManager.o connectionManager.o main.o netserver.o business.o connection.o dispatcher.o libthread.o
all: $(objects) $(NET_SERVER)
$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
$(NET_SERVER):$(objects)
	$(LINKCC) $(objects) -o $(NET_SERVER)
clean:
	rm *.o
