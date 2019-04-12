OBJ = main.o screen.o sound.o comm.o
APPNAME = sound.out

$(APPNAME) : $(OBJ)
	gcc -o $(APPNAME) $(OBJ) -lm -lcurl

%.o : %.c
	gcc -c -o $@ $<

clean :
	rm $(OBJ) $(APPNAME)

zip :
	tar cf sound.tar *.c *.h makefile
