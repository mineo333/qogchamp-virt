############################ object declaration ############################

MODULE_NAME := virt

obj-m += $(MODULE_NAME).o

OBJ := virt_main.o

$(MODULE_NAME)-y := $(patsubst %.o, src/%.o, $(OBJ))

############################ directories ############################

KDIR := /lib/modules/$(shell uname -r)/build #kernel source

BUILD_DIR := $(PWD)/bin #output directory

############################ Kbuild options ############################

CFLAGS_MODULE := -I$(PWD)/include -Wno-declaration-after-statement #this warning clobbers the make

KBUILD_OUTPUT:= $(PWD)/bin


all:
	mkdir -p bin
	@echo $($(MODULE_NAME)-y)
	make -C $(KDIR) M=$(PWD) modules
	mv src/*.o $(MODULE_NAME).mod* modules.order $(BUILD_DIR) #clean up
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
