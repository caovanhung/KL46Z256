/*
                    Chuong trinh dieu khien LED_RED bang SW1,SW2
                    Mode 1: Nhan SW1 de dao trang thai LED_RED
                    Mode 2: Nhan SW1 de tang do sang cua LED tu 0% len 25%,50%,75%,100% roi ve 0%
                    Su dung SW2 de chuyen che do giua cac mode
*/

#define SIM_SCGC5_PORTA_MASK    (0x200U)
#define SIM_SCGC5_PORTB_MASK    (0x400U)
#define SIM_SCGC5_PORTC_MASK    (0x800U)
#define SIM_SCGC5_PORTD_MASK    (0x1000U)
#define SIM_SCGC5_PORTE_MASK    (0x2000U)

#define RED_LED_PIN          (1 << 29)
#define SW1_PIN              (0 << 3)
#define SW2_PIN              (0 << 12)
#define DELAY_CNT            (10000)

//define status of LED
#define LED_RED_ON           (*FGPIOE_PCOR |= RED_LED_PIN)
#define LED_RED_OFF          (*FGPIOE_PSOR |= RED_LED_PIN)
#define INVERSION_STATE_LED  (*FGPIOE_PTOR |= RED_LED_PIN)

#define SW1                  (*FGPIOC_PDIR & 0x08)
#define SW2                  (*FGPIOC_PDIR & 0x1000)

//define status of button
#define SW_PRESS 0
#define SW_DROP  1

volatile unsigned int * SIM_SCGC5_ADRESS = (volatile unsigned int *)0x40048038u;

volatile unsigned int * PORTE_PCR29_ADRESS   = (volatile unsigned int *)0x4004D074u;
volatile unsigned int * FGPIOE_PSOR          = (volatile unsigned int *)0xF8000104u;
volatile unsigned int * FGPIOE_PCOR          = (volatile unsigned int *)0xF8000108u;
volatile unsigned int * FGPIOE_PTOR          = (volatile unsigned int *)0xF800010Cu;
volatile unsigned int * FGPIOE_PDDR          = (volatile unsigned int *)0xF8000114u;

// address of PORT_PCR3, get in paper 190 datasheet
volatile unsigned int * PORTC_PCR3_ADRESS    = (volatile unsigned int *)0x4004B00Cu;
volatile unsigned int * PORTC_PCR12_ADRESS   = (volatile unsigned int *)0x4004B030u;

//GPIO can be accessed directly at base adress 0xF800_0000.And it can also be accessed by the core and DMA masters at 0x400F_F000
volatile unsigned int * FGPIOC_PDIR          = (volatile unsigned int *)0xF8000090u;//port data input register
volatile unsigned int * FGPIOC_PDDR          = (volatile unsigned int *)0xF8000094u;//port data direction register

void init();
void delay(unsigned long t);
void PWM(float percent, unsigned long time);

int main(void)
{
    init();
    unsigned char count_sw = 0;
    unsigned char percent_of_PWM = 0;
    unsigned char mode = 1,status_SW1 = SW_DROP,status_SW2 = SW_DROP;

    while(1)
    {
        if(mode == 2)
        {
            if(percent_of_PWM == 0)
            {
                LED_RED_OFF;
            }
            else
            {
                PWM((float)percent_of_PWM,DELAY_CNT);
            }
            //Selec duty PWM of LED
            if(SW1 == SW_PRESS)
            {
                if(status_SW1 == SW_DROP)
                {
                    status_SW1 = SW_PRESS;
                    count_sw++;
                    if(count_sw == 1)
                    {
                        (percent_of_PWM) = 0;
                    }
                    else if(count_sw == 2)
                    {
                        (percent_of_PWM) = 25;
                    }
                    else if(count_sw == 3)
                    {
                        (percent_of_PWM) = 50;
                    }
                    else if(count_sw == 4)
                    {
                        (percent_of_PWM) = 75;
                    }
                    else
                    {
                        (percent_of_PWM) = 100;
                    }
                    if(count_sw == 5)
                    {
                        count_sw = 0;
                    }
                }
                //while(SW1 == SW_PRESS);
            }
            else
            {
                status_SW1 = SW_DROP;
            }
        }
        else
        {
            if(SW1 == SW_PRESS)
            {
                if(status_SW1 == SW_DROP)
                {
                    status_SW1 = SW_PRESS;
                    INVERSION_STATE_LED;
                }
            }
            else
            {
                status_SW1 = SW_DROP;
            }
        }
        //Mode change
        if(SW2 == SW_PRESS)
        {
            if(status_SW2 == SW_DROP)
            {
                status_SW2 = SW_PRESS;
                if(mode == 1)
                {
                    mode = 2;
                }
                else
                {
                    mode = 1;
                }
            }
        }
        else
        {
            status_SW2 = SW_DROP;
        }
    }
}

/*
Object : Setup register for LED,SW1,SW2
Input  : Nope
Output : Nope
*/
void init()
{
    *SIM_SCGC5_ADRESS |= (SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTC_MASK);// Enable clock for PORTE
    
    *PORTE_PCR29_ADRESS = 0x100; // Set the PTE29 pin multiplexer to GPIO mode
    *FGPIOE_PDDR |= RED_LED_PIN;//Set the pin's direction to output

    *PORTC_PCR3_ADRESS = 0x103;
    *PORTC_PCR12_ADRESS = 0x103;
    *FGPIOC_PDDR |= SW1_PIN;
    *FGPIOC_PDDR |= SW2_PIN;
    LED_RED_OFF;
}

/*
Object : delay using nop with time delay = 1(T). f ~ 20mhz
Input  : t : time delay
Output : Nope
*/
void delay(unsigned long t)
{
    unsigned long i;
    for (i = 0; i < t; i++)
    {
        __asm("nop");
    }
}

/*
Object : 
Input: percent : phan tram bam xung
        time : thoi gian bam xung
Output : Nope
*/
void PWM(float percent, unsigned long time)
{
    float t=0;
    t = (float)time*percent/100;
    LED_RED_ON;
    delay((unsigned long)t);
    LED_RED_OFF;
    delay((unsigned long)(time-t));
}
