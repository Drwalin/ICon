
CXX = g++
CC = gcc
AR = ar
LIBPATHS = -L"C:/mingw-w64/lib"
CFLAGS = -Ofast -m64 -s -std=c++17 -pipe -I"C:\mingw-w64\include"

IDIR = $(ROOT_PATH)src
CDIR = $(ROOT_PATH)src
BDIR = $(ROOT_PATH)bin
LDIR = $(ROOT_PATH)lib

LIBSWIN = -lwinmm -lWs2_32 -lMswsock -lAdvApi32 -lmsvcrt -lpthread -lboost_thread-mgw81-mt-x64-1_69 -lboost_filesystem-mgw81-mt-x64-1_69 -lboost_system-mgw81-mt-x64-1_69

LIB = $(LIBPATHS) $(LIBSWIN) $(LIBSWIN)

_DEPS = BinaryType.h Connection.h DeepServer.h HighLayerSocket.h GlobalBoostAsio.h Error.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ  = BinaryType.o Connection.o DeepServer.o HighLayerSocket.o GlobalBoostAsio.o Error.o
OBJ = $(patsubst %,$(BDIR)/%,$(_OBJ))






$(BDIR)/%.o: $(CDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS)

dep: $(OBJ)
	$(AR) -c $(LDIR)/libICon.a -r $^


