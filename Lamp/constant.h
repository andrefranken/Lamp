/******************************************************************************
 *
 *  Latest Chatty Command Line Client
 *  By Electroly.
 *
 *  CONSTANT.H
 *
 *  Global constants and macros.
 *  
 *****************************************************************************/

#pragma once

#define HOSTNAME         "www.shackchatty.com"

#define MAX_POST_LENGTH  4096

/* Box-drawing characters */
#define BOX_HORIZONTAL   0xCD
#define BOX_UPPER_LEFT   0xC9
#define BOX_UPPER_RIGHT  0xBB
#define BOX_VERTICAL     0xBA
#define BOX_LOWER_LEFT   0xC8
#define BOX_LOWER_RIGHT  0xBC
#define BOX_RULE_LEFT    0xC7
#define BOX_RULE_RIGHT   0xB6
#define BOX_RULE         0xC4

/* Single-line box-drawing characters */
#define SBOX_HORIZONTAL  0xC4
#define SBOX_UPPER_LEFT  0xDA
#define SBOX_UPPER_RIGHT 0xBF
#define SBOX_VERTICAL    0xB3
#define SBOX_LOWER_LEFT  0xC0
#define SBOX_LOWER_RIGHT 0xD9

/* Scrollbar background and foreground characters */
#define SCROLLBAR_BG     0xB0
#define SCROLLBAR_FG     0xDB

/* Extended keystroke characters */
#define KEY_UP           72
#define KEY_LEFT         75
#define KEY_RIGHT        77
#define KEY_DOWN         80
#define KEY_ESCAPE       27
#define KEY_F5           63
#define KEY_F10          68
#define KEY_PAGEUP       73
#define KEY_PAGEDOWN     81
#define KEY_HOME         71
#define KEY_END          79
#define KEY_BACKSPACE    8
#define KEY_DELETE       83

/* Error reporting.  All fails are epic. */
typedef int error_t;
#define OK   0
#define FAIL (-1)

#define MIN(x, y) ((x)>(y)?(y):(x))
#define MAX(x, y) ((x)<(y)?(y):(x))

#ifndef BOOL
typedef int BOOL;
#define TRUE 1
#define FALSE 0
#endif

