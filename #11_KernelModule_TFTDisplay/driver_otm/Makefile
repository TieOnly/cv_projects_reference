obj-m += my_driver.o 
KDIR = /lib/modules/$(shell uname -r)/build
DRIVER_NAME = my_driver
DEVICE_FILE = /dev/my-spi-driver
###################################################################

all: clean build install chmod

###################################################################

build:
	make -C $(KDIR) M=$(shell pwd) modules CC=gcc CFLAGS="-std=gnu99"
clean:
	make -C $(KDIR) M=$(shell pwd) clean
	
###################################################################

install: $(DRIVER_NAME).ko
	sudo insmod $(DRIVER_NAME).ko
	
remove: $(DRIVER_NAME).ko
	sudo rmmod $(DRIVER_NAME).ko
	
chmod: 
	if lsmod | grep -q "$(DRIVER_NAME)"; then \
	   echo "Driver is installed"; \
	   sudo chmod 777 $(DEVICE_FILE); \
	else \
	   echo "Driver is not installed"; \
	fi
	

