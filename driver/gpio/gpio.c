#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>         //required for GPIO functions
#include <linux/interrupt.h>    // for IRQ code
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 6");
MODULE_DESCRIPTION("GPIO driver for Switch and Led");
MODULE_VERSION("0.1");
 
static unsigned int led0 = 61;
static unsigned int led1 = 62;
static unsigned int led2 = 63; 
static unsigned int led3 = 64; 
static unsigned int led4 = 65; 
static unsigned int led5 = 66; 


static unsigned int sw0 = 69;
static unsigned int sw1 = 70; 
static unsigned int sw2 = 71; 
static unsigned int sw3 = 72;
static unsigned int sw4 = 73;
static unsigned int sw5 = 74; 

static unsigned int irq0;
static unsigned int irq1;
static unsigned int irq2;
static unsigned int irq3;
static unsigned int irq4;
static unsigned int irq5;

static bool ledOn = 0;

//Alternative
//const int Range = 6
//static unsigned int led[] = {61, 62, 63, 64, 65, 66};
//static unsigned int sw[] = {69, 70, 71, 72, 73, 74};
 
// function prototype for custom irq handler
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);
 
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
        
        irq0 = gpio_to_irq(sw0);
 
	
	//Led1 and Switch1
  	gpio_request(led1, "sysfs");         // request LED on GPIO62
        gpio_direction_output(led1, ledOn);  
        gpio_export(led1, false);           

        gpio_request(sw1, "sysfs");          
        gpio_direction_input(sw1);           
        gpio_set_debounce(sw1, 200);        
        gpio_export(sw1, false);   
        
        irq1 = gpio_to_irq(sw1);          


	//Led2 and Switch2
  	gpio_request(led2, "sysfs");         // request LED on GPIO63
        gpio_direction_output(led2, ledOn);  
        gpio_export(led2, false); 
           
        gpio_request(sw2, "sysfs");          
        gpio_direction_input(sw2);          
        gpio_set_debounce(sw2, 200);         
        gpio_export(sw2, false);  
        
        irq2 = gpio_to_irq(sw2);

	//Led3 and Switch3
	gpio_request(led3, "sysfs");         // request LED on GPIO64
        gpio_direction_output(led3, ledOn);  
        gpio_export(led3, false); 
           
        gpio_request(sw3, "sysfs");          
        gpio_direction_input(sw3);          
        gpio_set_debounce(sw3, 200);         
        gpio_export(sw3, false);
        
        irq3 = gpio_to_irq(sw3);


	//Led4 and Switch4
	gpio_request(led4, "sysfs");         // request LED on GPIO65
        gpio_direction_output(led4, ledOn);  
        gpio_export(led4, false); 
           
        gpio_request(sw4, "sysfs");          
        gpio_direction_input(sw4);          
        gpio_set_debounce(sw4, 200);         
        gpio_export(sw4, false);
        
        irq4 = gpio_to_irq(sw4);


	//Led5 and Switch5
	gpio_request(led5, "sysfs");         // request LED on GPIO66
        gpio_direction_output(led5, ledOn);  
        gpio_export(led5, false); 
           
        gpio_request(sw5, "sysfs");          
        gpio_direction_input(sw5);          
        gpio_set_debounce(sw5, 200);         
        gpio_export(sw5, false);
        
        irq5 = gpio_to_irq(sw5);
	return result;

}           
 
 //If using Alternative
/*
gpio_request(led, "sysfs");         // request LED on GPIO
        gpio_direction_output(led, ledOn);  
        gpio_export(led, false);            // export will make the gpio pin to appear in /sys/class/gpio
        gpio_request(sw, "sysfs");          // set up gpio switch
        gpio_direction_input(sw);           // set switch as input
        gpio_set_debounce(sw, 200);         // debounce with delay of 200ms
        gpio_export(sw, false);             // make it appear in /sys/class/gpio
 
        //printk(KERN_INFO "GPIO_LED: switch currently in state %d\n", gpio_get_value(gpioSW));
 
       GPIO numbers and IRQ numbers are not the same and this function will map them
        irqNumber = gpio_to_irq(sw);
        printk(KERN_INFO "GPIO_LED: switch %d is mapped to IRQ %d\n", gpioSW, irqNumber);
 
        request an interrupt line
        result = request_irq(irqNumber,                             // interrupt number requested
                        (irq_handler_t) gpio_irq_handler,           // pointer to the handler function
                        IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, // trigger both on rising and falling signal edge
                        "ledswitch_gpio_handler",                   // used in /proc/interrupts to identify the owner
                        NULL);                                      // *dev_id for shared interrupt lines
        //printk(KERN_INFO "GPIO_LED: the interrupt request result is: %d\n", result);
        return result;
}

*/

 
static void __exit gpio_exit(void) {
      
	gpio_unexport(led0);
        gpio_unexport(led1);
	gpio_unexport(led2);
	gpio_unexport(led3);
	gpio_unexport(led4);
	gpio_unexport(led5);
	gpio_free(led0);        
	gpio_free(led1);
	gpio_free(led2);
	gpio_free(led3);        
	gpio_free(led4);
	gpio_free(led5);

	gpio_unexport(sw0);
        gpio_unexport(sw1);
	gpio_unexport(sw2);
	gpio_unexport(sw3);
    gpio_unexport(sw4);
	gpio_unexport(sw5);
	gpio_free(sw0);        
	gpio_free(sw1);
	gpio_free(sw2);
	gpio_free(sw3);        
	gpio_free(sw4);
	gpio_free(sw5);
	
        printk(KERN_INFO "GPIO: Module unloaded\n");
}

//If using Alternative
/*
static void __exit gpio_exit(void) {
        printk(KERN_INFO "GPIO_LED: The switch state is currently %d\n", gpio_get_value(gpioSW));
        gpio_set_value(led, 0);
        gpio_unexport(led);
        free_irq(irqNumber, NULL);
        gpio_unexport(sw);
        gpio_free(led);
        gpio_free(sw);
        printk(KERN_INFO "GPIO_LED: Module unloaded\n");
}
static irq_handler_t gpio_irq_handler(unsigned int irq, void *dev_id, struct pt_regs *regs) {
        ledOn = gpio_get_value(sw); // invert led state
        gpio_set_value(led, ledOn);
        return (irq_handler_t) IRQ_HANDLED; // announce that the IRQ has been handled correctly
}*/
  
module_init(gpio_init);
module_exit(gpio_exit);






