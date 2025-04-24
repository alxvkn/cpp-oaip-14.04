CXX = clang++
WX-CONFIG = wx-config

WIN32 = 0

all: 1 2 3

1: 1.o
	$(CXX) $(LDFLAGS) $^ -o $@

2: 2.o
	$(CXX) $(LDFLAGS) $^ -o $@

3: 3.o RatesProvider.o
	$(CXX) $^ -o $@ $(LDFLAGS) $(shell $(WX-CONFIG) --libs) 

3.o: 3.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(shell $(WX-CONFIG) --cxxflags)

%.o: %.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

clean:
	rm -f *.o 1 2 3

.PHONY: clean all
