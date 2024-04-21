all: my_program

my_program: BlackJack.o
	g++ -o BlackJack BlackJack.o
	chmod +x BlackJack

clean:
	rm -f BlackJack BlackJack.o

