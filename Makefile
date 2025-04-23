CXX = clang++

1: 1.o
	$(CXX) $^ -o $@

2: 2.o
	$(CXX) $^ -o $@

3: 3.o RatesProvider.o
	$(CXX) $^ -o $@ $(shell wx-config --libs) -lcurl -ljsoncpp

3.o: 3.cpp
	$(CXX) -c $^ -o $@ $(CXXFLAGS) $(shell wx-config --cxxflags)

%.o: %.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

clean:
	rm -f *.o 1 2 3
