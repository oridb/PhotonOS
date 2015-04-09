PhotonOS
===========

Inspired from photon elementary particle, PhotonOS is light, fast and simple. It is develop as a hobby, only for educational purpose, so there enventualy are some bugs and bad code style. It can be used for simple tasks, because it is under developement. Current version is v0.0.1 and its features are:

	- Basic VGA driver in text mode
	- GDT, IDT and TSS support
	- Interrupt support
	- Keyboard and PIT drivers
	- Some C Standard Library functions
	- Virtual memory manager
	- Real time clock

How to build?	
================

You must have installed on your computer:

	- a linux OS
	
	- i686-elf-gcc
	
	- i686-elf-ar
	
	- nasm


Now, in source directory (PhotonOS/src) run following commands:

	- ./configure.sh
	
	- ./clean.sh
	
	- ./build.sh
	
	- ./iso.sh


Now, you will have in sysroot folder (PhotonOS/src/sysroot) the kernel ELF, boot configuration, headers and libs. In source directory (PhotonOS/src) you have a iso file with kernel.

! You can use only one script to build (./all.sh), but it will run kernel too, so you will need a qemu-system-i386 virtual machine.

How to run?
==============

You can run kernel by using ./run.sh script, but you will need a qemu-system-i386 virtual machine. Another way to run is to write kernel on a USB or to configure GRUB to load it on real hardware.

How to contribue?
==================
If you want to contribue send me a mail at mihailferaru2000@gmail.com and say why you want to join developing. You will receive a mail with instructions with what to do next.
