GXX = g++
LDFLAGS = -lgtest -lgtest_main
INCLUDE_DIR=-I../Security/Inc -IInc
VERSION=-std=c++17
SRC_FILES=Src/test.cpp ../Security/Src/timeunit.cpp
FLAGS = $(VERSION) $(LDFLAGS) $(INCLUDE_DIR) $(SRC_FILES) -o unit_test

all:
	$(GXX) $(FLAGS)

clean:
	rm unit_test