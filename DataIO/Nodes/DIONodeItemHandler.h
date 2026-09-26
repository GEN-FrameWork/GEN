/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeItemHandler.h
* 
* @class      DIONODEITEMHANDLER
* @brief      Data Input/Output Node Item Handler class
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

#include "XBase.h"
#include "XVector.h"
#include "XSubject.h"

#include "DIONodeItem.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum DIONODEITEMHANDLER_TYPE
{
  DIONODEITEMHANDLER_TYPE_UNKNOWN            = 0 ,
    
  DIONODEITEMHANDLER_TYPE_SENSOR_AM2315          ,
  DIONODEITEMHANDLER_TYPE_GPIODIGITAL            ,   

  DIONODEITEMHANDLER_TYPE_OWNER                          
};

#define DIONODEITEMHANDLER_INVALIDPARAM     -1 

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIONODEITEMHANDLER : public XSUBJECT
{
  public:
                                  DIONODEITEMHANDLER     ();
    virtual                      ~DIONODEITEMHANDLER     ();

    XDWORD                        GetType                 (); 
    void                          SetType                 (XDWORD type);      
    XSTRING*                      GetName                 ();      

    virtual bool                  Open                    ();
    virtual bool                  Update                  ();
    virtual bool                  Close                   ();

    bool                          IsOpen                  ();
    bool                          IsWorking               ();

    DIONODEITEM*                  GetNodeItem             ();
    virtual bool                  SetNodeItem             (DIONODEITEM* nodeitem);    

  protected:

    XDWORD                        type;      
    XSTRING                       name; 
    
    bool                          isopen;   
    bool                          isworking;

  private:

    void                          Clean                   (); 

    DIONODEITEM*                  nodeitem;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





