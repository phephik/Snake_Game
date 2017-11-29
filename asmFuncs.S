; HOWTO ASM syntax in CB: https://github.com/samoylenko/codeblocks_avr_asm

; Set offset for control register addresses (NEEDED)
#define __SFR_OFFSET 0

;Defines I/O port aliases
#include <avr/io.h>


; Makes assembly functions visible in other source files

.global comparator





not_equal:
    ldi     r24, 0      ;put 0 to output register
    ret

comparator:
    mov     r31,r24    ;move input value 1
    mov     r30,r22     ;move input value 2
    cp      r30,r31     ;compare val1 and val 2
    brne    not_equal   ;if not equal jump to not_equal else continue in this branch
    ldi     r24, 1      ;put 1 to output register
    ret




