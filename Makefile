CXX			= g++
LD			= g++
CXXFLAGS	= -std=c++11 -g -Wall
LDFLAGS		= -lcaf_core
SOURCES		= $(wildcard *.cpp)

all: $(SOURCES:.cpp=)

%: %.o
	$(LD) $^ $(LDFLAGS) -o $@

include $(SOURCES:.cpp=.d)

%.d: %.cpp
	$(CXX) -M $(CXXFLAGS) $^ > $@

clean:
	$(RM) *.o
	$(RM) *.d
	$(RM) *.log
	$(RM) $(SOURCES:.cpp=)

.PHONY: all clean
