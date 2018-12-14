#Output program name to make this Makefile more reusable between programming exercises
PNAME = BST

#Project Tree Structure
LDIR = lib
SDIR = src
ODIR = obj
BDIR = bin
DDIR = dep

#Compiler flag variables
INC        = $(SDIR)
INC_PARAMS = $(foreach d, $(INC), -I$d)
DEPFLAGS   = -MT $@ -MMD -MP -MF $(DDIR)/$*.Td
CPPFLAGS   = -std=c++11 -g -O0
LINKFLAGS  = 

#The full command for compilation
CPPC       = g++ $(CPPFLAGS) $(INC_PARAMS) $(DEPFLAGS)
CPPL       = g++ $(CPPFLAGS)
POSTCPPC   = @mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

#Default make target: setup the environment, build the program, build and run tests
.PHONY: all
all: | toolchain $(PNAME)

#List all sources to be included in the project (except main.cpp)
_SRCS = 

#Derived variable SRCS used by dependency management
SRCS = main2.cpp $(_SRCS)

#Derived variables POBJ/TOBJ lists dependencies for the output binaries
POBJ = $(ODIR)/main.o $(patsubst %.cpp, $(ODIR)/%.o, $(_SRCS))

#Program link step
$(BDIR)/$(PNAME): $(POBJ)
	$(CPPL) -o $@ $^ $(LINKFLAGS)

#All compilation units in the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DDIR)/%.d
	$(CPPC) -c -o $@ $<
	$(POSTCPPC)

#Standard build targets
.PHONY: clean
clean:
	rm -rf $(DDIR) $(ODIR) $(BDIR)

.PHONY: toolchain pmain $(PNAME)
toolchain: | $(BDIR) $(ODIR) $(DDIR)
pmain: $(BDIR)/$(PNAME)
$(PNAME): | toolchain pmain

$(DDIR):
	@mkdir $(DDIR)

$(ODIR):
	@mkdir $(ODIR)

$(BDIR):
	@mkdir $(BDIR)

#Dependency management
$(DDIR)/%.d: ;
.PRECIOUS: $(DDIR)/%.d

include $(wildcard $(patsubst %,$(DDIR)/%.d,$(basename $(SRCS))))

