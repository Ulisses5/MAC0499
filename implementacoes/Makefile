# Define source files for each target (include any shared .cpp files in both)
vEB_SRCS = vEB.cpp
WLP_SRCS = WLP.cpp WLPTest.cpp

# Generate object file lists for each target
vEB_OBJS = $(vEB_SRCS:.cpp=.o)
WLP_OBJS = $(WLP_SRCS:.cpp=.o)
ALL_OBJS = $(sort $(vEB_OBJS) $(WLP_OBJS))

# Define the two output binaries
PROG1 = vEB
PROG2 = WLPTest
PROGS = $(PROG1) $(PROG2)

CXX = g++
CXXFLAGS = -std=c++17 -pedantic -MMD -MP
RM = rm -fv

COMPILE.cpp = $(CXX) $(CXXFLAGS)
OUTPUT_OPTION = -c -o $@
LINK.o = $(CXX) $(LDFLAGS)

.PHONY: all clean

# Building 'all' will now compile both binaries
all: $(PROGS)

# Pattern rule to compile .cpp into .o
%.o: %.cpp
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

# Linking rule for the first binary
$(PROG1): $(vEB_OBJS)
	$(LINK.o) $^ -o $@

# Linking rule for the second binary
$(PROG2): $(WLP_OBJS)
	$(LINK.o) $^ -o $@

# Include auto-generated header dependencies (recompiles .o if a .h changes)
-include $(ALL_OBJS:.o=.d)

clean:
	$(RM) $(ALL_OBJS) $(ALL_OBJS:.o=.d) $(PROGS)