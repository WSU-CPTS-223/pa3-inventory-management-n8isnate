out: clean compile

GCC=g++
GCCFLAGS=-Wall -Werror -std=c++11 -g
EXEC=mainexe
OBJECTS=main.cpp HashMap.cpp

compile:
	$(GCC) -o $(EXEC) $(OBJECTS) $(GCCFLAGS)

execute:
	./$(EXEC)

clean:
	rm -f $(EXEC)