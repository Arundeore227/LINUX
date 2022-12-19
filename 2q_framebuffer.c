//2. Write a program to read framebuffer fixed size information

#include<stdio.h>
#include<fcntl.h>
#include <linux/input.h>
#include<linux/fb.h>
int main(int argc, char *argv[])
{
	int fd1;

	//argv[1] has in format /dev/fb0

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 3) {
		printf("Open Fails");
		return -1;
	}
	struct fb_var_screeninfo name;
	//function call to get framebuffer size
	ioctl(fd1, FBIOGET_VSCREENINFO,&name);
	//screen information
	printf("visible resolution xres --> %u\n",name.xres);
	printf("visible resolution yres --> %u\n",name.yres);
	printf("\nvirtual resolution xres --> %u\n",name.xres_virtual);
	printf("virtual resolution yres --> %u\n",name.yres_virtual);
	printf("bits_per_pixel --> %u\n",name.bits_per_pixel);

	return 0;
}
