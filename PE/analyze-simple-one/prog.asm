; Listing generated by Microsoft (R) Optimizing Compiler Version 19.28.29337.0 

; include listing.inc

; INCLUDELIB LIBCMT
; INCLUDELIB OLDNAMES


;
;  PUBLIC Makes the specified variable, label or absolute symbol vailable to all other modules in the program.
;
PUBLIC	start

EXTRN	__imp_ExitProcess:PROC
EXTRN	__imp_MessageBoxA:PROC

; pdata	SEGMENT
; 
; $pdata$start DD	imagerel $LN3
; 	DD	imagerel $LN3+47
; 	DD	imagerel $unwind$start
; pdata	ENDS

_DATA	SEGMENT
$SG95679 DB	'Hello...', 00H
	ORG $+7
$SG95680 DB	'...World', 00H
_DATA	ENDS
xdata	SEGMENT
$unwind$start DD 010901H
	DD	04209H
xdata	ENDS
; Function compile flags: /Odtp
_TEXT	SEGMENT
PEB$ = 48
start	PROC
; File C:\Users\Rene\github\temp\Windows\WinAPI-hooking\PE\analyze-simple-one\prog.c
; Line 3
$LN3:
	mov	QWORD PTR [rsp+8], rcx
	sub	rsp, 40					; 00000028H
; Line 5
	xor	r9d, r9d
	lea	r8, OFFSET $SG95679
	lea	rdx, OFFSET $SG95680
	xor	ecx, ecx
	call	QWORD PTR __imp_MessageBoxA
; Line 6
	xor	ecx, ecx
	call	QWORD PTR __imp_ExitProcess
$LN2@start:
; Line 7
	add	rsp, 40					; 00000028H
	ret	0
start	ENDP
_TEXT	ENDS
END
