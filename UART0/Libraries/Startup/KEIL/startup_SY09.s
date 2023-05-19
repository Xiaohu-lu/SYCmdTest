;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; File name    :	Startup_SY09.s
; Function     :	Startup file for SY09   
; Author       : 	Coins
; Date         :	2014/04/25
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

; Define the stack top label
				AREA    STACK_TOP, NOINIT, READWRITE, ALIGN=3
__initial_sp 

; Define the heap bottom label			
				AREA    HEAP_BOTTOM, NOINIT, READWRITE, ALIGN=3
__heap_base

; Define the heap top label
			    AREA    HEAP_TOP, NOINIT, READWRITE, ALIGN=3
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler
				; Interrupt
                DCD     SCM_Handler           	;0 	SCM Handler
                DCD     GPIO_Handler           	;1 	GPIO Handler
                DCD     T0_Handler           	;2 	Timer 0 Handler
				DCD     SEA_Handler           	;3 	Symmetric Encryption/decryption Algorithms Accelerators Group Handler
				DCD     AEA_Handler        		;4 	Asymmetric Encryption/decryption Algorithms Accelerators Group Handler
				DCD     SPI0_Handler            ;5 	SPI handler
				DCD     TRNG_Handler           	;6 	True Random Number Generater Handler
				DCD     USB_Handler           	;7 	USB Handler    
				DCD     T1_Handler           			;8 	Reserved
                DCD     T2_Handler           			;9 	Reserved
                DCD     SSI_Handler           			;10 Reserved
                DCD     UART0_Handler           			;11 Reserved
                DCD     I2C0_Handler        				;12 Reserved
                DCD     SWPS_Handler         				;13 Reserved
                DCD     0          				;14 Reserved
                DCD     ISO7816_Handler          				;15 Reserved
                DCD     SCI_Handler            			;16 Reserved
                DCD     FPA_Handler           			;17 Reserved	
				DCD		VPWM_Handler  						;18 Reserved
				DCD     UART1_Handler          				;19 Reserved
				DCD		HASH_Handler						;20	Reserved
				DCD		EFC_Handler						;21	Reserved
				DCD		0 						;22	Reserved
                DCD     0           			;23 Reserved
                DCD     0           			;24 Reserved
                DCD     0           			;25 Reserved
                DCD     LOCSC_Handler           			;26 Reserved
                DCD     SPI1_Handler          	 			;27 Reserved
                DCD     DMA_Handler          				;28 Reserved
                DCD     0          				;29 Reserved
                DCD     0          				;30 Reserved
                DCD     0          				;31 Reserved
              	
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     0          				;Reserved
				DCD     __heap_base          	;Reserved
				DCD     __heap_limit         	;Reserved

__Vectors_End

__Vectors_Size         EQU     __Vectors_End - __Vectors
                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  __main
                IMPORT  SystemInit
				IMPORT  SYS_GenSramKey

				;LDR		R0, =__Vectors  	; SCB->VTOR = __Vectors
				;MOVS	R1, #0xE000E000		
				;STR		R0, [R1, #0xD08]

				; ro write SRAM encrypt key
				LDR     R0, =SYS_GenSramKey
                BLX     R0
				LDR		R1, SCM_BASE_ADDR
				LDR		R2, REG_PROT_VAL
				STR		R2, [R1, #0x5c]
				STR		R0, [R1, #0x50]
				LSRS	R0, R0, #8
				STR		R0, [R1, #0x54]
				MOVS	R2, #0
				STR		R2, [R1, #0x5c]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

SCM_BASE_ADDR		DCD		0x40000000		; SCM
REG_PROT_VAL		DCD		0x32107654

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
               EXPORT  SysTick_Handler            [WEAK]
               B       .
               ENDP
Default_Handler PROC               
                EXPORT SCM_Handler             		[WEAK]
                EXPORT GPIO_Handler            		[WEAK]
                EXPORT T0_Handler              		[WEAK]
				EXPORT SEA_Handler					[WEAK]	
				EXPORT AEA_Handler        			[WEAK]
				EXPORT SPI0_Handler             	[WEAK]
				EXPORT TRNG_Handler            		[WEAK]
				EXPORT USB_Handler             		[WEAK]
				EXPORT T1_Handler					[WEAK]           	
				EXPORT T2_Handler					[WEAK]           		
				EXPORT SSI_Handler					[WEAK]           		
				EXPORT UART0_Handler				[WEAK]           		
				EXPORT I2C0_Handler					[WEAK]        			
				EXPORT SWPS_Handler					[WEAK]         			
				EXPORT ISO7816_Handler				[WEAK]          		
				EXPORT SCI_Handler					[WEAK]            		
				EXPORT FPA_Handler					[WEAK]           			
				EXPORT VPWM_Handler					[WEAK]  				
				EXPORT UART1_Handler				[WEAK]          			
				EXPORT DMA_Handler					[WEAK]					
				EXPORT EFC_Handler					[WEAK]					
				EXPORT LOCSC_Handler				[WEAK]           	
				EXPORT SPI1_Handler					[WEAK]          	 		
				EXPORT HASH_Handler					[WEAK] 
             
SCM_Handler     
GPIO_Handler    
T0_Handler          
SEA_Handler    ;EAAG_Handler
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
               B       .
               ENDP

                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                ;LDR     R0, =  Heap_Mem
                ;LDR     R1, =(Stack_Mem + Stack_Size)
                ;LDR     R2, = (Heap_Mem +  Heap_Size)
                ;LDR     R3, = Stack_Mem
                ;BX      LR
				; modified for scatter file, 2013.04.07, coins
				IMPORT |Image$$STACK$$Base|
				IMPORT |Image$$STACK$$ZI$$Limit|
				IMPORT |Image$$HEAP$$Base|
				IMPORT |Image$$HEAP$$ZI$$Limit|
				LDR     R0, = |Image$$HEAP$$Base|
				LDR     R1, = |Image$$STACK$$ZI$$Limit|
                LDR     R2, = |Image$$HEAP$$ZI$$Limit|
                LDR     R3, = |Image$$STACK$$Base|
                BX      LR

                ALIGN

                ENDIF
                END

