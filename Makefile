
CXX = g++
CC = gcc
AR = ar
LIBPATHS = -L"C:/mingw-w64/lib"
INCLUDEPATHS = -I"C:/mingw-w64/include"
CFLAGS = -Ofast -s -m64 -std=c++17 -pipe

IDIR = $(ROOT_PATH)src
CDIR = $(ROOT_PATH)src
BDIR = $(ROOT_PATH)bin
LDIR = $(ROOT_PATH)lib

LIBSWIN = -lwinmm -lWs2_32 -lMswsock -lAdvApi32 -lmsvcrt -lpthread -lboost_thread-mgw81-mt-x64-1_69 -lboost_filesystem-mgw81-mt-x64-1_69 -lboost_system-mgw81-mt-x64-1_69

LIB = $(LIBPATHS) $(LIBSWIN) $(LIBSWIN)

_DEPS = DeepServer.h HighLayerSocket.h GlobalBoostAsio.h Error.h LoopBuffer.h BinaryType.h Connection.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ  = DeepServer.o HighLayerSocket.o GlobalBoostAsio.o Error.o LoopBuffer.o BinaryType.o Connection.o
OBJ = $(patsubst %,$(BDIR)/%,$(_OBJ))



$(BDIR)/%.o: $(CDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS) -time

dep: $(OBJ)
	$(AR) -c $(LDIR)/libICon.a -r $^


	