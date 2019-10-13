#
#		MAKEFILE
#
#	EJERCICIO: Calculo secuencial del número pi
#
#	Alba Moreno Ontiveros 2ºB, B3
#
#	SCD
#
#############################################################################################################################

all: calculopi clean

calculopi: calculopi.o fun_tiempo.o
	g++ -o calculopi calculopi.o fun_tiempo.o -lrt -lpthread 	# enlaza, genera calculopi

calculopi.o: calculopi.cpp fun_tiempo.h
	g++ -g -c calculopi.cpp	-I/fun_tiempo.h					# compila calculopi.cpp y genera calculopi.o

fun_tiempo.o: fun_tiempo.c fun_tiempo.h
	gcc -g -c fun_tiempo.c -I/fun_tiempo.h					# compila "fun_tiempos.c" y genera "fun_tiempos.o"

clean:
	rm -rf *o