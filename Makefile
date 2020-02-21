#####REFERENCE#####
#
#  Automatic Variables:
#  target : dependencies
#       commands
#
#  $^ means all of the dependencies
#  $@ means the target
#  $< means the first dependency
#
#  patsubst
#  $(patsubst pattern,replacement,text)
#  Finds whitespace-separated words in text that match pattern and replaces them with replacement
#####END REFERENCE#####

CC = em++
#em++ src\main.cpp -Iinclude -o SetThemFree.html
#-ggdb compiles with debug symbols
#-mwindows compiles without terminal

CFLAGS = 
LINKERS =-s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -std=c++11 --preload-file png 

SRCDIR = src
OBJDIR = obj
BINDIR = bin
HDRDIR = include

SRCS = $(wildcard $(SRCDIR)/*.cpp)

HEADERS = $(wildcard $(HDRDIR)/*.h)

OBJFILES = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

SRCFILES = $(patsubst $(SRCDIR)/%,%,$(SRCS))
HEADERFILES = $(patsubst $(HDRDIR)/%,%,$(HEADERS))

GAMENAME = SetThemFree
INCLUDES = -Iinclude                                                     \

#Need to put the linkers at the end of the call
$(BINDIR)/$(GAMENAME).html: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKERS) --shell-file customShell.html

#Note the -c tells the compiler to create obj files

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(HDRDIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LINKERS)

#Makes it so that if these files exist, it won't mess up Makefile
.PHONY: clean clearScreen all

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(BINDIR)/$(GAMENAME).html
	rm -f $(BINDIR)/$(GAMENAME).js
	rm -f $(BINDIR)/$(GAMENAME).wasm
	rm -f $(BINDIR)/$(GAMENAME).data


clearScreen:
	clear

#This target will clean, clearscreen, then make project
all: clean clearScreen $(BINDIR)/$(GAMENAME).html

#This target prints out variable names, just type:
#make print-VARIABLE
print-%  : ; @echo $* = $($*)
