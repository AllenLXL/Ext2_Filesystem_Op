 echo "Mkdir /level1/level2"
./ext2_mkdir images/onedirectory.img  /level1/level2

echo "Mkdir /level1/level2/fuck1"
./ext2_mkdir images/onedirectory.img  /level1/level2/fuck1

echo "Mkdir /level1/level2/fuck2"
./ext2_mkdir images/onedirectory.img /level1/level2/fuck2

echo "cp /level1/temp1"
./ext2_cp images/onedirectory.img oneblock.txt /level1/temp1

echo "cp /level1/temp2"
./ext2_cp images/onedirectory.img oneblock.txt /level1/temp2

echo "cp /level1/temp3"
./ext2_cp images/onedirectory.img largefile.txt /level1/temp3

echo "hln /level1/hard1"
./ext2_ln images/onedirectory.img /level1/temp1 /level1/hard1

echo "hln /level1/hard2"
./ext2_ln images/onedirectory.img /level1/temp1 /level1/hard2

echo "hln /hard3"
./ext2_ln images/onedirectory.img /level1/temp2 /hard3

echo "sln /hard4"
./ext2_ln images/onedirectory.img -s /level1/temp3 /level1/hard4

echo "rm /level1"
./ext2_rm_bonus images/onedirectory.img -r /level1/

echo "rm /hard3"
./ext2_rm_bonus images/onedirectory.img /hard3

echo "diff with empty"
#./ext2_dump images/onedirectory.img > temp1
#./ext2_dump images/emptydisk.img > temp2
#
./readimage images/onedirectory.img > temp1
./readimage images/emptydisk.img > temp2

echo "now showing results"
diff temp1 temp2