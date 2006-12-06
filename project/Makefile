#
#Makefile for the EFM-OAM layer
#

obj-m += 8023ah.o
8023ah-objs := efmoam.o efmoamproc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

