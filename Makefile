CXX=g++
CXXFLAGS=-c -std=c++11 -g -O0 -Wall -I/usr/local/include

LDFLAGS= -L/usr/local/lib -luv

LD=g++

OBJ-DIR=.objs

SOURCES=main.cxx tcp.cxx http.cxx log.cxx

OBJECTS=$(addprefix $(OBJ-DIR)/,$(SOURCES:.cxx=.o))
DEPS=$(OBJECTS:.o=.d)

EXECUTABLE=$(OBJ-DIR)/alloc

all: $(EXECUTABLE)

clean:
	-rm -rf $(OBJ-DIR)

$(EXECUTABLE):$(DEPS) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ-DIR)/%.o: %.cxx
	$(CXX) $(CXXFLAGS) $< -o $@

$(DEPS): $(OBJ-DIR)/.stamp

$(OBJ-DIR)/%.d: %.cxx
	$(CXX) -M $(CXXFLAGS) -o $@.tmp $<
	printf "$@ $(OBJ-DIR)/" > $@
	cat $@.tmp >> $@
	rm -f $@.tmp

$(OBJ-DIR)/.stamp:
	mkdir -p $(OBJ-DIR)
	touch $@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
