SRCS:=$(wildcard *.cpp)
OBJS:=${SRCS:.cpp=.o}

2048: ${OBJS}
	g++ -lncurses -o $@ $^
