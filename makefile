# Medusa language makefile
# use it to compile this program and your file writen on
# medusa language

# You can use some flags to compile
# Your file as you want.

# Set compiler for language
CXX = g++ -g -std=c++17

# Set start flags for compiler
# You can add optional flags if you set LAUNCH_MODE as DEBUG for example
CXX_FLAGS = -Wall -Wextra -Weffc++

# SET LAUNCH MODE
# Use it to add optional flags in your project
LAUNCH_MODE = DEBUG

# CXX_FLAGS += -Wc++0x-compat -Wc++11-compat -Wc++14-compat\
#  -Waggressive-loop-optimizations -Walloc-zero -Walloca\
#  -Walloca-larger-than=8192 -Warray-bounds -Wcast-align\
#  -Wcast-qual -Wchar-subscripts -Wconditionally-supported\
#  -Wconversion -Wctor-dtor-privacy -Wdangling-else\
#  -Wduplicated-branches -Wempty-body -Wfloat-equal\
#  -Wformat-nonliteral -Wformat-security -Wformat-signedness\
#  -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2\
#  -Winline -Wlarger-than=8192 -Wvla-larger-than=8192\
#  -Wlogical-op -Wmissing-declarations -Wnon-virtual-dtor\
#  -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith\
#  -Wredundant-decls -Wrestrict -Wshadow -Wsign-promo\
#  -Wstack-usage=8192 -Wstrict-null-sentinel -Wstrict-overflow=2\
#  -Wstringop-overflow=4 -Wsuggest-attribute=noreturn\
#  -Wsuggest-final-types -Wsuggest-override -Wswitch-default\
#  -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused\
#  -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers\
#  -Wnarrowing -Wno-old-style-cast -Wvarargs -Waligned-new\
#  -Walloc-size-larger-than=1073741824 -Walloc-zero -Walloca\
#  -Walloca-larger-than=8192 -Wcast-align=strict\
#  -Wdangling-else -Wduplicated-branches -Wformat-overflow=2\
#  -Wformat-truncation=2 -Wmissing-attributes -Wmultistatement-macros\
#  -Wrestrict -Wshadow=global -Wsuggest-attribute=malloc -fcheck-new\
#  -fsized-deallocation -fstack-check -fstrict-overflow\
#  -flto-odr-type-merging -fno-omit-frame-pointer\
#  -Wno-unknown-pragmas

# Execute program
# Standart process of compiling files and execute it whith win cmd
# After and before executing makefile 
# call clean process which remove all files of compiling

# Set filse which included in project src/include/tree.c  src/frontend/Main.c 
TARGETS = encoding.c

# Set name of executer program
EXECUTOR = Compiler

# Create Makefile process
all: execute clean

execute: $(EXECUTOR).exe 
	@$(EXECUTOR)

$(EXECUTOR).exe: $(TARGETS)
	@$(CXX) $(CXX_FLAGS) $(TARGETS) -o $(EXECUTOR).exe

clean:
	@del $(EXECUTOR).exe *.obj *.dot