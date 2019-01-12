
ROOT_PATH = ../../
include ../../Makefile


	
$(BDIR)/Server3.o: Server.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
server: $(BDIR)/Server3.o $(STATICLIB)
	$(CXX) -o Server.exe $^ $(CFLAGS) $(LIB) $(STATICLIB)

$(BDIR)/Client3.o: Client.cpp
	$(CXX) -c -o $@ $< $(CFLAGS) $(INCLUDEPATHS)
client: $(BDIR)/Client3.o $(STATICLIB)
	$(CXX) -o Client.exe $^ $(CFLAGS) $(LIB) $(STATICLIB)

all: server client


