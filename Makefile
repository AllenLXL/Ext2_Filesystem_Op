CFlag = -Wall -g -std=gnu99

fs : ext2_mkdir.o ext2_utils.o
	gcc $(CFlag) -o fs $^

cp : ext2_cp.o ext2_utils.o
	gcc $(CFlag) -o cp $^

mkdir : ext2_mkdir.o ext2_utils.o
	gcc $(CFlag) -o cp $^

%.o : %.c ext2_utils.h ext2.h
	gcc $(CFlag) -c $<

clean :
	rm -f *.o traffic *~

