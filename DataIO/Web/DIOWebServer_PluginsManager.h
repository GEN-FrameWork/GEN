/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_PluginsManager.h
* 
* @class      DIOWEBSERVER_PLUGINSMANAGER
* @brief      Data Input/Output Web Server Plugings Manager class
* @ingroup    DATAIO
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

#include "XString.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIOWEBSERVER_PLUGIN;


class DIOWEBSERVER_PLUGINSMANAGER
{
  public:
                                      DIOWEBSERVER_PLUGINSMANAGER   ();
    virtual                          ~DIOWEBSERVER_PLUGINSMANAGER   ();

    bool                              Plugins_Add                   (DIOWEBSERVER_PLUGIN* plugin);
    XDWORD                            Plugins_GetNPlugins           ();
    DIOWEBSERVER_PLUGIN*              Plugins_Get                   (int index);
    bool                              Plugins_Delete                (DIOWEBSERVER_PLUGIN* plugin);
    bool                              Plugins_DeleteAll             ();

  private:

    void                              Clean                         ();

    XVECTOR<DIOWEBSERVER_PLUGIN*>     plugins;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





