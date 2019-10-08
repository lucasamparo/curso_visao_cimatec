CXX := g++

CXXFLAGS = -std=c++11 -O3

INCLUDE = `pkg-config --cflags opencv`

LDFLAGS := `pkg-config --libs opencv`

%.out: build/%.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)

clean:
	rm -f build/*.o
	rm -f *.out
