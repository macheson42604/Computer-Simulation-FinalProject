 # THE NAME OF YOUR EXECUTABLE
TARGET = PROGRAM
# ALL CPP IMPLEMENTATION FILES THAT MAKE UP THE PROJECT
SRC_FILES = main.cpp PRNG.cpp StandardPRNG.cpp

# NO EDITS NEEDED BELOW THIS LINE

CXX = g++
CFLAGS = -Wall -g -std=c++17

OBJECTS = $(SRC_FILES:.cpp=.o)

ifeq ($(shell echo "Windows"), "Windows")
    TARGET := $(TARGET).exe
    DEL = del
else
    DEL = rm
endif

all: $(TARGET)

debug: CFLAGS += -DDEBUG
debug: clean $(TARGET)

release: clean $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^

.cpp.o:
	$(CXX) $(CFLAGS) -o $@ -c $<

clean:
	$(DEL) $(TARGET) $(OBJECTS)

# DEPENDENCIES
main.o: main.cpp StandardPRNG.h
PRNG.o: PRNG.cpp PRNG.h
StandardPRNG.o: StandardPRNG.cpp StandardPRNG.h PRNG.h