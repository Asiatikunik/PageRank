CC = gcc
# CFLAGS = -Wall -lm
CFLAGS = -lm
EXEC = main
OBJ = main.o PageRank.o
HEAD = PageRank.h

all: clean $(EXEC) $(OBJ)
	./$(EXEC)

main: $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

main.o: main.c $(HEAD)
	$(CC) -o $@ -c $< $(CFLAGS)

PageRank.o: PageRank.c $(HEAD)
	$(CC) -o $@ -c $< $(CFLAGS)

script: clean
	gnuplot -c resultat/script_damping_temps.gp resultat/damping_temps.txt > resultat/png/damping_temps.png
	gnuplot -c resultat/script_damping_NbExecution.gp resultat/damping_NbExecution.txt > resultat/png/damping_NbExecution.png

valgrind: clean $(EXEC) $(OBJ)
	#     valgrind ./$(EXEC)
	#     valgrind --tool=memcheck --leak-check=full ./$(EXEC)
	valgrind --tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes ./$(EXEC)
	#     valgrind --tool=memcheck --leak-check=yes|no|full|summary --leak-resolution=low|med|high --show-reachable=yes ./$(EXEC)


clean: 
	rm -rf *.o
	rm -rf main