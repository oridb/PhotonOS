.PHONY: all clean install install-headers install-libs

all: install install-headers install-libs photonos.elf

photonos.elf: $(BOOT_OBJS) $(INIT_OBJS) $(SYS_OBJS)
	$(CC) $(LDFLAGS) $(BOOT_OBJS) $(INIT_OBJS) $(SYS_OBJS) $(LIBC_OBJS) $(LIBS) -o photonos.elf

%.o: %.s
	i686-elf-as $< -o $@

%.o: %.asm
	$(AS) $< -o $@ $(ASMFLAGS)
	
%.o: %.c
	$(CC) $< -o $@ $(CFLAGS) $(INCLUDE)

install:
	mkdir ../sysroot/usr
	mkdir ../sysroot/boot
	mkdir ../sysroot/boot/grub
	touch ../sysroot/boot/grub/grub.cfg
	echo "menuentry \"myos\" {\n\tmultiboot /boot/photonos.elf\n}" > ../sysroot/boot/grub/grub.cfg
	
install-headers:
	mkdir ../sysroot/usr/include
	cp include/. ../sysroot/usr/include -R
	cp ../libc/include/. ../sysroot/usr/include -R
	
install-libs:
	mkdir ../sysroot/usr/lib
	mv ../libc/libc.a ../sysroot/usr/lib
	
clean:
	rm -f $(INIT_OBJS) $(BOOT_OBJS) $(SYS_OBJS)
	rm -f photonos.elf
