#!/bin/bash
sudo mount /dev/sdb2 mount/
sudo make modules_install ARCH=arm INSTALL_MOD_PATH=mount/
sync
sudo umount mount/
