CC = g++

COMPILER_FLAGS = -Werror -Wextra -Wall

all : tres_sv.cpp
	$(CC) tres_sv.cpp -o  sv.out $(COMPILER_FLAGS)
	$(CC) tres_client.cpp -o  cli.out $(COMPILER_FLAGS)

clean:
	rm -rf *.out

re : clean all

.PHONY: all clean re