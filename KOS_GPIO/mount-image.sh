sudo mount /dev/sdb1 ~/mnt/fat32
sudo cp build/einit/kos-image ~/mnt/fat32/kos-image
sync

sudo umount ~/mnt/fat32

# Created by fanglores
# github.com/fanglores
# Moscow Aviation Institute featuring Kaspersky Laboratory, hackaton
# Moscow, 2022