obj-m +=oskernel.o
 
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(shell pwd) modules
	$(CC) usercode.c -o usercode

clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(shell pwd) clean
	rm usercode
