CC=g++
CFLAGS=--std=c++14 -g -O3

TARGETS=alice bob test

all: ${TARGETS}

%: %.cpp common.h
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -f ${TARGETS}
