/// 3-Channel Hardware PWM & USB I/O

/// this define is necessary with my gcc/libc combo for some reason
#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <math.h>

/// LED gamma & max color value
#define GAMMA	2.2
#define	MAXPWM	0xFFFF

/// PWM counter top value
#define CNT_TOP	0xFFFF

/// RGB output pins
#define RED PC6
#define GREEN PC5
#define BLUE PB7

/// color changing speed delay
#define DELAY 10
#define R_SPEED 2
#define G_SPEED 3
#define B_SPEED 5

/// function protos
void init(void);
void setcolor(uint16_t,uint16_t,uint16_t);
int lowvoltage(void);
void helloworld(void);
void mood(void);
void USB_sendstring(char *);
void USB_sendstring_P(PGM_P);
void USB_sendbyte(uint8_t);

#include "USBtoSerial.h"

/** Circular buffer to hold data from the host before it is sent to the device via the serial port. */
static RingBuffer_t USBtoUSART_Buffer;

/** Underlying data buffer for \ref USBtoUSART_Buffer, where the stored bytes are located. */
static uint8_t      USBtoUSART_Buffer_Data[128];

/** Circular buffer to hold data from the serial port before it is sent to the host. */
static RingBuffer_t USARTtoUSB_Buffer;

/** Underlying data buffer for \ref USARTtoUSB_Buffer, where the stored bytes are located. */
static uint8_t      USARTtoUSB_Buffer_Data[128];

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber         = 0,
				.DataINEndpoint                 =
					{
						.Address                = CDC_TX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.DataOUTEndpoint                =
					{
						.Address                = CDC_RX_EPADDR,
						.Size                   = CDC_TXRX_EPSIZE,
						.Banks                  = 1,
					},
				.NotificationEndpoint           =
					{
						.Address                = CDC_NOTIFICATION_EPADDR,
						.Size                   = CDC_NOTIFICATION_EPSIZE,
						.Banks                  = 1,
					},
			},
	};


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
	/// moodlight init
	init();

	/// LUFA init
	SetupHardware();
	RingBuffer_InitBuffer(&USBtoUSART_Buffer, USBtoUSART_Buffer_Data, sizeof(USBtoUSART_Buffer_Data));
	RingBuffer_InitBuffer(&USARTtoUSB_Buffer, USARTtoUSB_Buffer_Data, sizeof(USARTtoUSB_Buffer_Data));
	sei();
	
	/// greetings to the user via USB
	helloworld();
	
	/// main loop
	for (;;)
	{
		mood();
		/* Only try to read in bytes from the CDC interface if the transmit buffer is not full */
		if (!(RingBuffer_IsFull(&USBtoUSART_Buffer)))
		{
			int16_t ReceivedByte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);

			/* Read bytes from the USB OUT endpoint into the USART transmit buffer */
			if (!(ReceivedByte < 0))
			  RingBuffer_Insert(&USBtoUSART_Buffer, ReceivedByte);
		}

		/* Check if the UART receive buffer flush timer has expired or the buffer is nearly full */
		uint16_t BufferCount = RingBuffer_GetCount(&USARTtoUSB_Buffer);
		if ((TIFR0 & (1 << TOV0)) || (BufferCount > (uint8_t)(sizeof(USARTtoUSB_Buffer_Data) * .75)))
		{
			/* Clear flush timer expiry flag */
			TIFR0 |= (1 << TOV0);

			/* Read bytes from the USART receive buffer into the USB IN endpoint */
			while (BufferCount--)
			{
				/* Try to send the next byte of data to the host, abort if there is an error without dequeuing */
				if (CDC_Device_SendByte(&VirtualSerial_CDC_Interface,
				                        RingBuffer_Peek(&USARTtoUSB_Buffer)) != ENDPOINT_READYWAIT_NoError)
				{
					break;
				}

				/* Dequeue the already sent byte from the buffer now we have confirmed that no transmission error occurred */
				RingBuffer_Remove(&USARTtoUSB_Buffer);
			}
		}
		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}


/************************/
/** moodlight functions */
/************************/

void USB_sendstring(char *s)	/// send string from RAM
{
	while (*s) RingBuffer_Insert(&USARTtoUSB_Buffer, *s++);
}

void USB_sendstring_P(PGM_P s)	/// send string from PROGMEM
{
	uint8_t b;
	while ( b = pgm_read_byte(s++) ) RingBuffer_Insert(&USARTtoUSB_Buffer, b);
}

void USB_sendbyte(uint8_t byte)	/// send byte
{
	RingBuffer_Insert(&USARTtoUSB_Buffer, byte);
}

void helloworld(void)	/// "# Hello World" to host via USB ;-)
{
	static uint8_t num=0;

	USB_sendbyte('0' + num++);
	USB_sendstring_P(PSTR(" Hello World\r\n"));
	num %= 10;
}

void mood(void)
{
	static uint8_t command;
	static uint16_t i=0;
	static uint16_t r=MAXPWM,g=MAXPWM/2,b=MAXPWM/42;
	static uint16_t r_dir=R_SPEED, g_dir=G_SPEED, b_dir=B_SPEED;

	command='m';	// default command: mood
	
	/* Load the next command byte from the USB to USART transmit buffer */
	if (!(RingBuffer_IsEmpty(&USBtoUSART_Buffer)))
		  command=RingBuffer_Remove(&USBtoUSART_Buffer);
	switch (command) {
		case 'r': if (r >=1000) r-=1000; break;
		case 'R': if (r <= MAXPWM-1000) r+=1000; break;
		case 'g': if (g >=1000) g-=1000; break;
		case 'G': if (g <= MAXPWM-1000) g+=1000; break;
		case 'b': if (b >=1000) b-=1000; break;
		case 'B': if (b <= MAXPWM-1000) b+=1000; break;
		case 'h': helloworld();
		default:
			if (++i > DELAY) {
				if (r >= (MAXPWM - R_SPEED)) r_dir = -R_SPEED;
				if (r <= R_SPEED) r_dir = R_SPEED;
				if (g >= (MAXPWM - G_SPEED)) g_dir = -G_SPEED;
				if (g <= G_SPEED) g_dir = G_SPEED;
				if (b >= (MAXPWM - B_SPEED)) b_dir = -B_SPEED;
				if (b <= B_SPEED) b_dir = B_SPEED;
				r += r_dir; g += g_dir; b += b_dir;
				setcolor(r,g,b);
				if (lowvoltage()) {	// red light and stop
					setcolor(MAXPWM,0,0);
					for(;;);
				}
				i=0;
			}
	}
}

/*************************************************/
/** actual setup routines for the moodlight part */
/*************************************************/

void init(void)		// setup stuff
{
	// setup system clock
	cli();			// do not interrupt clock frequency changes
	CLKPR = (1 << CLKPCE);	// enable clock prescaler update
	CLKPR = 0;		// set clock to maximum (= crystal)
	// set alternate pin functions for RGB pins (clear on compare match [10])
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << COM1C1);
	// setup timer1 for Fast PWM mode 14 (TOP=ICR1)
	TCCR1A |= (1 << WGM11);	// WGM bits are split across TCCR1A and TCCR1B
	TCCR1B |= (1 << WGM12) | (1 << WGM13);
	// select clock source and set divider to 1 (244 Hz 16-bit PWM)
	TCCR1B |= (1 << CS10);
	// setup ICR1 for counter TOP value
	ICR1 = CNT_TOP;
	// set direction of rgb ports to output
	DDRC |= (1 << RED);
	DDRC |= (1 << GREEN);
	DDRB |= (1 << BLUE);

	// analog input setup (interrupt on falling edge, bandgap reference)
	ACSR = (1 << ACBG) | (1 << ACIS1);
	// analoc comparator mux - select AIN2, disable digital input
	ACMUX = (1 << CMUX0);
	DIDR1 = (1 << AIN2D);
	// clear pending undervoltage interrupt
	ACSR |= (1 << ACI);
}

void setcolor(uint16_t r,uint16_t g,uint16_t b)	// set a new RGB color
{
	OCR1A = round((double) (CNT_TOP) * pow(((double) r / (double) (MAXPWM)) , GAMMA));
	OCR1B = round((double) (CNT_TOP) * pow(((double) g / (double) (MAXPWM)) , GAMMA));
	OCR1C = round((double) (CNT_TOP) * pow(((double) b / (double) (MAXPWM)) , GAMMA));
}

int lowvoltage(void)	// check if voltage ok [0] or low [1]
{
	if ( (ACSR & (1 << ACI)) == 0 ) return 0;	// no brownout occured
	ACSR |= (1 << ACI);	// clear pending undervoltage interrupt
	return 1;
}


/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Hardware Initialization */
	USB_Init();

	/* Start the flush timer so that overflows occur rapidly to push received bytes to the USB interface */
	TCCR0B = (1 << CS02);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

/** ISR to manage the reception of data from the serial port, placing received bytes into a circular buffer
 *  for later transmission to the host.
 */
//ISR(USART1_RX_vect, ISR_BLOCK)
//{
//}

/** Event handler for the CDC Class driver Line Encoding Changed event.
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo)
{
}

