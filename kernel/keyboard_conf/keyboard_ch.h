/**
 * @file keyboard_ch.h
 * @brief Mapping of keyboard keys for the swiss layout
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */
#ifndef _KEYBOARD_CONF_CH_H_
#define _KEYBOARD_CONF_CH_H_

#define SIZE 105 // ISO 9995

#define ASCII_ESC 0x1B
#define ASCII_BACKSPACE 0x8
#define ASCII_SPACE 0x20
#define ASCII_TAB 0x9
#define ASCII_SHIFT_IN 0xF
#define ASCII_ENTER 0xD
#define ASCII_UP_ARROW 0x8F
#define ASCII_LEFT_ARROW 0x9F
#define ASCII_RIGHT_ARROW 0xAF
#define ASCII_DOWN_ARROW 0xBF
#define ASCII_NO_VALUE 0x0

// ch configuration
#define ESC 0x1

#define N1 0x2
#define N2 0x3
#define N3 0x4
#define N4 0x5
#define N5 0x6
#define N6 0x7
#define N7 0x8
#define N8 0x9
#define N9 0xA
#define N0 0xB
#define QUOTE 0xC
#define CIRCUMFLEX 0xD
#define BACKSPACE 0xE

#define TAB 0xF
#define Q 0x10
#define W 0x11
#define E 0x12
#define R 0x13
#define T 0x14
#define Z 0x15
#define U 0x16
#define I 0x17
#define O 0x18
#define P 0x19  
#define OP_HOOK 0x1A
#define CL_HOOK 0x1B

#define ENTER 0x1C

#define A 0x1E
#define S 0x1F
#define D 0x20
#define F 0x21
#define G 0x22
#define H 0x23
#define J 0x24
#define K 0x25
#define L 0x26

#define L_SHIFT 0x2A
#define LOWER_UPPER 0x56
#define Y 0x2C
#define X 0x2D
#define C 0x2E
#define V 0x2F
#define B 0x30
#define N 0x31
#define M 0x32
#define COMMA 0x33
#define DOT 0x34
#define HYPHEN 0x35
#define R_SHIFT 0x36

#define SPACE_BAR 0x39

#define UP_ARROW 0x48
#define LEFT_ARROW 0x4B
#define RIGHT_ARROW 0x4D
#define DOWN_ARROW 0x50

extern char keyboard_conf[SIZE];
extern char shift_keyboard_conf[SIZE];

extern void keyboard_conf_init();

#endif