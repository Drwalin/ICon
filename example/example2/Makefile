
ROOT_PATH = ../../
include ../../Makefile


	
$(BDIR)/Server2.o: Server.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
server: $(BDIR)/Server2.o $(STATICLIB)
	$(CXX) -o Server.exe $^ $(CFLAGS) $(LIB) $(STATICLIB)

$(BDIR)/Client2.o: Client.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
client: $(BDIR)/Client2.o $(STATICLIB)
	$(CXX) -o Client.exe $^ $(CFLAGS) $(LIB) $(STATICLIB)

all: server client


