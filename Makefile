LIB_NAME=bellboy

CFLAGS=-g -O3 -Wall -Isrc

SRC=$(wildcard src/*.c src/**/*.c)
OBJ=$(patsubst %.c,%.o,${SRC})

LIB_OBJ=${OBJ}

TEST_SRC=$(wildcard test/*_test.c)
TESTS=$(patsubst %.c,%,${TEST_SRC})

all: build/${LIB_NAME}.a $(TESTS)
debug: CFLAGS+=-DDEBUG
debug: all

build/${LIB_NAME}.a: build ${LIB_OBJ}
	ar rvs $@ ${LIB_OBJ}
	ranlib $@

build:
	@mkdir -p build

clean:
	rm -rf build
	rm -rf ${OBJ}
	rm -rf ${TESTS}
	rm -rf test/*.dSYM

${TESTS}: %: %.c build/${LIB_NAME}.a
	$(CC) $(CFLAGS) -o $@ $< build/${LIB_NAME}.a

tests: debug ${TESTS}
	./test/bellboy_test