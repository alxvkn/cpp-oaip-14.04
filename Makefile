CXX = clang++

1: 1.o
	$(CXX) $< -o $@

2: 2.o
	$(CXX) $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $^

clean:
	rm -f *.o 1 2
