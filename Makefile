
CXX = g++
CC = gcc
AR = ar
LIBPATH = C:/mingw-w64/lib
LIBPATHS = -L$(LIBPATH)
INCLUDEPATHS = -I"C:/mingw-w64/include"
CFLAGS = -Ofast -s -m64 -std=c++17 -pipe

IDIR = $(ROOT_PATH)src
CDIR = $(ROOT_PATH)src
BDIR = $(ROOT_PATH)bin
LDIR = $(ROOT_PATH)lib

LIBSWIN = -lwinmm -lWs2_32 -lMswsock -lAdvApi32 -lmsvcrt -lpthread

LIBSBOOST_ = libboost_thread-mgw81-mt-x64-1_69.a libboost_filesystem-mgw81-mt-x64-1_69.a libboost_system-mgw81-mt-x64-1_69.a
LIBBOOST = $(patsubst %,$(LIBPATH)/%,$(LIBSBOOST_))


LIB = $(LIBPATHS) $(LIBSWIN)

_DEPS = DeepServer.h HighLayerSocket.h GlobalBoostAsio.h Error.h LoopBuffer.h BinaryType.h Connection.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ  = DeepServer.o HighLayerSocket.o GlobalBoostAsio.o Error.o LoopBuffer.o BinaryType.o Connection.o
OBJ = $(patsubst %,$(BDIR)/%,$(_OBJ))

STATICLIB = $(LDIR)/libICon.a



$(BDIR)/%.o: $(CDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)

$(STATICLIB): dep

dep: $(OBJ)
	$(AR) -c $(STATICLIB) -r $^ $(LIBBOOST)


	
