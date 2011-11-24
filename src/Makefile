CC=g++
CFLAGS= -I ../include/ -Wall -g -O0 -D_LINUX_ -Wno-deprecated
NET_SERVER = ../bin/netServer
LINKCC = $(CC) $(LDFLAGS)
LIBDIR = ../lib
LDFLAGS = -L$(LIBDIR) -L/usr/lib  -llog4cplus -levent -lpthread -O0


objects = connAttrManager.o connectionManager.o main.o netserver.o business.o connection.o dispatcher.o libthread.o
all: $(objects) $(NET_SERVER)
$(objects): %.o: %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
$(NET_SERVER):$(objects)
	$(LINKCC) $(objects) -o $(NET_SERVER)
	#mv $(NET_SERVER) ../bin/
clean:
	rm *.o
