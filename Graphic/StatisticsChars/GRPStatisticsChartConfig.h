/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRPStatisticsChartConfig.h
* 
* @class      GRPSTATISTICSCHARTCONFIG
* @brief      Graphic Statistics Chart : visual configuration (title, colors, palette, grid and legend), format independent
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

#include "GRPStatisticsChartStyle.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define GRPSTATISTICSCHARTCONFIG_PALETTESIZE    8

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class GRPSTATISTICSCHARTCONFIG
{
  public:
                                      GRPSTATISTICSCHARTCONFIG              ()                                              { SetDefault();                                         }
    virtual                          ~GRPSTATISTICSCHARTCONFIG              ()                                              {                                                       }

    XSTRING&                          GetTitle                              ()                                              { return title;                                         }
    void                              SetTitle                              (XCHAR* xtitle)                                 { title.Set(xtitle);                                    }

    bool                              HasBackground                         ()                                              { return hasbackground;                                 }
    void                              SetHasBackground                      (bool has)                                      { hasbackground = has;                                  }
    GRPSTATISTICSCHARTCOLOR&          GetBackgroundColor                    ()                                              { return backgroundcolor;                               }
    void                              SetBackgroundColor                    (GRPSTATISTICSCHARTCOLOR& color)                { backgroundcolor = color; hasbackground = true;        }

    GRPSTATISTICSCHARTCOLOR&          GetAxisColor                          ()                                              { return axiscolor;                                     }
    GRPSTATISTICSCHARTCOLOR&          GetGridColor                          ()                                              { return gridcolor;                                     }
    GRPSTATISTICSCHARTCOLOR&          GetTextColor                          ()                                              { return textcolor;                                     }

    double                            GetFontSize                           ()                                              { return fontsize;                                      }
    void                              SetFontSize                           (double size)                                   { fontsize = size;                                      }
    double                            GetTitleFontSize                      ()                                              { return titlefontsize;                                 }
    void                              SetTitleFontSize                      (double size)                                   { titlefontsize = size;                                 }

    bool                              GetShowGrid                           ()                                              { return showgrid;                                      }
    void                              SetShowGrid                           (bool show)                                     { showgrid = show;                                      }
    bool                              GetShowValues                         ()                                              { return showvalues;                                    }
    void                              SetShowValues                         (bool show)                                     { showvalues = show;                                    }

    bool                              GetShowAxisLabels                     ()                                              { return showaxislabels;                                }
    void                              SetShowAxisLabels                     (bool show)                                     { showaxislabels = show;                                }
    double                            GetAxisFontSize                       ()                                              { return axisfontsize;                                  }
    void                              SetAxisFontSize                       (double size)                                   { axisfontsize = size;                                  }

    bool                              GetShowLegend                         ()                                              { return showlegend;                                    }
    void                              SetShowLegend                         (bool show)                                     { showlegend = show;                                    }
    GRPSTATISTICSCHARTLEGENDPOSITION  GetLegendPosition                     ()                                              { return legendposition;                                }
    void                              SetLegendPosition                     (GRPSTATISTICSCHARTLEGENDPOSITION position)     { legendposition = position;                            }
    double                            GetLegendFontSize                     ()                                              { return legendfontsize;                                }
    void                              SetLegendFontSize                     (double size)                                   { legendfontsize = size;                                }

    XSTRING&                          GetFontFileName                       ()                                              { return fontfilename;                                  }
    void                              SetFontFileName                       (XCHAR* xname)                                  { fontfilename.Set(xname);                              }

    double                            GetMargin                             ()                                              { return margin;                                        }
    void                              SetMargin                             (double value)                                  { margin = value;                                       }

    GRPSTATISTICSCHARTCOLOR&          GetPaletteColor                       (XDWORD index)                                  { return palette[index % GRPSTATISTICSCHARTCONFIG_PALETTESIZE];  }

  private:

    void                              SetDefault                            ()
                                      {
                                        fontsize        = 12.0;
                                        titlefontsize   = 18.0;
                                        margin          = 16.0;
                                        showgrid        = true;
                                        showvalues      = false;

                                        showaxislabels  = true;
                                        axisfontsize    = 12.0;

                                        showlegend      = true;
                                        legendposition  = GRPSTATISTICSCHARTLEGENDPOSITION_BOTTOM;
                                        legendfontsize  = 12.0;

                                        hasbackground   = true;
                                        backgroundcolor.Set(255, 255, 255);
                                        axiscolor.Set      (60, 60, 60);
                                        gridcolor.Set      (220, 220, 220);
                                        textcolor.Set      (40, 40, 40);

                                        palette[0].Set( 79, 129, 189);
                                        palette[1].Set(192, 80, 77);
                                        palette[2].Set(155, 187, 89);
                                        palette[3].Set(128, 100, 162);
                                        palette[4].Set( 75, 172, 198);
                                        palette[5].Set(247, 150, 70);
                                        palette[6].Set(119, 119, 119);
                                        palette[7].Set(193, 165, 90);
                                      }

    XSTRING                         title;

    bool                            hasbackground;
    GRPSTATISTICSCHARTCOLOR         backgroundcolor;

    GRPSTATISTICSCHARTCOLOR         axiscolor;
    GRPSTATISTICSCHARTCOLOR         gridcolor;
    GRPSTATISTICSCHARTCOLOR         textcolor;

    double                          fontsize;
    double                          titlefontsize;

    bool                            showgrid;
    bool                            showvalues;

    bool                            showaxislabels;
    double                          axisfontsize;

    bool                            showlegend;
    GRPSTATISTICSCHARTLEGENDPOSITION legendposition;
    double                          legendfontsize;

    XSTRING                         fontfilename;

    double                          margin;

    GRPSTATISTICSCHARTCOLOR         palette[GRPSTATISTICSCHARTCONFIG_PALETTESIZE];
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




