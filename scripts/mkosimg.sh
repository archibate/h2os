set -e
dd if=/dev/zero of=os.img bs=1024 count=1440
mkfs.fat -R 16 os.img
dd if=grub4dos/grldr.mbr of=os.img bs=1 count=3 conv=notrunc
dd if=grub4dos/grldr.mbr of=os.img bs=1 skip=94 seek=94 count=$((16*512-94)) conv=notrunc
sudo umount /mnt || true
sudo mount os.img /mnt
sudo cp grub4dos/grldr /mnt
sudo cp grub4dos/grub.exe /mnt
sudo umount /mnt
