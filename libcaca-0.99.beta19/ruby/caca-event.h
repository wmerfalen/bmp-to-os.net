/*
 *  libcaca Ruby bindings
 *  Copyright (c) 2007-2010 Pascal Terjan <pterjan@linuxfr.org>
 *                2012 Sam Hocevar <sam@hocevar.net>
 *
 *  This library is free software. It comes without any warranty, to
 *  the extent permitted by applicable law. You can redistribute it
 *  and/or modify it under the terms of the Do What the Fuck You Want
 *  to Public License, Version 2, as published by Sam Hocevar. See
 *  http://www.wtfpl.net/ for more details.
 */

#ifndef __CACA_EVENT_H__
#define __CACA_EVENT_H__

#include <ruby.h>

extern VALUE cEvent;
extern VALUE cEventKeyPress;
extern VALUE cEventKeyRelease;
extern VALUE cEventMouse;
extern VALUE cEventMousePress;
extern VALUE cEventMouseRelease;
extern VALUE cEventMouseMotion;
extern VALUE cEventResize;
extern VALUE cEventQuit;
extern void Init_caca_event(VALUE);

#endif
