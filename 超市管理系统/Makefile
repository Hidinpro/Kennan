CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
TARGE=market
OBJECT=main.o tools.o manager.o logistics.o sale.o score.o login_menus.o administrators.o

$(TARGE):$(OBJECT)
	$(CC) $(OBJECT) -o $(TARGE)

main.o:%.o:%.c main.c manager.h logistics.h login_menus.h
	$(CC) $(STD) $(FLAG) -c $<

tools.o:%.o:%.c tools.c tools.h
	$(CC) $(STD) $(FLAG) -c $<

manager.o:manager.c manager.h tools.h
	$(CC) $(STD) $(FLAG) -c $<

logistics.o:logistics.c logistics.h manager.h
	$(CC) $(STD) $(FLAG) -c $<

sale.o:sale.c sale.h administrators.h
	$(CC) $(STD) $(FLAG) -c $<

score.o:score.c score.h 
	$(CC) $(STD) $(FLAG) -c $<

login_menus.o:login_menus.c login_menus.h 
	$(CC) $(STD) $(FLAG) -c $<

administrators.o:administrators.c administrators.h 
	$(CC) $(STD) $(FLAG) -c $<


clean:
	rm -rf $(OBJECT) $(TARGE)
	rm -rf .h.gch

