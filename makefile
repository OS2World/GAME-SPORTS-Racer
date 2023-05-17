# nmake makefile
#
# Tools used:
#  Compile::Watcom Resource Compiler
#  Compile::GNU C
#  Make: nmake or GNU make
all : Racer.exe

Racer.exe : racer.obj racer.def
	gcc -Zomf racer.obj racer.def -o Racer.exe

racer.obj : racer.c 
	gcc -Wall -Zomf -c -O2 racer.c -o racer.obj

clean :
	rm -rf *exe *res *obj