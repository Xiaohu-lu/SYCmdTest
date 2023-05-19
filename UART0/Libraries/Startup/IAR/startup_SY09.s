;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; File name    :	Startup_SY09.s
; Function     :	Startup file for SY09   
; Author       : 	Coins
; Date         :	2015/05/28
; Version      : 	v1.0
; Description  :   
; ModifyRecord :
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Include Header Files
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Local Macro Definition
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Local static Variable Declaration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Global Variable Declaration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Local Functon Declaration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Functon
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	
		MODULE  ?cstartup
		
		;; Forward declaration of sections.
		SECTION CSTACK:DATA:NOROOT(3)
		
		SECTION .intvec:CODE:NOROOT(2)
		
		EXTERN  __iar_program_start
		EXTERN  SystemInit
		PUBLIC  __vector_table
		PUBLIC  __vector_table_0x1c
		PUBLIC  __Vectors
		PUBLIC  __Vectors_End
		PUBLIC  __Vectors_Size
		
		DATA

__vector_table
		DCD		sfe(CSTACK)
		DCD		Reset_Handler
		   		
		DCD		NMI_Handler
		DCD		HardFault_Handler
		DCD		MemManage_Handler
		DCD		BusFault_Handler
		DCD		UsageFault_Handler
__vector_table_0x1c
		DCD		0
		DCD		0
		DCD		0
		DCD		0
		DCD		SVC_Handler
		DCD		DebugMon_Handler
		DCD		0
		DCD		PendSV_Handler
		DCD		SysTick_Handler
		
		; External Interrupts
		DCD		SCM_Handler           	;0 	SCM Handler
		DCD		GPIO_Handler           	;1 	GPIO Handler
		DCD		T0_Handler           	;2 	Timer 0 Handler
		DCD		SEA_Handler           	;3 	Symmetric Encryption/decryption Algorithms Accelerators Group Handler
		DCD		AEA_Handler        		;4 	Asymmetric Encryption/decryption Algorithms Accelerators Group Handler
		DCD		SPI0_Handler            ;5 	SPI handler
		DCD		TRNG_Handler           	;6 	True Random Number Generater Handler
		DCD		USB_Handler           	;7 	USB Handler    
		DCD		T1_Handler           	;8 	Reserved
		DCD		T2_Handler           	;9 	Reserved
		DCD		SSI_Handler           	;10 Reserved
		DCD		UART0_Handler           ;11 Reserved
		DCD		I2C0_Handler        	;12 Reserved
		DCD		SWPS_Handler         	;13 Reserved
		DCD		0          				;14 Reserved
		DCD		ISO7816_Handler         ;15 Reserved
		DCD		SCI_Handler            	;16 Reserved
		DCD		FPA_Handler           	;17 Reserved	
		DCD		VPWM_Handler  			;18 Reserved
		DCD		UART1_Handler          	;19 Reserved
		DCD		HASH_Handler			;20	Reserved
		DCD		EFC_Handler				;21	Reserved
		DCD		0 						;22	Reserved
		DCD		0           			;23 Reserved
		DCD		0           			;24 Reserved
		DCD		0           			;25 Reserved
		DCD		LOCSC_Handler           ;26 Reserved
		DCD		SPI1_Handler          	;27 Reserved
		DCD		DMA_Handler          	;28 Reserved
		DCD		0          				;29 Reserved
		DCD		0          				;30 Reserved
		DCD		0          				;31 Reserved
		   		
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          				;Reserved
		DCD		0          	            ;Reserved
		DCD		0         	            ;Reserved
__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size  EQU   __Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler

        LDR	R0, =__Vectors  	; SCB->VTOR = __Vectors
        MOVS	R1, #0xE000E000		
        STR	R0, [R1, #0xD08]
                                
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
        B SysTick_Handler

       
		PUBWEAK SCM_Handler     
		PUBWEAK GPIO_Handler    
		PUBWEAK T0_Handler          
		PUBWEAK SEA_Handler    
		PUBWEAK AEA_Handler
		PUBWEAK SPI0_Handler         
		PUBWEAK TRNG_Handler    
		PUBWEAK USB_Handler 
		PUBWEAK T1_Handler           	
		PUBWEAK T2_Handler           		
		PUBWEAK SSI_Handler           		
		PUBWEAK UART0_Handler           		
		PUBWEAK I2C0_Handler        			
		PUBWEAK SWPS_Handler         			
		PUBWEAK ISO7816_Handler          		
		PUBWEAK SCI_Handler            		
		PUBWEAK FPA_Handler           			
		PUBWEAK VPWM_Handler  				
		PUBWEAK UART1_Handler          			
		PUBWEAK DMA_Handler					
		PUBWEAK EFC_Handler					
		PUBWEAK LOCSC_Handler           	
		PUBWEAK SPI1_Handler          	 		
		PUBWEAK HASH_Handler  
        SECTION .text:CODE:NOROOT:REORDER(1)
SCM_Handler     
GPIO_Handler    
T0_Handler          
SEA_Handler    
AEA_Handler
SPI0_Handler         
TRNG_Handler    
USB_Handler 
T1_Handler           	
T2_Handler           		
SSI_Handler           		
UART0_Handler           		
I2C0_Handler        			
SWPS_Handler         			
ISO7816_Handler          		
SCI_Handler            		
FPA_Handler           			
VPWM_Handler  				
UART1_Handler          			
DMA_Handler					
EFC_Handler					
LOCSC_Handler           	
SPI1_Handler          	 		
HASH_Handler          		
        B .

        END
