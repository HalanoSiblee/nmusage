CXX := $(shell command -v clang++ 2>/dev/null || command -v g++)
CXXFLAGS := -O3 -march=native -mtune=native -flto -pipe -std=c++20

TARGET := nwusage
SRC := main.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	strip -s $(TARGET)
clean:
	rm -f $(TARGET)
