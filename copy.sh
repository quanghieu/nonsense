#!/bin/bash

mkdir -p mount
sudo mount /dev/sdb1 mount/
sudo cp arch/arm/boot/zImage mount/
sync
sudo umount mount/
