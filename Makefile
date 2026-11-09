CC ?= gcc
CFLAGS ?= -Wall -Wextra -Wpedantic
CPPFLAGS ?=
LDLIBS ?= -lm
THREAD_FLAGS = -pthread

TSP_SOURCES = main.c \
	include/utils.c \
	include/distance.c \
	include/result.c \
	include/thread.c
TEST_SOURCES = tests/test_tsp.c \
	include/distance.c \
	include/result.c \
	include/thread.c \
	include/utils.c

.PHONY: all clean test

all: tsp

tsp: $(TSP_SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(THREAD_FLAGS) $^ -o $@ $(LDLIBS)

tsp_test: $(TEST_SOURCES)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(THREAD_FLAGS) $^ -o $@ $(LDLIBS)

test: tsp_test
	./tsp_test

clean:
	rm -f tsp tsp_test main
