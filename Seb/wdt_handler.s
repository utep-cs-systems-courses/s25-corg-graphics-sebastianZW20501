.text
  .balign 2                ; align to even address
  .global WDT

  ; WDT uses interrupt vector #11
  .section __interrupt_vector_11, "ax"
  .word WDT                ; Entry for vector table

.text

  .extern redrawScreen     ; External reference to C variable
  .extern wdt_c_handler    ; External reference to C function

WDT:
  ; --- Prologue: save registers ---
  PUSH    R15
  PUSH    R14
  PUSH    R13
  PUSH    R12
  PUSH    R11
  PUSH    R10
  PUSH    R9
  PUSH    R8
  PUSH    R7
  PUSH    R6
  PUSH    R5
  PUSH    R4

  ; --- Call C handler ---
  CALL    #wdt_c_handler

  ; --- Epilogue: restore registers ---
  POP     R4
  POP     R5
  POP     R6
  POP     R7
  POP     R8
  POP     R9
  POP     R10
  POP     R11
  POP     R12
  POP     R13
  POP     R14
  POP     R15

  ; --- Conditionally wake CPU if redrawScreen is true ---
  CMP     #0, &redrawScreen   ; Check if redrawScreen != 0
  JZ      dont_wake
  AND     #0xffef, 0(R1)      ; Clear CPUOFF bit in saved status register (wake CPU)

dont_wake:
  RETI                        ; Return from interrupt (restores SR & PC)

