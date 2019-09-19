DZN = ~/.npm/bin/dzn
SHELL:=/bin/bash
CPPFLAGS=-MMD -I./runtime -I./generated
CXXFLAGS=-Wno-psabi -I. -std=c++11 -pthread -l wiringPi
CXX=g++
DSRCS = $(wildcard *.dzn)
CSRCS = $(wildcard *.cc) $(wildcard runtime/*.cc) $(wildcard generated/*.cc)
OBJS = $(subst .cc,.o,$(CSRCS))
TARGET=test

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

setup: runtime generate

#generate the c++ code
generate: $(DSRCS)
	mkdir -p generated
	$(DZN) code $< -o generated

#download the c++ runtime
runtime:
	mkdir -p runtime
	mkdir -p runtime/dzn
	for f in $$($(DZN) ls /share/runtime/c++/dzn); do $(DZN) cat /share/runtime/c++/dzn/$$f > runtime/dzn/$$f; done
	for f in $$($(DZN) ls /share/runtime/c++ | grep -v dzn); do $(DZN) cat /share/runtime/c++/$$f > runtime/$$f; done

clean:
	rm -f $(OBJS) test

