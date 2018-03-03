
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <stdint.h>


int width = 0;




uint32_t getColor(int x, int y, char* buff){
  int i = (x + y*width) * 4;
  return *((uint32_t *) &buff[i]);
}

void setColor(int x, int y, uint32_t col, char *buff){
  int i = (x + y*width) * 4;
  *((uint32_t *) &buff[i]) = col;
}




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

  width = var_info.xres;

  screensize = finfo.smem_len;

  fbp = (char*) mmap(0, screensize, PROT_READ | PROT_WRITE,
    MAP_SHARED, fbfd, 0);

  if ( *((int*) fbp) == -1){
    printf("Failed to mmap!!!!!!!\n");
    return -1;
  }

  char* temp = new char[screensize];
  char* temp2 = new char[screensize];

    // draw.. format BGR
  memset(fbp, 0x00, screensize);
  memset(temp, 0x00, screensize);
  //memset(fbp + screensize/2, 0x18, screensize/2);


  for (unsigned int i = 0; i < var_info.xres;i++){
    for (unsigned int j = 0; j < var_info.yres;j++){
      setColor(i,j,rand(),temp);
    }
  }
memcpy(temp2,temp,screensize);
// ? RGB

//unsigned int x = 100;
//unsigned int y = 100;

  char* a = fbp;
  char* b = temp;
  char* c = temp2;
  while (1){


    for (unsigned int i = 0; i < var_info.xres;i++){
      for (unsigned int j = 0; j < var_info.yres;j++){
        int count = 0;
        for ( int l = -1; l <= 1;l++){
          for ( int k = -1; k <= 1;k++){
            if ((l != 0 || k !=0)){
              if ( (getColor((i + l) % var_info.xres,(j + k) % var_info.yres,c) & 0x000000ff ) > 128){
                count ++;
              }
            }
          }
        }

        if ( (getColor(i,j,c) & 0x000000ff ) > 128){
          if (count < 2){
            setColor(i,j,0x00000000,b);
          }else if (count > 3 ){
            setColor(i,j,0x00000000,b);
          }else{
            setColor(i,j,0x000000ff,b);
          }
        }else if (count == 3){
          setColor(i,j,0x000000ff,b);
        }else{
          setColor(i,j,0x00000000, b);
        }
      }
    }


    memcpy(a,b,screensize);
    memcpy(c,b,screensize);
}

  delete temp;
  delete temp2;
  munmap(fbp, screensize);
  // close file
  close(fbfd);

  return 0;

}
