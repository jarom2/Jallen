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
CPPFLAGS   = -std=c++11 -g
LINKFLAGS  = -lrt -pthread

#The full command for compilation
CPPC       = g++ $(CPPFLAGS) $(INC_PARAMS) $(DEPFLAGS)
CPPL       = g++ $(CPPFLAGS)
POSTCPPC   = @mv -f $(DDIR)/$*.Td $(DDIR)/$*.d && touch $@

#List all main cpp files
SRCS   = shmForked.cpp shmClient.cpp shmServer1.cpp
	#mqcalc.cpp mqpart1.cpp mqpart2.cpp smcalc.cpp smpart1.cpp smpart2.cpp
_BINS  = $(patsubst %.cpp,$(BDIR)/%,$(SRCS))

#Default make target: setup the environment, build the program, build and run tests
.PHONY: all
all: | toolchain $(_BINS)

#Program link step
$(BDIR)/%: $(ODIR)/%.o
	$(CPPL) -o $@ $^ $(LINKFLAGS)

#All compilation units in the project
$(ODIR)/%.o: $(SDIR)/%.cpp $(DDIR)/%.d
	$(CPPC) -c -o $@ $<
	$(POSTCPPC)

#Standard build targets
.PHONY: clean
clean:
	rm -rf $(DDIR) $(ODIR) $(BDIR)

.PHONY: toolchain
toolchain: | $(BDIR) $(ODIR) $(DDIR)

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
