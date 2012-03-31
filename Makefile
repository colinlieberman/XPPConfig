CXX         = g++
CXXFLAGS    = -Wall 
INCPATH     = -I.

CXXLIBFLAGS  = $(CXXFLAGS) -c -fPIC
LINKFLAGS    = -dynamiclib -Wl,-headerpad_max_install_names,-undefined,dynamic_lookup

LIBOBJECT   = xppconfig.o
LIBSOURCES  = XPPConfig.cpp XPPConfig.h

LIBNAME     = libxppconfig.so

TESTOBJECT  = test.o
TESTSOURCES = test.cpp $(LIBNAME)

lib: $(LIBSOURCES)
	$(CXX) $(CXXLIBFLAGS) -o $(LIBOBJECT) XPPConfig.cpp $(INCPATH)
	$(CXX) $(LINKFLAGS) -o $(LIBNAME) $(LIBOBJECT)

test: $(TESTSOURCES)
	$(CXX) $(CXXFLAGS) $(TESTSOURCES) -o $(TESTOBJECT) -L. -lxppconfig $(INCPATH)
	./$(TESTOBJECT)

clean:
	rm $(LIBNAME) $(LIBOBJECT) $(TESTOBJECT) 
