
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

// application entry point
int main(int argc, char* argv[])
{
  int fbfd = 0; // framebuffer filedescriptor
  struct fb_var_screeninfo var_info;
  struct fb_fix_screeninfo finfo;
  long int screensize = 0;
  char *fbp = 0;

  // Open the framebuffer device file for reading and writing
  fbfd = open("/dev/fb0", O_RDWR);
  if (fbfd == -1) {
    printf("Error: cannot open framebuffer device.\n");
    return(1);
  }
  printf("The framebuffer device opened.\n");

  // Get fixed screen information
 if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo)) {
   printf("Error reading fixed information.\n");
 }

  // Get variable screen information
  if (ioctl(fbfd, FBIOGET_VSCREENINFO, &var_info)) {
    printf("Error reading variable screen info.\n");
  }
  printf("Display info %dx%d, %d bpp\n",
         var_info.xres, var_info.yres,
         var_info.bits_per_pixel );

  screensize = finfo.smem_len;

  fbp = (char*) mmap(0, screensize, PROT_READ | PROT_WRITE,
    MAP_SHARED, fbfd, 0);

  if ( *((int*) fbp) == -1){
    printf("Failed to mmap!!!!!!!\n");
    return -1;
  }
    // draw.. format BGR
  memset(fbp, 0x00, screensize);
  //memset(fbp + screensize/2, 0x18, screensize/2);


  for ( int i = 3; i<screensize;i+= 4){
    fbp[i] = 0xff;
  }


  munmap(fbp, screensize);
  // close file
  close(fbfd);

  return 0;

}
