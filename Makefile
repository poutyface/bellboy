LIB_NAME=pouty_face

CFLAGS=-g -O3 -Wall -Isrc

SRC=$(wildcard src/*.c src/**/*.c)
OBJ=$(patsubst %.c,%.o,${SRC})

LIB_OBJ=${OBJ}

TEST_SRC=$(wildcard test/*_test.c)
TESTS=$(patsubst %.c, %, ${TEST_SRC})

all: build/${LIB_NAME}.a $(TESTS)

build/${LIB_NAME}.a: build ${LIB_OBJ}
	ar rcs $@ ${LIB_OBJ}
	ranlib $@

build:
	@mkdir -p build

clean:
	rm -rf build
	rm -rf ${OBJ}
	rm -fr ${TESTS}

$(TESTS): %: %.c
	$(CC) $(CFLAGS) -o $@ $< build/${LIB_NAME}.a

tests:
	sh ./tests/test