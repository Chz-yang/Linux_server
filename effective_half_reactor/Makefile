CC = g++
objects = main.o reactor.o event_handler.o

server : $(objects)
	$(CC) -o $@ $(objects) -lpthread

%.o : %.cc
	$(CC) -g -std=c++11 -c $^

.PHONY : clean
clean : 
	-rm server $(objects)
