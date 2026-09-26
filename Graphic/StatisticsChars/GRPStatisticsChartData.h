/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartData.h
* 
* @class      GRPSTATISTICSCHARTDATA
* @brief      Graphic Statistics Chart : data model (categories + series of values), shared by every chart type
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

#include "XBase.h"
#include "XString.h"
#include "XVector.h"

#include "GRPStatisticsChartStyle.h"

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSTATISTICSCHARTSERIE
{
  public:
                                        GRPSTATISTICSCHARTSERIE           ()                                              { hascolor = false;                                     }
    virtual                             ~GRPSTATISTICSCHARTSERIE          ()                                              { values.DeleteAll();                                   }

    XSTRING&                            GetName                           ()                                              { return name;                                          }
    void                                SetName                           (XCHAR* xname)                                  { name.Set(xname);                                      }

    bool                                HasColor                          ()                                              { return hascolor;                                      }
    GRPSTATISTICSCHARTCOLOR&            GetColor                          ()                                              { return color;                                         }
    void                                SetColor                          (GRPSTATISTICSCHARTCOLOR& xcolor)               { color = xcolor; hascolor = true;                      }

    bool                                AddValue                          (double value)                                  { return values.Add(value);                             }
    double                              GetValue                          (XDWORD index)                                  { return (index < values.GetSize()) ? values.Get(index) : 0.0;  }
    XDWORD                              GetNValues                        ()                                              { return values.GetSize();                              }

  private:

    XSTRING                             name;
    bool                                hascolor;
    GRPSTATISTICSCHARTCOLOR             color;
    XVECTOR<double>                     values;
};

class GRPSTATISTICSCHARTDATA
{
  public:
                                        GRPSTATISTICSCHARTDATA            ()                                              {                                                       }
    virtual                             ~GRPSTATISTICSCHARTDATA           ()                                              { Clear();                                              }

    bool                                AddCategory                       (XCHAR* label)
                                        {
                                          XSTRING* category = GEN_NEW XSTRING(label);
                                          if(!category) return false;

                                          return categories.Add(category);
                                        }

    XSTRING*                            GetCategory                       (XDWORD index)                                  { return (index < categories.GetSize()) ? categories.Get(index) : NULL;  }
    XDWORD                              GetNCategories                    ()                                              { return categories.GetSize();                          }

    GRPSTATISTICSCHARTSERIE*            AddSerie                          (XCHAR* name = NULL)
                                        {
                                          GRPSTATISTICSCHARTSERIE* serie = GEN_NEW GRPSTATISTICSCHARTSERIE();
                                          if(!serie) return NULL;

                                          if(name) serie->SetName(name);

                                          if(!series.Add(serie))
                                            {
                                              GEN_DELETE serie;
                                              return NULL;
                                            }

                                          return serie;
                                        }

    GRPSTATISTICSCHARTSERIE*            GetSerie                          (XDWORD index)                                  { return (index < series.GetSize()) ? series.Get(index) : NULL;  }
    XDWORD                              GetNSeries                        ()                                              { return series.GetSize();                              }

    double                              GetMaxValue                       ()
                                        {
                                          double max     = 0.0;
                                          bool   isfirst = true;

                                          for(XDWORD s=0; s<series.GetSize(); s++)
                                            {
                                              GRPSTATISTICSCHARTSERIE* serie = series.Get(s);
                                              if(!serie) continue;

                                              for(XDWORD v=0; v<serie->GetNValues(); v++)
                                                {
                                                  double value = serie->GetValue(v);
                                                  if(isfirst || (value > max)) { max = value; isfirst = false; }
                                                }
                                            }

                                          return max;
                                        }

    double                              GetMinValue                       ()
                                        {
                                          double min     = 0.0;
                                          bool   isfirst = true;

                                          for(XDWORD s=0; s<series.GetSize(); s++)
                                            {
                                              GRPSTATISTICSCHARTSERIE* serie = series.Get(s);
                                              if(!serie) continue;

                                              for(XDWORD v=0; v<serie->GetNValues(); v++)
                                                {
                                                  double value = serie->GetValue(v);
                                                  if(isfirst || (value < min)) { min = value; isfirst = false; }
                                                }
                                            }

                                          return min;
                                        }


    void                                Clear                             ()
                                        {
                                          categories.DeleteContents();
                                          categories.DeleteAll();

                                          series.DeleteContents();
                                          series.DeleteAll();
                                        }

  private:

    XVECTOR<XSTRING*>                   categories;
    XVECTOR<GRPSTATISTICSCHARTSERIE*>   series;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




