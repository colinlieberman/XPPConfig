CXX         = g++
CXXFLAGS    = -Wall
INCPATH     = -I.
OBJECT      = test.out
SOURCES     = test.cpp

obj: $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(OBJECT) test.cpp XPPConfig.cpp $(INCPATH)

test:
	./$(OBJECT)

clean: 
	rm $(OBJECT)
