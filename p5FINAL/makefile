.PHONY: x clean tar

EJECUTABLE= practica_objetos_B5

MODULOS:= practica_objetos_B5.o\
         objetos_B5.o\
         file_ply_stl.o\
      
LIBS:=-L/usr/X11R6/lib/ -lglut -lGLU -lGL -lX11 -lsupc++ -lm -lc -lstdc++ -lpthread -L/usr/lib/nvidia-331
CPPFLAGS:= -Wall -fmax-errors=2 -g

CC= g++

x: $(EJECUTABLE)
	./$(EJECUTABLE) beethoven

$(EJECUTABLE): $(MODULOS)	
	$(CC) -o $(EJECUTABLE) $(LDFLAGS) $(MODULOS) $(LIBS) 

clean:
	rm -rf *.o $(EJECUTABLE)

