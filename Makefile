BINARY=bin
# can be a list
CODEDIRS=.
INCDIRS=.

CC=g++
OPT=-O3 -march=native
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra -std=gnu++20 -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS)

CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

-include $(DEPFILES)
.PHONY: all clean run
