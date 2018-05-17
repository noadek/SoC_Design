#!/bin/sh

mount /dev/mmcblk0p1 /mnt
cd /mnt
insmod gpio.ko
insmod uio_pdrv_genirq.ko of_id=generic-uio
./change_ip_and_mac.sh 25
./mixer a
