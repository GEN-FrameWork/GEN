/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONotificationsManager.h
* 
* @class      DIONOTIFICATIONSMANAGER
* @brief      Data Input/Output Notifications Manager class
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

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class DIONOTIFICATION
{
  public:
                                    DIONOTIFICATION           ();
    virtual                        ~DIONOTIFICATION           ();

    XSTRING*                        Get_Title                 ();
    XSTRING*                        Get_Message               ();

    void*                           Application_GetHandle      ();
    void                            Application_SetHandle      (void* applicationhandle);

    void*                           GetApplicationIcon        ();
    void                            SetApplicationIcon        (void* applicationicon);

  private:

    void                            Clean                     ();

    XSTRING                         title;
    XSTRING                         message;

    void*                           applicationhandle;
    void*                           applicationicon;
};

class DIONOTIFICATIONSMANAGER
{
  public:
                                    DIONOTIFICATIONSMANAGER   ();
    virtual                        ~DIONOTIFICATIONSMANAGER   ();


    virtual bool                    Ini                       (XCHAR* ownertitle, XCHAR* appname);

    XSTRING*                        GetOwnerTitle             ();   
    XSTRING*                        GetAppName                ();  

    virtual bool                    Do                        (DIONOTIFICATION* notification);
    bool                            Do                        (DIONOTIFICATION& notification);

    virtual bool                    End                       ();

  protected:

    XSTRING                         ownertitle;   
    XSTRING                         appname;   

  private:

    void                            Clean                     (); 
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





