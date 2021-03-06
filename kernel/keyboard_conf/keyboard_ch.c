/**
 * @file keyboard_ch.c
 * @brief Mapping of keyboard keys for the swiss layout
 *
 * @author Ottavio Buonomo & Jean-Daniel Küenzi,
 * ottavio.buonomo@etu.hesge.ch & jean-daniel.kuenzi@etu.hesge.ch
 * @bug No known bugs.
 * @date March 8, 2021
 * @version 0.1
 */

#include "./keyboard_ch.h"

char keyboard_conf[SIZE];
char shift_keyboard_conf[SIZE];

/**
 * Mapping of keyboard keys with shit pressed
 */
void shift_keyboard_conf_init() {
    shift_keyboard_conf[ESC] = ASCII_ESC;

    shift_keyboard_conf[N1] = '+';
    shift_keyboard_conf[N2] = '"';
    shift_keyboard_conf[N3] = '*';
    shift_keyboard_conf[N4] = '4';
    shift_keyboard_conf[N5] = '%';
    shift_keyboard_conf[N6] = '&';
    shift_keyboard_conf[N7] = '/';
    shift_keyboard_conf[N8] = '(';
    shift_keyboard_conf[N9] = ')';
    shift_keyboard_conf[N0] = '=';
    shift_keyboard_conf[QUOTE] = '?';
    shift_keyboard_conf[CIRCUMFLEX] = '`';
    shift_keyboard_conf[BACKSPACE] = ASCII_BACKSPACE;

    shift_keyboard_conf[TAB] = ASCII_TAB;
    shift_keyboard_conf[Q] = 'Q';
    shift_keyboard_conf[W] = 'W';
    shift_keyboard_conf[E] = 'E';
    shift_keyboard_conf[R] = 'R';
    shift_keyboard_conf[T] = 'T';
    shift_keyboard_conf[Z] = 'Z';
    shift_keyboard_conf[U] = 'U';
    shift_keyboard_conf[I] = 'I';
    shift_keyboard_conf[O] = 'O';
    shift_keyboard_conf[P] = 'P';
    shift_keyboard_conf[OP_HOOK] = ASCII_NO_VALUE;
    shift_keyboard_conf[CL_HOOK] = '!';

    shift_keyboard_conf[ENTER] = ASCII_ENTER;

    shift_keyboard_conf[A] = 'A';
    shift_keyboard_conf[S] = 'S';
    shift_keyboard_conf[D] = 'D';
    shift_keyboard_conf[F] = 'F';
    shift_keyboard_conf[G] = 'G';
    shift_keyboard_conf[H] = 'H';
    shift_keyboard_conf[J] = 'J';
    shift_keyboard_conf[K] = 'K';
    shift_keyboard_conf[L] = 'L';

    shift_keyboard_conf[L_SHIFT] = ASCII_SHIFT_IN;
    shift_keyboard_conf[LOWER_UPPER] = '>';
    shift_keyboard_conf[Y] = 'Y';
    shift_keyboard_conf[X] = 'X';
    shift_keyboard_conf[C] = 'C';
    shift_keyboard_conf[V] = 'V';
    shift_keyboard_conf[B] = 'B';
    shift_keyboard_conf[N] = 'N';
    shift_keyboard_conf[M] = 'M';
    shift_keyboard_conf[COMMA] = ';';
    shift_keyboard_conf[DOT] = ':';
    shift_keyboard_conf[HYPHEN] = '_';
    shift_keyboard_conf[R_SHIFT] = ASCII_SHIFT_IN;

    shift_keyboard_conf[SPACE_BAR] = ASCII_SPACE;

    shift_keyboard_conf[UP_ARROW] = ASCII_UP_ARROW;
    shift_keyboard_conf[LEFT_ARROW] = ASCII_LEFT_ARROW;
    shift_keyboard_conf[RIGHT_ARROW] = ASCII_RIGHT_ARROW;
    shift_keyboard_conf[DOWN_ARROW] = ASCII_DOWN_ARROW;
}

/**
 * Mapping of keyboard keys with shit pressed
 */
void keyboard_conf_init() {
    keyboard_conf[ESC] = ASCII_ESC;

    keyboard_conf[N1] = '1';
    keyboard_conf[N2] = '2';
    keyboard_conf[N3] = '3';
    keyboard_conf[N4] = '4';
    keyboard_conf[N5] = '5';
    keyboard_conf[N6] = '6';
    keyboard_conf[N7] = '7';
    keyboard_conf[N8] = '8';
    keyboard_conf[N9] = '9';
    keyboard_conf[N0] = '0';
    keyboard_conf[QUOTE] = '\'';
    keyboard_conf[CIRCUMFLEX] = '^';
    keyboard_conf[BACKSPACE] = ASCII_BACKSPACE;

    keyboard_conf[TAB] = ASCII_TAB;
    keyboard_conf[Q] = 'q';
    keyboard_conf[W] = 'w';
    keyboard_conf[E] = 'e';
    keyboard_conf[R] = 'r';
    keyboard_conf[T] = 't';
    keyboard_conf[Z] = 'z';
    keyboard_conf[U] = 'u';
    keyboard_conf[I] = 'i';
    keyboard_conf[O] = 'o';
    keyboard_conf[P] = 'p';
    keyboard_conf[OP_HOOK] = ASCII_NO_VALUE;
    keyboard_conf[CL_HOOK] = ASCII_NO_VALUE;

    keyboard_conf[ENTER] = ASCII_ENTER;

    keyboard_conf[A] = 'a';
    keyboard_conf[S] = 's';
    keyboard_conf[D] = 'd';
    keyboard_conf[F] = 'f';
    keyboard_conf[G] = 'g';
    keyboard_conf[H] = 'h';
    keyboard_conf[J] = 'j';
    keyboard_conf[K] = 'k';
    keyboard_conf[L] = 'l';

    keyboard_conf[L_SHIFT] = ASCII_SHIFT_IN;
    keyboard_conf[LOWER_UPPER] = '<';
    keyboard_conf[Y] = 'y';
    keyboard_conf[X] = 'x';
    keyboard_conf[C] = 'c';
    keyboard_conf[V] = 'v';
    keyboard_conf[B] = 'b';
    keyboard_conf[N] = 'n';
    keyboard_conf[M] = 'm';
    keyboard_conf[COMMA] = ',';
    keyboard_conf[DOT] = '.';
    keyboard_conf[HYPHEN] = '-';
    keyboard_conf[R_SHIFT] = ASCII_SHIFT_IN;

    keyboard_conf[SPACE_BAR] = ASCII_SPACE;

    keyboard_conf[UP_ARROW] = ASCII_UP_ARROW;
    keyboard_conf[LEFT_ARROW] = ASCII_LEFT_ARROW;
    keyboard_conf[RIGHT_ARROW] = ASCII_RIGHT_ARROW;
    keyboard_conf[DOWN_ARROW] = ASCII_DOWN_ARROW;

    shift_keyboard_conf_init();
}