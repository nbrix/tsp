PROGS := tsp 

all: $(PROGS)

tsp: tsp.cpp
	g++ -g tsp.cpp -o tsp

clean:
	rm -f $(PROGS) *.o *~
