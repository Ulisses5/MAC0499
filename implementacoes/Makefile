SRCS=$(wildcard *.cpp)
DEPS=$(SRCS:.cpp=.d)
OBJS=$(SRCS:.cpp=.o)
PROG=main

.SECONDARY: $(OUTS)

CXX=g++
CXXFLAGS= -std=c++17 -pedantic
RM=rm -fv

COMPILE.cpp = $(CXX) $(CXXFLAGS)
OUTPUT_OPTION= -c
LINK.o = $(CXX) $(LDFLAGS)

all: $(PROG)

%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

$(PROG) : $(OBJS)
	$(LINK.o) $^ -o $@

clean:
	$(RM) $(OBJS)
