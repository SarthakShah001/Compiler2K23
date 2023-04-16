extern printf, scanf
section .data

		 int_val : db "%d" , 0
		 real_val : db "%lf" , 0
		 bool_val : db "%d" , 0
section .text

%macro pushAll 0
		 push RAX 
		 push RBX 
		 push RCX 
		 push RDX 
		 push RSP 
		 push RBP 
		 push RSI 
		 push RDI 
%endmacro 

%macro popAll 0
		 pop RDI 
		 pop RSI 
		 pop RBP 
		 pop RSP 
		 pop RDX 
		 pop RCX 
		 pop RBX 
		 pop RAX 
%endmacro 

		 pushAll
		 mov RDI , int_val 
		 push RBP
		 mov RDX, RBP
		 sub RDX, 12 
		 mov RAX, 0x0000_ffff_ffff_ffff
		 mov [RDX] , RAX
		 mov RSI, RDX
		 xor RAX, RAX 
		 call scanf 
		 pop RBP 
		 popAll

		 pushAll
		 mov RDI , int_val 
		 push RBP
		 mov RDX, RBP
		 sub RDX, 10 
		 mov RAX, 0x0000_ffff_ffff_ffff
		 mov [RDX] , RAX
		 mov RSI, RDX
		 xor RAX, RAX 
		 call scanf 
		 pop RBP 
		 popAll

		 pushAll
		 mov RAX , [RBP-0] 
		 mov RBX , [RBP-2] 
		 mul RBX  
		 mov [RBP-15], RAX
		 popAll

		 pushAll
		 mov RAX , 2 
		 mov RBX , [RBP-12] 
		 mul RBX  
		 mov [RBP-17], RAX
		 popAll

		 pushAll
		 popAll

		 pushAll
		 mov RAX , 3 
		 mov RBX , [RBP-19] 
		 sub RAX ,RBX  
		 mov [RBP-21], RAX
		 popAll

		 pushAll
		 mov RAX , [RBP-21] 
		 mov [RBP-8] , RAX
		 popAll

		 pushAll
		 mov RAX , [RBP-25] 
		 mov [RBP-14] , RAX
		 popAll

		 pushAll
		 mov RAX , [RBP-8] 
		 mov RBX , [RBP-10] 
		 add RAX ,RBX  
		 mov [RBP-27], RAX
		 popAll

		 pushAll
		 mov RAX , [RBP-27] 
		 mov [RBP-5] , RAX
		 popAll

		 pushAll
		 mov RAX , [RBP-31] 
		 mov [RBP-7] , RAX
		 popAll

global main
main:
		 ENTER 8,0
		 SUB RSP , 0

		 pushAll
		 mov RDI , int_val 
		 push RBP
		 mov RDX, RBP
		 sub RDX, 0 
		 mov RAX, 0x0000_ffff_ffff_ffff
		 mov [RDX] , RAX
		 mov RSI, RDX
		 xor RAX, RAX 
		 call scanf 
		 pop RBP 
		 popAll

		 pushAll
		 mov RDI , int_val 
		 push RBP
		 mov RDX, RBP
		 sub RDX, 2 
		 mov RAX, 0x0000_ffff_ffff_ffff
		 mov [RDX] , RAX
		 mov RSI, RDX
		 xor RAX, RAX 
		 call scanf 
		 pop RBP 
		 popAll

		 pushAll
		 popAll

		 leave
		 ret
