CFlag = -Wall -g -std=gnu99

all : ext2_mkdir

mkdir : ext2_mkdir.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

%.o: %.c ext2_utils.h ext2.h
	gcc $(CFlag) -c $<

clean :
	rm *.o mkdir

reset:
	cp clean_images/* images/

