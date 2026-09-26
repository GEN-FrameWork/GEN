/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPVectorFileDXFEntityDefPoint.h
* 
* @class      GRPVECTORFILEDXFENTITYDEFPOINT
* @brief      Graphic Vector File DXF Entity DefPoint class
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

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPVECTORFILEDXFENTITYDEFPOINT 
{
  public:   
                                GRPVECTORFILEDXFENTITYDEFPOINT        ();
                                GRPVECTORFILEDXFENTITYDEFPOINT        (double xpoint , double ypoint , double zpoint);
                                GRPVECTORFILEDXFENTITYDEFPOINT        (GRPVECTORFILEDXFENTITYDEFPOINT* point);
                               ~GRPVECTORFILEDXFENTITYDEFPOINT        ();

    void                        operator =                            (GRPVECTORFILEDXFENTITYDEFPOINT* point);
    void                        operator =                            (GRPVECTORFILEDXFENTITYDEFPOINT point);

    double                      GetX                                  ();
    void                        SetX                                  (double xpoint);

    double                      GetY                                  ();
    void                        SetY                                  (double ypoint);

    double                      GetZ                                  ();           
    void                        SetZ                                  (double zpoint);

    bool                        CopyTo                                (GRPVECTORFILEDXFENTITYDEFPOINT* point);
    bool                        CopyFrom                              (GRPVECTORFILEDXFENTITYDEFPOINT* point);
   
  private: 

    void                        Clean                                 ();

    double                      xpoint;
    double                      ypoint;
    double                      zpoint;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





