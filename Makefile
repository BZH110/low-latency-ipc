CC=g++
CFLAGS=--std=c++14 -g -O2

TARGETS=alice bob

all: ${TARGETS}

%: %.cpp common.h
	${CC} ${CFLAGS} $< -o $@

clean:
	rm -f ${TARGETS}
