section .data

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
    global main
main:
		 ENTER 12,0 
		 pushAll
		 mov RAX , 15 
		 mov  [RBP-6], RAX
		 popAll
		 pushAll
		 mov RAX , 9 
		 mov  [RBP-8], RAX
		 popAll
         leave
         ret