/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWifiManagerMode.h
* 
* @class      DIOWIFIMANAGERMODE
* @brief      Data Input/Output Wifi Manager Mode class
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIOWIFIMANAGER_MODETYPE 
{
  DIOWIFIMANAGER_MODETYPE_DEFAULT                    = 0 ,
  DIOWIFIMANAGER_MODETYPE_LINUX_CLIENT_NM               
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOIP;

class DIOWIFIMANAGERMODE
{
  public:
                                      DIOWIFIMANAGERMODE              ();
    virtual                          ~DIOWIFIMANAGERMODE              ();
    
    bool                              GetSelectNetInterface           (XSTRING& netinterface);
    bool                              SetSelectNetInterface           (XCHAR* netinterface);
    bool                              SetSelectNetInterface           (XSTRING& netinterface);

    DIOWIFIMANAGER_MODETYPE           GetModeType                     ();
    void                              SetModeType                     (DIOWIFIMANAGER_MODETYPE modetype = DIOWIFIMANAGER_MODETYPE_DEFAULT);     
  
    bool                              CheckNetInterface               (DIOIP* IP); 
    bool                              CheckNetInterface               (XSTRING* IP); 
      
    virtual bool                      SetMode_AP                      (XCHAR* SSID, XCHAR* password, bool serialize);      
    bool                              SetMode_AP                      (XSTRING& SSID, XSTRING& password, bool serialize);      
    bool                              SetMode_AP                      (XSTRING& SSID, XCHAR* password, bool serialize);      

    virtual bool                      SetMode_Client                  (XCHAR* SSID, XCHAR* password, bool serialize);      
    bool                              SetMode_Client                  (XSTRING& SSID, XSTRING& password, bool serialize);  
    bool                              SetMode_Client                  (XSTRING& SSID, XCHAR* password, bool serialize);  

    virtual bool                      IsDisconnected                  ();
    virtual bool                      Disconnect                      ();  

    virtual bool                      APList_Reload                   ();
    XVECTOR<XSTRING*>*                APList_Get                      ();
  
    
  protected: 
  
    XSTRING                           netinterface;
    XVECTOR<XSTRING*>                 nameAPs;
    DIOWIFIMANAGER_MODETYPE           modetype;

  private:

    void                              Clean                           ();    
}; 
   



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





