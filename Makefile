bin=Http_server
cc=g++
LDFLAGS=-lpthread -fpermissive
Http_server:Http_server.cpp
	$(cc) -o $@ $^ $(LDFLAGS) -std=c++11
.PHONY:clean
clean:
	rm -rf $(bin)
