/*include*/

#include<linux/module.h>
#include<linux/init.h>
#include<linux/uaccess.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/proc_fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/string.h>
#include<asm/io.h>
#include<linux/string.h>
#include<linux/timer.h>

/*-------------------------------------------------------------------------*/
/*Descript SPI peripheral setup*/

/* Use Only SPI0
 * | SPI Function |  Header  | Boardcom pin name | Boardcom pin function |
 * |     MOSI     |    19    |       GPIO10      |       SPI0_MOSI       |
 * |     MISO     |    21    |       GPIO09      |       SPI0_MISO       |
 * |     SCLK     |    23    |       GPIO11      |       SPI0_SCLK       |
 * |     CE0      |    24    |       GPIO08      |       SPI0_CE0_N      |
 * |     CE1      |    26    |       GPIO07      |       SPI0_CE1_N      |
 */
 
// CE Pin will be control by software 
 
/*to control LCD Nokia 5110, these part are the extra*/

/* SPI0
 * | GPIO25 | DC    |
 * | GPIO24 | RST   |
 * | GPIO23 | LIGHT |
 */

/*Define GPIO pin*/

#define SPI_CS_PIN 8
#define SPI_MISO_PIN 9
#define SPI_MOSI_PIN 10
#define SPI_SCK_PIN 11

#define LCD_DC_PIN 25
#define LCD_RST_PIN 24
#define LCD_LIGHT_PIN 23

/*-------------------------------------------------------------------------*/
/*Basic*/

#define _BV(pos) (1UL << (pos))

/*-------------------------------------------------------------------------*/
/*GPIO Macro*/

#define BCM_2835_GPIO_BASE_ADDRESS ((uint32_t)0x3f200000)

// select function register
#define GPIO_GPFSEL0_OFFSET ((uint32_t)0x00)
#define GPIO_GPFSEL1_OFFSET ((uint32_t)0x04)
#define GPIO_GPFSEL2_OFFSET ((uint32_t)0x08)

// gpio set register
#define GPIO_GPSET0_OFFSET ((uint32_t)0x1C)
// gpio clear register
#define GPIO_GPCLR0_OFFSET ((uint32_t)0x28)
// gpio pull up/down 
#define GPIO_GPPUD_OFFSET ((uint32_t)0x94)
#define GPIO_GPPUDCLK0_OFFSET ((uint32_t)0x98)
#define GPIO_GPPUDCLK1_OFFSET ((uint32_t)0x9C)

// GPIO Select function register
#define GPIO_FSELX_INPUT_MASK ((uint32_t)0x0)
#define GPIO_FSELX_OUTPUT_MASK ((uint32_t)0x1)
#define GPIO_FSELX_AF0_MASK ((uint32_t)0x4)
#define GPIO_FSELX_AF1_MASK ((uint32_t)0x5)
#define GPIO_FSELX_AF2_MASK ((uint32_t)0x6)
#define GPIO_FSELX_AF3_MASK ((uint32_t)0x7)
#define GPIO_FSELX_AF4_MASK ((uint32_t)0x3)
#define GPIO_FSELX_AF5_MASK ((uint32_t)0x2)

/*-------------------------------------------------------------------------*/
/*SPI Macro*/

#define BCM_2835_SPI_BASE_ADDRESS 0x3f204000

#define SPI_CS_OFFSET ((uint32_t)0x00)
#define SPI_FIFO_OFFSET ((uint32_t)0x04)
#define SPI_CLK_OFFSET ((uint32_t)0x08)
#define SPI_DLEN_OFFSET ((uint32_t)0x0C)
// CS
#define SPI_CS_CSPOL2_MASK _BV(23)
#define SPI_CS_CSPOL1_MASK _BV(22)
#define SPI_CS_CSPOL0_MASK _BV(21)

#define SPI_CS_RXF_FLAG_MASK _BV(20)
#define SPI_CS_RXR_FLAG_MASK _BV(19)
#define SPI_CS_TXD_FLAG_MASK _BV(18)
#define SPI_CS_RXD_FLAG_MASK _BV(17)
#define SPI_CS_DONE_FLAG_MASK _BV(16)

#define SPI_CS_INTR _BV(10)
#define SPI_CS_INTD _BV(9)

#define SPI_CS_DMAEN _BV(8)

#define SPI_CS_TA _BV(7)

#define SPI_CS_CSPOL _BV(6)

#define SPI_CS_CLEAR_NONE ((uint32_t)0x00000000)
#define SPI_CS_CLEAR_TX_FIFO ((uint32_t)0x00000010)
#define SPI_CS_CLEAR_RX_FIFO ((uint32_t)0x00000020)

#define SPI_CS_CPOL_HIGH _BV(3)
#define SPI_CS_CPOL_LOW  ((uint32_t)0x00000000)

#define SPI_CS_CPHA_HIGH _BV(2)
#define SPI_CS_CPHA_LOW  ((uint32_t)0x00000000)

#define SPI_CS_CS0_ ((uint32_t)0x00000000)
#define SPI_CS_CS1_ ((uint32_t)0x00000001)
#define SPI_CS_CS2_ ((uint32_t)0x00000002)

// clock divide (core freq = 250MHz)
#define SPI_CLK_DIV_1 _BV(0)
#define SPI_CLK_DIV_2 _BV(1)
#define SPI_CLK_DIV_4 _BV(2)
#define SPI_CLK_DIV_8 _BV(3)
#define SPI_CLK_DIV_16 _BV(4)
#define SPI_CLK_DIV_32 _BV(5)
#define SPI_CLK_DIV_64 _BV(6)
#define SPI_CLK_DIV_128 _BV(7)
#define SPI_CLK_DIV_256 _BV(8)
#define SPI_CLK_DIV_512 _BV(9)
#define SPI_CLK_DIV_1024 _BV(10)
#define SPI_CLK_DIV_2048 _BV(11)
#define SPI_CLK_DIV_4096 _BV(12)
#define SPI_CLK_DIV_8192 _BV(13)
#define SPI_CLK_DIV_16384 _BV(14)
#define SPI_CLK_DIV_32768 _BV(15)
#define SPI_CLK_DIV_65536 _BV(16)

/*-------------------------------------------------------------------------*/

/*Pointer to base register*/
static volatile void __iomem *gpio_base_register = NULL;
static volatile void __iomem *spi_base_register = NULL;

/*-------------------------------------------------------------------------*/
/*Use Kernel timer to create a simple delay*/

static struct timer_list my_timer;

//Timer Callback function. This will be called when timer expires
void timer_callback(struct timer_list * data)
{}
void delay_us(uint16_t us)
{
  mod_timer(&my_timer, jiffies + usecs_to_jiffies(us));
}

/*-------------------------------------------------------------------------*/
// gpio function

void gpio_pin_set_func(uint8_t pin, uint32_t sel_mask, uint32_t gpio_sel_offset)
{
	uint32_t temp = readl(gpio_base_register + gpio_sel_offset);
	temp |= ((uint32_t)(sel_mask << (3*(pin%10))));
	writel(temp,gpio_base_register + gpio_sel_offset);
}

static void gpio_pin_set(uint8_t pin, uint32_t gpio_set_offset){
	uint32_t temp = ((uint32_t)(1UL << (pin%31)));
	writel(temp,gpio_base_register + gpio_set_offset);
}

static void gpio_pin_clear(uint32_t pin,uint32_t gpio_clr_offset)
{
	uint32_t temp = ((uint32_t)(1UL << (pin%31)));
	writel(temp,gpio_base_register + gpio_clr_offset);
}

void gpio_pin_enable(uint32_t pupd, uint32_t pin, uint32_t gpio_pupd_clock_enable_offset)
{
	uint32_t temp_pupd_enable_clock = readl(gpio_base_register + gpio_pupd_clock_enable_offset);

	writel(pupd,gpio_base_register + GPIO_GPPUD_OFFSET);

	delay_us(2);

	temp_pupd_enable_clock |= ((uint32_t)(pin%31));
	writel(temp_pupd_enable_clock,gpio_base_register + gpio_pupd_clock_enable_offset);

	delay_us(2);

	writel(pupd,gpio_base_register + GPIO_GPPUD_OFFSET);

	temp_pupd_enable_clock &= ~((uint32_t)(pin%31));
	writel(temp_pupd_enable_clock,gpio_base_register + gpio_pupd_clock_enable_offset);
}

static void gpio_reg0_set(uint8_t pin)
{
  gpio_pin_set(pin,GPIO_GPSET0_OFFSET);
}

static void gpio_reg0_clear(uint8_t pin)
{
  gpio_pin_clear(pin,GPIO_GPCLR0_OFFSET);
}

static void gpio_reg0_write(uint8_t pin,uint8_t cmd)
{
	switch(cmd)
	{
	case 0: gpio_reg0_clear(pin); break;
	case 1: gpio_reg0_set(pin); break;
	}
}

/*-------------------------------------------------------------------------*/
// SPI function

static void spi_init(uint8_t cs_pin){
	// clear old select
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL0_OFFSET); /// pin 7,8,9 
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL1_OFFSET); /// pin 10,11
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL2_OFFSET); /// pin 23, 24, 25

	// change gpio select to AF 0
	gpio_pin_set_func(cs_pin,GPIO_FSELX_OUTPUT_MASK,GPIO_GPFSEL0_OFFSET);
	gpio_pin_set_func(SPI_MISO_PIN,GPIO_FSELX_AF0_MASK,GPIO_GPFSEL0_OFFSET);
	gpio_pin_set_func(SPI_MOSI_PIN,GPIO_FSELX_AF0_MASK,GPIO_GPFSEL1_OFFSET);
	gpio_pin_set_func(SPI_SCK_PIN,GPIO_FSELX_AF0_MASK,GPIO_GPFSEL1_OFFSET);

	// change gpio select for dc,rst, light pin to GPIO_OUT_PP
	gpio_pin_set_func(LCD_DC_PIN,GPIO_FSELX_OUTPUT_MASK,GPIO_GPFSEL2_OFFSET);
	gpio_pin_set_func(LCD_LIGHT_PIN,GPIO_FSELX_OUTPUT_MASK,GPIO_GPFSEL2_OFFSET);
	gpio_pin_set_func(LCD_RST_PIN,GPIO_FSELX_OUTPUT_MASK,GPIO_GPFSEL2_OFFSET);

	// config pupd for all pin 
	gpio_pin_enable(0,cs_pin,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,SPI_MISO_PIN,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,SPI_MOSI_PIN,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,SPI_SCK_PIN,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,LCD_DC_PIN,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,LCD_LIGHT_PIN,GPIO_GPPUDCLK0_OFFSET);
	gpio_pin_enable(0,LCD_RST_PIN,GPIO_GPPUDCLK0_OFFSET);

	/*-------------------------------------------------*/
	// config frequency div
	writel(SPI_CLK_DIV_4,spi_base_register + SPI_CLK_OFFSET);

	// config cs register
	uint32_t temp_cs = readl(spi_base_register + SPI_CS_OFFSET);

	temp_cs = SPI_CS_CPOL_LOW \
		| SPI_CS_CPHA_LOW \
		| SPI_CS_CLEAR_TX_FIFO \
		| SPI_CS_CLEAR_RX_FIFO; 
		
	writel(temp_cs,spi_base_register + SPI_CS_OFFSET);

	gpio_reg0_write(LCD_RST_PIN,0); /// reset
	gpio_reg0_write(LCD_RST_PIN,1); /// keep rst at high
  
}

static void spi_deinit(uint8_t cs_pin)
{
	writel(0x00000000,spi_base_register + SPI_CLK_OFFSET); /// reset cdiv register
	writel(0x00000000,spi_base_register + SPI_CS_OFFSET); /// reset spi cs register
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL2_OFFSET); /// pin 23, 24, 25

	gpio_reg0_write(SPI_CS_PIN,0); /// reset
	gpio_reg0_write(SPI_MISO_PIN,0); /// reset
	gpio_reg0_write(SPI_MOSI_PIN,0); /// reset
	gpio_reg0_write(SPI_SCK_PIN,0); /// reset

	gpio_reg0_write(LCD_LIGHT_PIN,0);
	gpio_reg0_write(LCD_DC_PIN,0);

	gpio_reg0_write(LCD_RST_PIN,0); /// reset
	gpio_reg0_write(LCD_RST_PIN,1); /// keep rst at high

	writel(0x00000000,gpio_base_register + GPIO_GPFSEL0_OFFSET); /// pin 7,8,9 
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL1_OFFSET); /// pin 10,11
	writel(0x00000000,gpio_base_register + GPIO_GPFSEL2_OFFSET); /// pin 23, 24, 25
}

static inline uint8_t spi_byte_transfer(uint8_t data)
{
	while(!(readl(spi_base_register + SPI_CS_OFFSET) & SPI_CS_TXD_FLAG_MASK)){};
		writeb(data,spi_base_register + SPI_FIFO_OFFSET);
	while(!(readl(spi_base_register + SPI_CS_OFFSET) & SPI_CS_RXD_FLAG_MASK)){};
		uint8_t dump = readb(spi_base_register + SPI_FIFO_OFFSET);
	return dump;
}

static void spi_transfer(uint8_t cs_pin, uint8_t cmd, char *buffer, uint32_t size) {
	// config data length
	writel(1,spi_base_register + SPI_DLEN_OFFSET);
	// config cs register
	uint32_t temp_cs = readl(spi_base_register + SPI_CS_OFFSET);
	temp_cs |= SPI_CS_TA;
	writel(temp_cs,spi_base_register + SPI_CS_OFFSET);

	gpio_reg0_write(cs_pin,0); // enable slave
	uint8_t nop_data = 0;
	switch(cmd){
		case 0:	//CMD LIGHT
		{ 
			gpio_reg0_write(LCD_DC_PIN,0);
			gpio_reg0_write(LCD_LIGHT_PIN,*(buffer));
			nop_data = 1;
		} break;
		
		case 1:	//CMD RESET
		{
			gpio_reg0_write(LCD_DC_PIN,0);
			gpio_reg0_write(LCD_RST_PIN,0);
			delay_us(50*1000);
			gpio_reg0_write(LCD_RST_PIN,1);
			delay_us(50*1000);
			nop_data = 1;
		} break;
		
		case 2:	//CMD COMMAND
		{
			gpio_reg0_write(LCD_DC_PIN,0); /// switch to control command by reset DC pin
		} break;
		
		case 3: //CMD DATA
		{
			gpio_reg0_write(LCD_DC_PIN,1); /// switch to data command by set DC pin
		} break;
	}

	if (nop_data) spi_byte_transfer(0x00);
	else
	{
		for (uint32_t i = 0; i < size; ++i )
		{
			spi_byte_transfer(*(buffer+i));
		}
	}
	gpio_reg0_write(cs_pin,1);
	// while(!(readl(spi_base_register + SPI_CS_OFFSET) & SPI_CS_DONE_FLAG_MASK))
	// {
	// 	while(readl(spi_base_register + SPI_CS_OFFSET) & SPI_CS_RXD_FLAG_MASK)
	// 	{
	// 		uint32_t r = readl(spi_base_register + SPI_FIFO_OFFSET);
	// 		printk("Left Over: %d\n", r);
	// 	}
	// }

	// clear TA flag, end transfer
	temp_cs = readl(spi_base_register + SPI_CS_OFFSET);
	temp_cs &= ~ SPI_CS_TA;
	writel(temp_cs,spi_base_register + SPI_CS_OFFSET);
}

/*------------------------------------------------------------------------------------------*/
#define DRIVER_NAME "my-spi-driver"
#define DRIVER_CLASS "MyModuleClass"

static dev_t my_device_nb;
static struct class *my_class;
static struct cdev my_device;
 
static int driver_open(struct inode *device_file, struct file *instance){
  printk("device open was called\n");
  return 0;
}

static int driver_close(struct inode *device_file, struct file *instance){
  printk("device close was called\n");
  return 0;
}

/*------------------------------------------------------------------------------------------*/

#define MAX_MSG_LEN 256*1024
static char mesg[MAX_MSG_LEN];

static ssize_t driver_read(struct file *File, char *user_buffer, size_t size, loff_t *off_s)
{
	//
	return size;
}

static ssize_t driver_write(struct file *File, const char *user_buffer, size_t size, loff_t *off_s)
{
	if(size > MAX_MSG_LEN) size = MAX_MSG_LEN;

	if(size == 0) return -ENOMEM;

	if(copy_from_user(mesg, user_buffer, size))
		return -EFAULT;

	spi_transfer(SPI_CS_PIN, mesg[0], mesg+1, size-1);

	*off_s += size;
	return size;
}

/*------------------------------------------------------------------------------------------*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

static int my_driver_init(void){
	printk("Hello device driver\n");
	
	if(alloc_chrdev_region(&my_device_nb, 0, 1, DRIVER_NAME)){
		printk("Device could not be allocated \n");
		return -1;
	}
	long major = (my_device_nb >> 20);
	long minor = (my_device_nb & 0xfffff);
	printk("my-driver, Major = %ld, Minor = %ld \n",major,minor);
	
	if((my_class = class_create(DRIVER_CLASS)) == NULL){
		printk("device class can not be create\n");
		unregister_chrdev_region(my_device_nb,1);
		return -2;
	}
	
	if(device_create(my_class,NULL,my_device_nb,NULL,DRIVER_NAME) == NULL){
		printk("can not create device file\n");
		unregister_chrdev_region(my_device_nb,1);
		class_destroy(my_class);
		return -3;
	}
	
	cdev_init(&my_device,&fops);
	if(cdev_add(&my_device,my_device_nb,1) == 1){
		printk("registering device fail\n");
		device_destroy(my_class,my_device_nb);
	}
	
	timer_setup(&my_timer,timer_callback, 0);
	
	gpio_base_register = (int*)ioremap(BCM_2835_GPIO_BASE_ADDRESS,PAGE_SIZE);
	if(gpio_base_register == NULL)printk("failed to map gpio\n");
	
	spi_base_register = (int*)ioremap(BCM_2835_SPI_BASE_ADDRESS,PAGE_SIZE);
	if(spi_base_register == NULL)printk("failed to map spi\n");
	
	spi_init(SPI_CS_PIN);
	
	printk(KERN_EMERG"DEVICE DRIVER INIT DONE\n");
	 
	return 0;
}

static void my_driver_exit(void){
	printk("goodbye device driver\n");
	
	spi_deinit(SPI_CS_PIN);
	
	del_timer(&my_timer);
	cdev_del(&my_device);
	device_destroy(my_class,my_device_nb);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nb,1);
	
	iounmap(gpio_base_register);
	iounmap(spi_base_register);
	
	printk(KERN_EMERG"DEVICE DRIVER EXIT DONE\n");
}


/*------------------------------------------------------------------------------------------*/

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TTCS03");
MODULE_VERSION("1.0");
