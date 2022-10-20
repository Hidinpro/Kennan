CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror

TARGE1=library
OBJECT1=main.o tools.o operator_system.o admin_system.o timer.o

TARGE2=librarystu
OBJECT2=mainstu.o student_system.o operator_system.o tools.o admin_system.o

all:

$(TARGE1):$(OBJECT1)
	$(CC) $(OBJECT1)	-o $(TARGE1)


$(TARGE2):$(OBJECT2)
	$(CC) $(OBJECT2)	-o $(TARGE2)

main.o:%.o:%.c operator_system.h admin_system.h student_system.h
	$(CC) $(STD) $(FLAG) -c $<

tools.o:%.o:%.c tools.h
	$(CC) $(STD) $(FLAG) -c $<

operator_system.o:%.o:%.c operator_system.h admin_system.h
	$(CC) $(STD) $(FLAG) -c $<

admin_system.o:%.o:%.c admin_system.h
	$(CC) $(STD) $(FLAG) -c $<

timer.o:%.o:%.c timer.h operator_system.h admin_system.h
	$(CC) $(STD) $(FLAG) -c $<

mainstu.o:%.o:%.c operator_system.h admin_system.h student_system.h
	$(CC) $(STD) $(FLAG) -c $<

student_system.o:%.o:%.c student_system.h operator_system.h admin_system.h
	$(CC) $(STD) $(FLAG) -c $<

clean:
	rm -rf $(OBJECT) $(TARGE)
	rm -rf .h.gch
