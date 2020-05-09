all: sistema

sistema: registro.o cabecalho.o sistema.o binarionatela.o
	gcc registro.o cabecalho.o sistema.o binarionatela.o -o arquivoTrabalho -Wall

registro.o: registro.c
	gcc registro.c -c -o registro.o -Wall

cabecalho.o: cabecalho.c
	gcc cabecalho.c -c -o cabecalho.o -Wall

sistema.o: sistema.c registro.h cabecalho.h
	gcc sistema.c -c -o sistema.o -Wall

binarionatela.o:  binarionatela.c
	gcc binarionatela.c -c -o binarionatela.o -Wall

clean:
	rm -rf *.o

run:
	./arquivoTrabalho
