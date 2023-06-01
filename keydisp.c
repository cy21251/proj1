#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <linux/input.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include<sys/time.h>

int t=0;

// peripheral register physical address
#define GPIO_PHY_BASEADDR  0x3F200000
#define GPIO_AREA_SIZE	4096	// PAGE_SIZE
#define GPIO_GPFSEL0	0x0000	// for gpio 0..9, MSB 2bits are reserved
// omit GPFSEL1..GPFSEL5
#define GPIO_GPSET0	0x001C	// gpio 0..31
#define GPIO_GPSET1	0x0020	// gpio 32..53
#define GPIO_GPCLR0	0x0028	// gpio 0..31
#define GPIO_GPCLR1	0x002C	// gpio 32..53
#define GPIO_GPLEV0	0x0034	// gpio 0..31
#define GPIO_GPLEV1	0x0038	// gpio 32..53



void ledOnOff();
unsigned int memread(void *baseaddr, int offset);
void memwrite(void *baseaddr, int offset, unsigned int x);

void *gpio_baseaddr;

void ledOnOff()
{
    unsigned int gpfsel0;
    gpfsel0= memread(gpio_baseaddr, GPIO_GPFSEL0);	// get GPIO5 mode
    gpfsel0 = gpfsel0 | (1<<15);		// 15=GPIO5*3, bit15 ON
    memwrite(gpio_baseaddr, GPIO_GPFSEL0, gpfsel0);	// GPIO5 output mode
    memwrite(gpio_baseaddr, GPIO_GPSET0, (1 << 5));	// GPIO5 high
    sleep(1);
    memwrite(gpio_baseaddr, GPIO_GPCLR0, (1 << 5));	// GPIO5 low
}

unsigned int memread(void *baseaddr, int offset)
{
    unsigned int *p;
    p = baseaddr+offset;
    return *p;	// read memory-mapped register
}

void memwrite(void *baseaddr, int offset, unsigned int x)
{
    unsigned int *p;
    p = baseaddr+offset;
    *p = x;	// write memory-mapped register
}

int lcd_cmdwrite(int fd, unsigned char dat)
{
	unsigned char buff[2];
	buff[0] = 0;
	buff[1] = dat;
	return write(fd,buff,2);
}

int lcd_datawrite(int fd, char dat[])
{
	int len;
	char buff[100];

	len = strlen(dat);  // don't count EOS (Null char)
	if (len>99) {printf("too long string\n"); exit(1); }
	memcpy(buff+1, dat, len);	// shift 1 byte, ignore EOS
	buff[0] = 0x40;	// DATA Write command
	return write(fd, buff, len+1);
}

void initLCD(int fd)
{
	int i;
	unsigned char init1[]={ 0x38, 0x39, 0x14, 0x70, 0x56, 0x6c };
	unsigned char init2[]={ 0x38, 0x0c, 0x01 };

	usleep(100000);	// wait 100ms
	for (i=0;i<sizeof(init1)/sizeof(unsigned char);i++) {
		if(lcd_cmdwrite(fd, init1[i])!=2){
			printf("internal error1\n");
			exit(1);
		}
		usleep(50); // wait 50us
	}

	usleep(300000);	// wait 300ms

	for (i=0;i<sizeof(init2)/sizeof(unsigned char);i++) {
		if(lcd_cmdwrite(fd, init2[i])!=2){
			printf("internal error2\n");
			exit(1);
		}
		usleep(50);
	}
	usleep(2000);	// wait 2ms
}

int location(int fd, int y)
{
	int x = 0;
	int cmd=0x80 + y * 0x40 + x;
	return lcd_cmdwrite(fd, cmd);
}

int clear(int fd)
{
	int val = lcd_cmdwrite(fd, 1);
	usleep(1000);	// wait 1ms
	return val;
}

void sig_handler(int signum){
  t=1;
}

void main(){
  int fd;
  void *gpio_baseaddr;
  int val;
  int i2c;

  
  fd = open("/dev/mem",O_RDWR);
  if(fd==-1){//エラーの場合
	  perror("読み込みエラー");
	  exit(1);
	}
  gpio_baseaddr=mmap(NULL,4096,PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_PHY_BASEADDR);
  int  x=memread(gpio_baseaddr,GPIO_GPLEV0);
  i2c=open("/dev/i2c-1",O_RDWR);
  if(i2c==-1){//エラーの場合
	  perror("エラー");
	  exit(1);
	}
    val=ioctl(i2c,I2C_SLAVE,0x3e);
    if(val==-1){//エラーの場合
	  perror("エラー");
	  exit(1);
	}
    initLCD(i2c);
    val=lcd_datawrite(i2c,"           ");//空白にする
    if(val==-1){//エラーの場合
	  perror("エラー");
	  exit(1);
	}
    while(1){
      int  x=memread(gpio_baseaddr,GPIO_GPLEV0);
      i2c=open("/dev/i2c-1",O_RDWR);
      if(i2c==-1){//エラーの場合
	  perror("エラー");
	  exit(1);
       }
      val=ioctl(i2c,I2C_SLAVE,0x3e);
      initLCD(i2c);
      if(((x>>22)&1)==0){//ボタンが押された場合
	val=lcd_datawrite(i2c,"hello");//ハローと印字
	if(val==-1){//エラーの場合
	  perror("エラー");
	  exit(1);
	}
	while(((x>>22)&1)!=0){
	  int  x=memread(gpio_baseaddr,GPIO_GPLEV0);
	  i2c=open("/dev/i2c-1",O_RDWR);
	  if(i2c==-1){//エラーの場合
	    perror("エラー");
	    exit(1);
	  }
	  val=ioctl(i2c,I2C_SLAVE,0x3e);
	  if(val==-1){//エラーの場合
	    perror("エラー");
	    exit(1);
	  }
	  initLCD(i2c);
	}
	
	if(((x>>22)&1)!=0){//ボタンが離されたら
	  signal(SIGALRM,sig_handler);
	  struct itimerval timval;
	  timval.it_interval.tv_sec=0;
	  timval.it_interval.tv_usec=0;
	  timval.it_value.tv_sec=1;
	  timval.it_value.tv_usec=0;
	  setitimer(ITIMER_REAL, &timval, NULL);
	  if(t=1){//LCD制御
	    printf("zikkou\n");
	    val=lcd_datawrite(i2c,"           ");//空白にする
	    t=0;
	  }
	}
	
      }
  }
  val=close(i2c);
}
