CXX = g++
CXXFLAGS = -g -Wall 
ROOTFLAGS = `root-config --cflags --glibs`

# make a binary for every .cxx files
all : $(patsubst %.cxx, %.o, $(wildcard *.cxx))

# rule for each one
%.o : %.cxx
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $*.o -c $*.cxx 
	$(CXX) $(INCLUDE) $(CXXFLAGS) $(ROOTFLAGS) -o $* $*.o        
	rm $*.o

clean:
	rm -f $(wildcard *.o) $(wildcard *.so) $(patsubst %.cxx, %, $(wildcard *.cxx))
