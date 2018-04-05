# Self-test
# cp self-tester/images/twolevel.img images/twolevel.img
echo "Mkdir 1"
./ext2_mkdir images/twolevel.img /level1/aabb

# Copy a file
echo "Copy 1"
./ext2_cp images/twolevel.img self-tester/files/oneblock.txt /level1/file


echo "Soft Link 1"
./ext2_ln images/twolevel.img -s /level1/level2/bfile /z.txtaaa


# --- Now do the dumps ---
echo "dupm to temp1"
./ext2_dump images/twolevel.img > temp1
