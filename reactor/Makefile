CC = g++
objects = main.o reactor.o worker_thread.o

server : $(objects)
	$(CC) -o $@ $(objects) -lpthread

main.o : main.cc reactor.h worker_thread.h
	$(CC) -g -std=c++11 -c main.cc

reactor.o : reactor.cc reactor.h worker_thread.h
	$(CC) -g -std=c++11 -c reactor.cc

worker_thread.o : worker_thread.cc worker_thread.h
	$(CC) -g -std=c++11 -c worker_thread.cc

.PHONY : clean
clean : 
	-rm server $(objects)
