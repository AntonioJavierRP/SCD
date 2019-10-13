all: prod-cons_LIFO prod-cons_FIFO fumadores

clean: 
	@echo "cleaning..."
	rm *.o prod-cons_LIFO prod-cons_FIFO fumadores

prod-cons_FIFO.o: prod-cons_FIFO.cpp
	g++ -o $@ -c $^

prod-cons_FIFO: prod-cons_FIFO.o
	g++ -o $@ $^ -lrt -lpthread  

prod-cons_LIFO.o: prod-cons_LIFO.cpp
	g++ -o	$@ -c $^

prod-cons_LIFO: prod-cons_LIFO.o
	g++ -o $@ $^ -lrt -lpthread

fumadores.o: fumadores.cpp
	g++ -o $@ -c $^

fumadores: fumadores.o
	g++ -o fumadores fumadores.o -lrt -lpthread