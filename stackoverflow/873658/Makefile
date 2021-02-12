DETOURS_SRC=../../Detours/src
TQ84_DEBUG_SRC=../../tq84-c-debug
# INCLUDE=-I$(DETOURS_SRC) -I$(TQ84_DEBUG_SRC)

all: hooks.dll target.exe inject.exe

hooks.dll:    hooks.o Detours.o disasm.o modules.o tq84_debug.o
	g++ -shared hooks.o Detours.o disasm.o modules.o tq84_debug.o -o hooks.dll

hooks.o : hooks.c
	gcc -DTQ84_DEBUG_ENABLED -DTQ84_DEBUG_TO_FILE -I$(TQ84_DEBUG_SRC) -I$(DETOURS_SRC) -c hooks.c

Detours.o : $(DETOURS_SRC)/Detours.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/Detours.cpp

disasm.o : $(DETOURS_SRC)/disasm.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/disasm.cpp

modules.o : $(DETOURS_SRC)/modules.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/modules.cpp

target.exe: target.c
	gcc target.c -o target.exe

inject.exe: inject.c
	gcc inject.c -o inject.exe

tq84_debug.o: $(TQ84_DEBUG_SRC)/tq84_debug.c
	gcc -DTQ84_DEBUG_ENABLED -DTQ84_DEBUG_TO_FILE -I$(TQ84_DEBUG_SRC) -c $(TQ84_DEBUG_SRC)/tq84_debug.c

