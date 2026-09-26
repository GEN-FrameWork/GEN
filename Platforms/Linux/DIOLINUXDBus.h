/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOLINUXDBus.h
* 
* @class      DIOLINUXDBUS
* @brief      LINUX Data Input/Output DBus class
* @ingroup    PLATFORM_LINUX
* 
* @copyright  EndoraSoft. All rights reserved.
* 
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
* 
* --------------------------------------------------------------------------------------------------------------------*/

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <dbus/dbus.h>

#include "XSubject.h"
#include "XVariant.h"
#include "XThreadCollected.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/
                                                   
#define DIOLINUXDBUS_IFACE_PROPERTIES         __L("org.freedesktop.DBus.Properties")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOLINUXDBUS_MESSAGE
{
  public:
                                        DIOLINUXDBUS_MESSAGE        ();
    virtual                            ~DIOLINUXDBUS_MESSAGE        ();

    XSTRING*                            GetDestination              ();
    XSTRING*                            GetPath                     ();
    XSTRING*                            GetIFace                    ();
    XSTRING*                            GetMethod                   (); 

    bool                                Create                      (); 
    DBusMessage*                        GetHandler                  ();
    bool                                SetHandler                  (DBusMessage* handler);
    bool                                Delete                      ();    

    bool                                GetResult                   (XDWORD& result);
    bool                                GetResult                   (XSTRING& result);
    bool                                GetResult                   (XVECTOR<XSTRING*>& result);    

  private:
    
    void                                Clean                       ();

    DBusMessage*                        handler;

    XSTRING                             destination;
    XSTRING                             path;
    XSTRING                      	      iface;
    XSTRING                             method; 
};


class DIOLINUXDBUS_ARGUMENTS
{
  public:
                                        DIOLINUXDBUS_ARGUMENTS      (bool in, DIOLINUXDBUS_MESSAGE* message);
    virtual                            ~DIOLINUXDBUS_ARGUMENTS      ();

    DBusMessageIter*                    GetHandler                  (); 

    bool                                Append                      (bool value);
    bool                                Append                      (XCHAR* value); 

    bool                                AppendIterator              (bool value);
    bool                                AppendIterator              (XCHAR* value); 

    bool                                GetResultInteractor         (XVARIANT& result);

  private:
    
    void                                Clean                       ();

    bool                                in;
    DIOLINUXDBUS_MESSAGE*               message;
    DBusMessageIter                     iter;
};

class DIOLINUXDBUS_SIGNAL
{
  public:
                                        DIOLINUXDBUS_SIGNAL         ();
    virtual                            ~DIOLINUXDBUS_SIGNAL         ();

    XSTRING*                            GetType                     ();
    XSTRING*                            GetName                     ();  

  private:
    
    void                                Clean                       ();

    XSTRING                             type;
    XSTRING                             name;  

};

class DIOLINUXDBUS : public XSUBJECT
{
  public:
                                        DIOLINUXDBUS                ();
    virtual                            ~DIOLINUXDBUS                ();

    bool                                Ini                         (DBusBusType type = DBUS_BUS_SYSTEM);  

    bool                                IsConnected                 ();

    bool                                CallMethod                  (DIOLINUXDBUS_MESSAGE& message, DIOLINUXDBUS_MESSAGE& reply);
    
    bool                                Signal_Add                  (XCHAR* type, XCHAR* name);                
    bool                                Signal_Add                  (XSTRING& type, XSTRING& name);   
    bool                                Signal_IsTypeAdd            (XCHAR* type);
    bool                                Signal_DeleteAll            ();                

    bool                                End                         ();  

    static bool                         TraceError                  (DBusError& error);

  private:

    static void                         Thread_Signals              (void* param);

    void                                Clean                       ();

    DBusBusType                         type;
    DBusConnection*                     connection; 
    XSTRING                             connection_name;

    XVECTOR<DIOLINUXDBUS_SIGNAL*>       signals;

    XTHREADCOLLECTED*                   thread_signals;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





