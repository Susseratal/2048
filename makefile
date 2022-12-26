SRCS:=$(wildcard *.cpp)
OBJS:=${SRCS:.cpp=.o}

bin/2048: ${OBJS}
	g++ -lncurses -o $@ $^

clean:
	rm bin/2048 *.o
