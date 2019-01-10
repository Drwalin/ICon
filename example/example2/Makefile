d=$$(date +%s)



ROOT_PATH = ../../
include ../../Makefile



client: Client.cpp $(OBJ)
	$(CXX) -o Client.exe $^ $(CFLAGS) $(LIB)

server: Server.cpp $(OBJ)
	$(CXX) -o Server.exe $^ $(CFLAGS) $(LIB)

all:
	$(MAKE) server
	$(MAKE) client







	