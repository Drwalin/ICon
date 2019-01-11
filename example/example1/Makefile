
ROOT_PATH = ../../
include ../../Makefile


	
$(BDIR)/Server1.o: Server.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
server: $(BDIR)/Server1.o $(OBJ)
	$(CXX) -o Server.exe $^ $(CFLAGS) $(LIB)

$(BDIR)/Client1.o: Client.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
client: $(BDIR)/Client1.o $(OBJ)
	$(CXX) -o Client.exe $^ $(CFLAGS) $(LIB)

all: server client


