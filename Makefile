object = Lab001_final.c
demo=Lab001_final
$(demo) :$(object)
	gcc -o $(demo) $(object) -I include -L lib -lraylib -lgdi32 -lwinmm
