CFlag = -Wall -g -std=gnu99

all : ext2_mkdir ext2_cp ext2_ln ext2_rm ext2_restore ext2_checker ext2_rm_bonus

ext2_cp: ext2_cp.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_restore: ext2_restore.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_checker: ext2_checker.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_rm: ext2_rm.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_rm_bonus: ext2_rm_bonus.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_ln: ext2_ln.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

ext2_mkdir: ext2_mkdir.o ext2_utils.o
	gcc $(CFlag) -o $@ $^

%.o: %.c ext2_utils.h ext2.h
	gcc $(CFlag) -c $<

clean :
	rm *.o ext2_mkdir ext2_cp ext2_ln ext2_rm ext2_restore ext2_checker ext2_rm_bonus

reset :
	cp clean_images/* images/

