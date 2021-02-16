DETOURS_SRC=../Detours/src
TQ84_DEBUG_SRC=../tq84-c-debug

all: hooks.dll setDate.exe target.exe

hooks.dll:    hooks.o Detours.o disasm.o modules.o tq84_debug.o
	g++ -shared hooks.o Detours.o disasm.o modules.o tq84_debug.o -lntdll -o hooks.dll

hooks.o : hooks.c
	gcc -DTQ84_DEBUG_ENABLED -DTQ84_DEBUG_TO_FILE -I$(TQ84_DEBUG_SRC) -I$(DETOURS_SRC) -c hooks.c

Detours.o : $(DETOURS_SRC)/Detours.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/Detours.cpp

disasm.o : $(DETOURS_SRC)/disasm.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/disasm.cpp

modules.o : $(DETOURS_SRC)/modules.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/modules.cpp

creatwth.o : $(DETOURS_SRC)/creatwth.cpp
	g++ -I$(DETOURS_SRC) -c $(DETOURS_SRC)/creatwth.cpp

setDate.exe:           setDate.o creatwth.o detours.o disasm.o modules.o
	g++                  setDate.o creatwth.o detours.o disasm.o modules.o -o setDate.exe

target.exe: target.o
	gcc target.o -o target.exe

# setDate.exe:           setDate.o Detours.o disasm.o modules.o
#	g++                  setDate.o Detours.o disasm.o modules.o -o setDate.exe

setDate.o: setDate.c
	gcc -I$(DETOURS_SRC) -c setDate.c

tq84_debug.o: $(TQ84_DEBUG_SRC)/tq84_debug.c
	gcc -DTQ84_DEBUG_ENABLED -DTQ84_DEBUG_TO_FILE -I$(TQ84_DEBUG_SRC) -c $(TQ84_DEBUG_SRC)/tq84_debug.c

target.o: target.c
	gcc -c target.c
