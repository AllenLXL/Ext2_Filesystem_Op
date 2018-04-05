# Self-test
cp ./self-tester/images/emptydisk.img ./self-tester/runs/selftest1.img
cp ./self-tester/images/largefile.img ./self-tester/runs/selftest12.img

echo "Mkdir 1"
./ext2_mkdir ./self-tester/runs/selftest1.img /level1/aabb


# Copy a file
echo "Copy 1"
./ext2_cp ./self-tester/runs/selftest1.img ./self-tester/files/oneblock.txt /level1/file

# Link
echo "Hard Link 1" #error
./ext2_ln ./self-tester/runs/selftest1.img /level1 blink

echo "Hard Link 2"
./ext2_ln ./self-tester/runs/selftest1.img /level1/level2/bfile /aaaaay.txt

echo "Soft Link 1"
./ext2_ln ./self-tester/runs/selftest1.img -s /level1/level2/bfile /z.txtaaa

# Remove 3 times
# echo "Remove 1"
# ./ext2_rm ./self-tester/runs/selftest1.img /afile
# echo "Remove 2"
# ./ext2_rm ./self-tester/runs/selftest1.img /level1/level2/bfile
# echo "Remove 3"
# ./ext2_rm ./self-tester/runs/selftest1.img /level1/file
# echo "Remove Soft Link"
# ./ext2_rm ./self-tester/runs/selftest1.img /z.txtaaa
# echo "Remove Hard Link"
# ./ext2_rm ./self-tester/runs/selftest1.img /aaaaay.txt

# echo "Remove 4" # error
# ./ext2_rm ./self-tester/runs/selftest1.img /level1/aabb

# Remove Corner Case
echo "Remove Corner" # error
./ext2_rm ./self-tester/runs/selftest1.img /level1

# Restore 
# echo "Restore 1"
# ./ext2_restore ./self-tester/runs/selftest1.img /level1/level2/bfile
# echo "Restore 2"
# ./ext2_restore ./self-tester/runs/selftest1.img /level1/file
# echo "Restore 3"
# ./ext2_restore ./self-tester/runs/selftest1.img /afile

# --- Now do the dumps ---
the_files="$(ls ./self-tester/runs)"
for the_file in $the_files
do
	g=$(basename $the_file)
	./ext2_dump ./self-tester/runs/$g > ./self-tester/results/$g.txt
done

echo "Final - compare"
diff ./self-tester/results/selftest1.img.txt ./self-tester/solution-results/sample.img.txt