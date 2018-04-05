# Self-test
#cp ./images/emptydisk.img ./self-tester/runs/testsyq.img

echo "Mkdir 1"
./ext2_mkdir images/emptydisk.img /level1
#
echo "Mkdir 1"
./ext2_mkdir images/emptydisk.img /level1/level2
#
#
## Copy a file
echo "Copy 1"
./ext2_cp images/emptydisk.img oneblock.txt /level1/level2/bfile
#
echo "Copy 1"
./ext2_cp images/emptydisk.img oneblock.txt /afile
#
## Link
echo "Hard Link 1" #error
./ext2_ln images/emptydisk.img /level1 blink
#
echo "Hard Link 2"
./ext2_ln images/emptydisk.img /level1/level2/bfile /hard
#
echo "Soft Link 1"
./ext2_ln images/emptydisk.img -s /level1/level2/bfile /soft

# Remove 3 times
# echo "Remove 2"
# ./ext2_rm images/emptydisk.img /level1/level2/bfile

echo "Remove Soft Link"
./ext2_rm images/emptydisk.img /soft
#
echo "Remove a"
./ext2_rm images/emptydisk.img /afile

echo "Remove Hard Link"
./ext2_rm images/emptydisk.img /hard

# echo "Remove 4" # error
# ./ext2_rm ./self-tester/runs/testsyq.img /level1/aabb

# # Remove Corner Casel
# echo "Remove Corner" # error
# ./ext2_rm ./self-tester/runs/testsyq.img /level1

# Restore
echo "Restore 1"
./ext2_restore images/emptydisk.img /afile
#
echo "Restore Soft Link"
./ext2_restore images/emptydisk.img /soft




# --- Now do the dumps ---
#the_files="$(ls ./self-tester/runs)"
#for the_file in $the_files
#do
#	g=$(basename $the_file)
#	./ext2_dump ./self-tester/runs/$g > ./self-tester/self-results/$g.txt
#done
# echo "Final - compare"
# diff ./self-tester/results/testsyq.img.txt ./self-tester/solution-results/sample.img.txt
