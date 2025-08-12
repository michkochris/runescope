CC = gcc
CFLAGS = -Wall -Wextra -std=c11

TARGET = runescope
TEST_PROG = test_ltrace_program

SRCS = runescope.c rune_exec.c rune_strace_parser.c rune_path_finder.c rune_ltrace_parser.c rune_analyzer.c

all: $(TARGET) $(TEST_PROG)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

$(TEST_PROG): $(TEST_PROG).c
	$(CC) $(CFLAGS) $(TEST_PROG).c -o $(TEST_PROG)

clean:
	rm -f $(TARGET) $(TEST_PROG)
