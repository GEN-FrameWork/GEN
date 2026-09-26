/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONode.h
* 
* @class      DIONODE
* @brief      Data Input/Output NODE (Base class for a control of nodes) class
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

#include "XVector.h"
#include "XUUID.h"
#include "XSerializable.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIONODEITEM;

class DIONODE : public XSERIALIZABLE
{
  public:
                                    DIONODE                   ();
    virtual                        ~DIONODE                   ();

    XUUID&                          GetID                     ();
    void                            SetID                     (XUUID& UUID);

    bool                            IsLocal                   ();    
    void                            SetIsLocal                (bool islocal);

    XSTRING*                        GetGroup                  ();

    bool                            GetSubGroups              (XVECTOR<XSTRING*>& listsubgroups);
    bool                            GetSubGroup               (XDWORD level, XSTRING& subgroup);

    XVECTOR<DIONODEITEM*>*          GetItems                  ();   

    bool                            CreateJSONSerialization   (); 

    bool                            Update                    ();

    virtual bool                    Serialize                 ();                                          
    virtual bool                    Deserialize               ();

  protected:

  private:

    void                            Clean                     ();

    XUUID                           UUID;  
    bool                            islocal;
    XSTRING                         group;
    XVECTOR<DIONODEITEM*>           items;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





