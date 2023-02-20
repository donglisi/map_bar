obj-m += map_bar.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc test_map.c -o test_map

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f test_map
