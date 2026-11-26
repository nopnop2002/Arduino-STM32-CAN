# stm32f103 interrupt example
BxCAN can use three interrupts.   
- Transmission interrupt   
	This occurs upon completion of a transmission or in the event of a transmission error.   
- FIFO0 receive interrupt   
	This occurs upon completion of reception into FIFO0 or in the event of a reception error .   
- FIFO1 receive interrupt   
	This occurs upon completion of reception into FIFO1 or in the event of a reception error .   

# Interrupt handlers
Interrupt handlers are defined with the following names.   
```
.word     CAN1_TX_IRQHandler                /* CAN1 TX                      */
.word     CAN1_RX0_IRQHandler               /* CAN1 RX0                     */
.word     CAN1_RX1_IRQHandler               /* CAN1 RX1                     */
.word     CAN1_SCE_IRQHandler               /* CAN1 SCE                     */

.word     CAN2_TX_IRQHandler                /* CAN2 TX                      */
.word     CAN2_RX0_IRQHandler               /* CAN2 RX0                     */
.word     CAN2_RX1_IRQHandler               /* CAN2 RX1                     */
.word     CAN2_SCE_IRQHandler               /* CAN2 SCE                     */
```

These functions are provided by the core library, but since they are "weak" functions, they can be overridden.   
```
.weak      CAN1_TX_IRQHandler
.weak      CAN1_RX0_IRQHandler
.weak      CAN1_RX1_IRQHandler
.weak      CAN1_SCE_IRQHandler

.weak      CAN2_TX_IRQHandler
.weak      CAN2_RX0_IRQHandler
.weak      CAN2_RX1_IRQHandler
.weak      CAN2_SCE_IRQHandler
```

# Interrupt number
Interrupt number are defined with the following names.   
```
CAN1_TX_IRQn                = 19,     /*!< CAN1 TX Interrupt                                                 */
CAN1_RX0_IRQn               = 20,     /*!< CAN1 RX0 Interrupt                                                */
CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                                */
CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                                */
```

In the STM32F1 series, these interrupts are shared at the hardware level.   
Therefore, it is not possible to use these interrupts simultaneously.   
```
#define USB_HP_CAN1_TX_IRQHandler  USB_HP_IRQHandler
#define CAN1_TX_IRQHandler         USB_HP_IRQHandler
#define CAN1_RX0_IRQHandler        USB_LP_IRQHandler
#define USB_LP_CAN1_RX0_IRQHandler USB_LP_IRQHandler

```


# How to use interrupts

- Configure the IER register within CANInit()   
	The types of interrupts that can be used are described in the "CAN interrupt enable register" section of the reference manual.
	This sample uses the following interrupts.
	```
	CAN1->IER |= CAN_IER_TMEIE;           // Set to Transmit mailbox empty interrupt enable
	CAN1->IER |= CAN_IER_FMPIE0;          // Set to FIFO0 message pending interrupt enable
	CAN1->IER |= CAN_IER_FMPIE1;          // Set to FIFO1 message pending interrupt enable
	```

- Set interrupt priority within Setup()
	```
	NVIC_SetPriority(CAN1_TX_IRQn, 1);
	NVIC_SetPriority(CAN1_RX0_IRQn, 1);
	NVIC_SetPriority(CAN1_RX1_IRQn, 1);
	```

- Enable interrupts within Setup()
	```
	NVIC_EnableIRQ(CAN1_TX_IRQn);
	NVIC_EnableIRQ(CAN1_RX0_IRQn);
	NVIC_EnableIRQ(CAN1_RX1_IRQn);
	```

- Set up a transmit interrupt handler
Determine the type of interrupt and clear the interrupt that occurred.   
```
extern "C" void CAN1_TX_IRQHandler (void)
{
  // Check the transmission completion flag for mailbox 0.
  if (CAN1->TSR & CAN_TSR_RQCP0) {
    // Clear the transmission completion flag for mailbox 0.
    // If this is not done, the interrupt flag will not be cleared, resulting in an infinite loop.
    CAN1->TSR |= CAN_TSR_RQCP0;
  }

  // Check the transmission completion flag for mailbox 1.
  if (CAN1->TSR & CAN_TSR_RQCP1) {
    // Clear the transmission completion flag for mailbox 1.
    // If this is not done, the interrupt flag will not be cleared, resulting in an infinite loop.
    CAN1->TSR |= CAN_TSR_RQCP1;
  }

  // Check the transmission completion flag for mailbox 2.
  if (CAN1->TSR & CAN_TSR_RQCP2) {
    // Clear the transmission completion flag for mailbox 2.
    // If this is not done, the interrupt flag will not be cleared, resulting in an infinite loop.
    CAN1->TSR |= CAN_TSR_RQCP2;
  }
}
```

- Set up a receive interrupt handler for FIFO0
Determine the type of interrupt and clear the interrupt that occurred.   
```
extern "C" void CAN1_RX0_IRQHandler(void)
{
  // Check if there are any unread messages in FIFO0.
  // These bits indicate how many messages are pending in the receive FIFO0.
  // The FIFO can store up to three messages.
  if ((CAN1->RF0R & CAN_RF0R_FMP0) != 0U) {
    // Release the output mailbox of the FIFO.
    // If at least two messages are pending in the FIFO, 
    // the software has to release the output mailbox to access the next message.
    // If this is not done, the interrupt flag will not be cleared, resulting in an infinite loop.
    CAN1->RF0R |= CAN_RF0R_RFOM0;
  }
}
```

- Set up a receive interrupt handler for FIFO1
Determine the type of interrupt and clear the interrupt that occurred.   
```
extern "C" void CAN1_RX1_IRQHandler(void)
{
  // Check if there are any unread messages in FIFO1.
  // These bits indicate how many messages are pending in the receive FIFO1.
  // The FIFO can store up to three messages.
  if ((CAN1->RF1R & CAN_RF1R_FMP1) != 0U) {
    // Release the output mailbox of the FIFO.
    // If at least two messages are pending in the FIFO, 
    // the software has to release the output mailbox to access the next message.
    // If this is not done, the interrupt flag will not be cleared, resulting in an infinite loop.
    CAN1->RF1R |= CAN_RF1R_RFOM1;
  }
}
```

