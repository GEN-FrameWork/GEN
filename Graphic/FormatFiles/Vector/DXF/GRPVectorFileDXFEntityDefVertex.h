/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFEntityDefVertex.h
* 
* @class      GRPVECTORFILEDXFENTITYDEFVERTEX
* @brief      Graphic Vector File DXF Entity DefVertex class
* @ingroup    GRAPHIC
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

#include "GRPVectorFileDXFEntityDefPoint.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPVECTORFILEDXFENTITYDEFVERTEX 
{
  public:
   
                                        GRPVECTORFILEDXFENTITYDEFVERTEX       ();
                                        GRPVECTORFILEDXFENTITYDEFVERTEX       (GRPVECTORFILEDXFENTITYDEFVERTEX* vertex);
                                       ~GRPVECTORFILEDXFENTITYDEFVERTEX       ();

    void                                operator =                            (GRPVECTORFILEDXFENTITYDEFVERTEX* vertex);
    void                                operator =                            (GRPVECTORFILEDXFENTITYDEFVERTEX vertex);

    GRPVECTORFILEDXFENTITYDEFPOINT*     GetPoint                              (); 
    bool                                SetPoint                              (GRPVECTORFILEDXFENTITYDEFPOINT* point); 

    double                              GetIniWidth                           ();
    void                                SetIniWidth                           (double iniwith);

    double                              GetEndWidth                           ();      
    void                                SetEndWidth                           (double endwidth);

    double                              GetBulge                              ();      
    void                                SetBulge                              (double bulge);
      
    bool                                CopyTo                                (GRPVECTORFILEDXFENTITYDEFVERTEX* vertex);
    bool                                CopyFrom                              (GRPVECTORFILEDXFENTITYDEFVERTEX* vertex);
   
   private: 

    void                                Clean                                 ();

    GRPVECTORFILEDXFENTITYDEFPOINT      point;
    double                              iniwidth;
    double                              endwidth;
    double                              bulge;
};



/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





