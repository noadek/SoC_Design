#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>         //required for GPIO functions
#include <linux/interrupt.h>    // for IRQ code
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 6");
MODULE_DESCRIPTION("GPIO driver for Switch and Led");
MODULE_VERSION("0.1");
 
static unsigned int sw0 = 992;
static unsigned int sw1 = 993;
static unsigned int sw2 = 994; 
static unsigned int sw3 = 995; 
static unsigned int sw4 = 996; 
static unsigned int sw5 = 997; 


static unsigned int led0 = 960;
static unsigned int led1 = 961;
static unsigned int led2 = 962; 
static unsigned int led3 = 963; 
static unsigned int led4 = 964; 
static unsigned int led5 = 965; 

static bool ledOn = 0;

static void freeLedSw(unsigned int gpioLED, unsigned int gpioSW);
 
static int __init gpio_init(void) {
	int result = 0;

	//Led0 and Switch0 
	gpio_request(led0, "sysfs");         // request LED on GPIO61
	gpio_direction_output(led0, ledOn);  
	gpio_export(led0, false);            // export will make the gpio pin to appear in /sys/class/gpio

	gpio_request(sw0, "sysfs");          // set up gpio switch
	gpio_direction_input(sw0);           // set switch as input
	gpio_set_debounce(sw0, 200);         // debounce with delay of 200ms
	gpio_export(sw0, false);             // make it appear in /sys/class/gpio

	//Led1 and Switch1
	gpio_request(led1, "sysfs");         // request LED on GPIO62
	gpio_direction_output(led1, ledOn);  
	gpio_export(led1, false);           

	gpio_request(sw1, "sysfs");          
	gpio_direction_input(sw1);           
	gpio_set_debounce(sw1, 200);        
	gpio_export(sw1, false);   

	//Led2 and Switch2
	gpio_request(led2, "sysfs");         // request LED on GPIO63
	gpio_direction_output(led2, ledOn);  
	gpio_export(led2, false); 

	gpio_request(sw2, "sysfs");          
	gpio_direction_input(sw2);          
	gpio_set_debounce(sw2, 200);         
	gpio_export(sw2, false);  

	//Led3 and Switch3
	gpio_request(led3, "sysfs");         // request LED on GPIO64
	gpio_direction_output(led3, ledOn);  
	gpio_export(led3, false); 

	gpio_request(sw3, "sysfs");          
	gpio_direction_input(sw3);          
	gpio_set_debounce(sw3, 200);         
	gpio_export(sw3, false);

	//Led4 and Switch4
	gpio_request(led4, "sysfs");         // request LED on GPIO65
	gpio_direction_output(led4, ledOn);  
	gpio_export(led4, false); 

	gpio_request(sw4, "sysfs");          
	gpio_direction_input(sw4);          
	gpio_set_debounce(sw4, 200);         
	gpio_export(sw4, false);

	//Led5 and Switch5
	gpio_request(led5, "sysfs");         // request LED on GPIO66
	gpio_direction_output(led5, ledOn);  
	gpio_export(led5, false); 

	gpio_request(sw5, "sysfs");          
	gpio_direction_input(sw5);          
	gpio_set_debounce(sw5, 200);         
	gpio_export(sw5, false);
	
	return result;
}

static void __exit gpio_exit(void) {
    freeLedSw(led0, sw0);
	freeLedSw(led1, sw1);
	freeLedSw(led2, sw2);
	freeLedSw(led3, sw3);
	freeLedSw(led4, sw4);
	freeLedSw(led5, sw5);

	printk(KERN_INFO "GPIO: Module unloaded\n");
}

void freeLedSw(unsigned int gpioLED, unsigned int gpioSW)
{
	printk(KERN_INFO "GPIO_LED: The switch state is currently %d\n", gpio_get_value(gpioSW));
	gpio_set_value(gpioLED, 0);
	gpio_unexport(gpioLED);
	gpio_unexport(gpioSW);
	
	gpio_free(gpioLED);
	gpio_free(gpioSW);
}

module_init(gpio_init);
module_exit(gpio_exit);






