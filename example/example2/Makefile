
ROOT_PATH = ../../
include ../../Makefile


	
$(BDIR)/Server2.o: Server.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
server: $(BDIR)/Server2.o $(OBJ)
	$(CXX) -o Server.exe $^ $(CFLAGS) $(LIB)

$(BDIR)/Client2.o: Client.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
client: $(BDIR)/Client2.o $(OBJ)
	$(CXX) -o Client.exe $^ $(CFLAGS) $(LIB)

all: server client


