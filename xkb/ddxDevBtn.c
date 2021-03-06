/************************************************************
Copyright (c) 1995 by Silicon Graphics Computer Systems, Inc.

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of Silicon Graphics not be 
used in advertising or publicity pertaining to distribution 
of the software without specific prior written permission.
Silicon Graphics makes no representation about the suitability 
of this software for any purpose. It is provided "as is"
without any express or implied warranty.

SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
THE USE OR PERFORMANCE OF THIS SOFTWARE.

********************************************************/

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/keysym.h>
#include "inputstr.h"
#include "scrnintstr.h"
#include "windowstr.h"
#include "eventstr.h"
#include <xkbsrv.h>
#include "mi.h"
#include <X11/extensions/XI.h>
#include <X11/extensions/XIproto.h>

void
XkbDDXFakeDeviceButton(DeviceIntPtr dev,Bool press,int button)
{
    EventListPtr        events;
    int                 nevents, i;
    DeviceIntPtr        ptr;

    /* If dev is a slave device, and the SD is attached, do nothing. If we'd
     * post through the attached master pointer we'd get duplicate events.
     *
     * if dev is a master keyboard, post through the master pointer.
     *
     * if dev is a floating slave, post through the device itself.
     */

    if (IsMaster(dev))
        ptr = GetMaster(dev, MASTER_POINTER);
    else if (!dev->u.master)
        ptr = dev;
    else
        return;

    events = InitEventList(GetMaximumEventsNum());
    nevents = GetPointerEvents(events, ptr,
                               press ? ButtonPress : ButtonRelease, button,
                               0 /* flags */, 0 /* first */,
                               0 /* num_val */, NULL);


    for (i = 0; i < nevents; i++)
        mieqProcessDeviceEvent(ptr, (InternalEvent*)events[i].event, NULL);

    FreeEventList(events, GetMaximumEventsNum());
}
