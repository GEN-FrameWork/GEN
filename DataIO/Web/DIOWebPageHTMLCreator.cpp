/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebPageHTMLCreator.cpp
* 
* @class      DIOWEBPAGEHTMLCREATOR
* @brief      Data Input/Output Web Page HTML Create class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOWebPageHTMLCreator.h"

#include "XFactory.h"
#include "XFileTXT.h"
#include "XBuffer.h"
#include "XTrace.h"

#include "DIOWebServer.h"
#include "DIOWebPageHTMLCreator_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATOR::DIOWEBPAGEHTMLCREATOR()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR::DIOWEBPAGEHTMLCREATOR()
{
  Clean();

  RegisterEvent(DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_XTAGFOUND);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATOR::~DIOWEBPAGEHTMLCREATOR()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATOR::~DIOWEBPAGEHTMLCREATOR()
{
  DeRegisterEvent(DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_XTAGFOUND);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator = (const char* htlmpage)
* @brief      operator =
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator = (const char* htlmpage)
{
  Set(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator = (const XCHAR* htlmpage)
* @brief      operator =
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator = (const XCHAR* htlmpage)
{
  Set(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator = (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator =
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator = (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  Set(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator = (XSTRING& string)
* @brief      operator =
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator = (XSTRING& string)
{
  Set(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator = (XWORD* htlmpage)
* @brief      operator =
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage pointer to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator = (XWORD* htlmpage)
{
  Set(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator += (const char* htlmpage)
* @brief      operator +=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator += (const char* htlmpage)
{
  Add(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator += (const XCHAR* htlmpage)
* @brief      operator +=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator += (const XCHAR* htlmpage)
{
  Add(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator += (XPATH& htlmpage)
* @brief      operator +=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator += (XPATH& htlmpage)
{
  Add(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator += (XSTRING& string)
* @brief      operator +=
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator += (XSTRING& string)
{
  Add(string);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBPAGEHTMLCREATOR::operator += (XCHAR htlmpage)
* @brief      operator +=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBPAGEHTMLCREATOR::operator += (XCHAR htlmpage)
{
  Add(htlmpage);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator < (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator <
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator <  (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) == -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator > (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator >
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator >  (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) ==  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator <= (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator <=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator <= (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) !=  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator >= (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator >=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator >= (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) != -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator == (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator ==
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator == (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) ==  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator != (DIOWEBPAGEHTMLCREATOR& htlmpage)
* @brief      operator !=
* @ingroup    DATAIO
* 
* @param[in]  htlmpage : Htlmpage value.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator != (DIOWEBPAGEHTMLCREATOR& htlmpage)
{
  return (bool)(Compare(htlmpage) !=  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator < (XSTRING& string)
* @brief      operator <
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator <  (XSTRING& string)
{
  return (bool)(Compare(string) == -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator > (XSTRING& string)
* @brief      operator >
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator >  (XSTRING& string)
{
  return (bool)(Compare(string) ==  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator <= (XSTRING& string)
* @brief      operator <=
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator <= (XSTRING& string)
{
  return (bool)(Compare(string) !=  1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator >= (XSTRING& string)
* @brief      operator >=
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator >= (XSTRING& string)
{
  return (bool)(Compare(string) != -1);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator == (XSTRING& string)
* @brief      operator ==
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator == (XSTRING& string)
{
  return (bool)(Compare(string) ==  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::operator != (XSTRING& string)
* @brief      operator !=
* @ingroup    DATAIO
* 
* @param[in]  string : String to use.
* 
* @return     bool DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::operator != (XSTRING& string)
{
  return (bool)(Compare(string) !=  0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR DIOWEBPAGEHTMLCREATOR::operator [] (int position)
* @brief      operator []
* @ingroup    DATAIO
* 
* @param[in]  position : Position value.
* 
* @return     XCHAR DIOWEBPAGEHTMLCREATOR:: : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR DIOWEBPAGEHTMLCREATOR::operator [] (int position)
{
  if(IsEmpty())           return 0;
  if(position<0)          return text[0];
  if(position>=(int)size) return text[size-1];
  return text[position];
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Printf(XCHAR* mask, ...)
* @brief      Printf
* @ingroup    DATAIO
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Printf(XCHAR* mask, ...)
{
  XSTRING outstring;

  va_list arg;

  va_start(arg, mask);

  outstring.FormatArg(mask, &arg, true);

  va_end(arg);

  Add(outstring.Get());

  //--------------------------------------------------
  // To See better the sorce code of WEB page
  #ifdef DEBUG_ACTIVE
  //Add(_L("\n"));
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Print(XCHAR* text)
* @brief      Print
* @ingroup    DATAIO
* 
* @param[in]  text : Text to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Print(XCHAR* text)
{
  Add(text);

  //--------------------------------------------------
  // To See better the sorce code of WEB page
  #ifdef DEBUG_ACTIVE
  //Add(_L("\n"));
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Table_Ini(int border, XCHAR* addstyle)
* @brief      Table ini
* @ingroup    DATAIO
* 
* @param[in]  border : Border value.
* @param[in]  addstyle : Addstyle pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Ini(int border, XCHAR* addstyle)
{
  this->tableborder = border;

  Printf(_L("<table style=\"border-collapse: collapse; %s \">"), (addstyle?addstyle:_L("")));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Table_Line(int ncolumns, ...)
* @brief      Table line
* @ingroup    DATAIO
* 
* @param[in]  ncolumns : Ncolumns value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Line(int ncolumns, ...)
{
  va_list arg;

  va_start(arg, ncolumns);

  Printf(_L("<tr>"));

  for(int c=0; c<ncolumns; c++)
    {
      int                         width = va_arg(arg, int);
      DIOWEBPAGEHTMLCREATORALIGN  align = (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
      XCHAR*                      text  = va_arg(arg, XCHAR*);
      XSTRING                     alignstr;
      XSTRING                     textrev;

      textrev.FormatArg(text, &arg, true);

      alignstr.Empty();

      switch(align)
        {
          case DIOWEBPAGEHTMLCREATORALIGN_NONE   : alignstr = _L("");        break;
          case DIOWEBPAGEHTMLCREATORALIGN_LEFT   : alignstr = _L("left");    break;
          case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = _L("right");   break;
          case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = _L("center");  break;
        }

      Printf(_L("<td style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\">%s</td>"), tableborder, width, alignstr.Get(), textrev.Get());

    }

  Printf(_L("</tr>"));

  va_end(arg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Table_Line(XCHAR* colorbackground, int ncolumns, ...)
* @brief      Table line
* @ingroup    DATAIO
* 
* @param[in]  colorbackground : Colorbackground pointer to use.
* @param[in]  ncolumns : Ncolumns value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_Line(XCHAR* colorbackground, int ncolumns, ...)
{
  va_list arg;

  va_start(arg, ncolumns);

  XSTRING tr;
  XSTRING color;
  color = colorbackground;

  if(!color.IsEmpty())
    {
      tr = _L("<tr bgcolor=\"");
      tr += colorbackground;
      tr += "\">";
    }
   else tr = _L("<tr>");

  Printf(tr.Get());

  for(int c=0; c<ncolumns; c++)
    {
      int                         width = va_arg(arg, int);
      DIOWEBPAGEHTMLCREATORALIGN  align = (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
      XCHAR*                      text  = va_arg(arg, XCHAR*);
      XSTRING                     alignstr;
      XSTRING                     textrev;

      textrev.FormatArg(text, &arg, true);

      alignstr.Empty();

      switch(align)
        {
          case DIOWEBPAGEHTMLCREATORALIGN_NONE   : alignstr = _L("");        break;
          case DIOWEBPAGEHTMLCREATORALIGN_LEFT   : alignstr = _L("left");    break;
          case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = _L("right");   break;
          case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = _L("center");  break;
        }

      Printf(_L("<td style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\">%s</td>"), tableborder, width, alignstr.Get(), textrev.Get());
    }

  Printf(_L("</tr>"));

  va_end(arg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Table_LineColor(int ncolumns, ...)
* @brief      Table line color
* @ingroup    DATAIO
* 
* @param[in]  ncolumns : Ncolumns value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_LineColor(int ncolumns, ...)
{
  va_list arg;

  va_start(arg, ncolumns);

  Printf(_L("<tr>"));

  for(int c=0; c<ncolumns; c++)
    {
      int                         width     = va_arg(arg, int);
      XCHAR*                      colortxt  = va_arg(arg, XCHAR*);
      XCHAR*                      colorbkg  = va_arg(arg, XCHAR*);
      DIOWEBPAGEHTMLCREATORALIGN  align     = (DIOWEBPAGEHTMLCREATORALIGN)va_arg(arg, int);
      XCHAR*                      text      = va_arg(arg, XCHAR*);
      XSTRING                     alignstr;
      XSTRING                     textrev;

      textrev.FormatArg(text, &arg, true);

      alignstr.Empty();

      switch(align)
        {
          case DIOWEBPAGEHTMLCREATORALIGN_NONE   : alignstr = _L("");        break;
          case DIOWEBPAGEHTMLCREATORALIGN_LEFT   : alignstr = _L("left");    break;
          case DIOWEBPAGEHTMLCREATORALIGN_RIGHT  : alignstr = _L("right");   break;
          case DIOWEBPAGEHTMLCREATORALIGN_CENTER : alignstr = _L("center");  break;
        }

      Printf(_L("<td bgcolor=\"%s\" style=\"border: %dpx solid silver;\" width=\"%d\" align=\"%s\"><font color=\"%s\">%s</font></td>"), colorbkg, tableborder, width, alignstr.Get(), colortxt, textrev.Get());
    }

  Printf(_L("</tr>"));

  va_end(arg);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::Table_End()
* @brief      Table end
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::Table_End()
{
  Printf(_L("</table>"));
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::AddAutoRefresh(int refresh, XSTRING* url)
* @brief      Add auto refresh
* @ingroup    DATAIO
* 
* @param[in]  refresh : Refresh value.
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::AddAutoRefresh(int refresh, XSTRING* url)
{
  if(refresh<0) return false;

  XSTRING meta;

  if(url)
        meta.Format(_L("<META http-equiv=\"refresh\" content=\"%d;URL=%s\"/>"), refresh, url->Get());
   else meta.Format(_L("<META http-equiv=\"refresh\" content=\"%d;\"/>"), refresh);

  Add(meta.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBPAGEHTMLCREATOR::AddFile(XPATH& xpath)
* @brief      Add file
* @ingroup    DATAIO
* 
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBPAGEHTMLCREATOR::AddFile(XPATH& xpath)
{
  XFILETXT filetxt;
  bool     status = true;
  bool     intag  = false;

  if(!filetxt.Open(xpath)) return false;

  if(filetxt.ReadAllFile())
    {
      Empty();

      XBUFFER LFdata;

      filetxt.GetLF(LFdata);

      for(int c=0;c<filetxt.GetNLines();c++)
        {
          XSTRING* line = filetxt.GetLine(c);
          if(line)
            {
              XSTRING inioutput;
              XSTRING endoutput;
              int     result;

              result = CheckXTAG(line, intag, inioutput, endoutput);
              switch(result)
                {
                  case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND   : { if(!line->IsEmpty())
                                                                              {
                                                                                if(Add(*line)) Add(filetxt.GetLF()->Get());
                                                                              }
                                                                          }
                                                                          break;

                  case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND   : { if(!inioutput.IsEmpty())
                                                                              {
                                                                                if(Add(inioutput)) Add(filetxt.GetLF()->Get());
                                                                              }

                                                                            DIOWEBPAGEHTMLCREATOR_XEVENT xevent(this, DIOWEBPAGEHTMLCREATOR_XEVENT_TYPE_XTAGFOUND);

                                                                            xevent.SetPageHTMLCreator(this);
                                                                            xevent.SetXTAG(&lastXTAG);

                                                                            PostEvent(&xevent);

                                                                            if(!endoutput.IsEmpty())
                                                                              {
                                                                                if(Add(endoutput)) Add(filetxt.GetLF()->Get());
                                                                              }
                                                                          }
                                                                          break;

                  case DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND: { if(!inioutput.IsEmpty())
                                                                              {
                                                                                if(Add(inioutput)) Add(filetxt.GetLF()->Get());
                                                                              }

                                                                            if(!endoutput.IsEmpty())
                                                                              {
                                                                                if(Add(endoutput)) Add(filetxt.GetLF()->Get());
                                                                              }
                                                                          }
                                                                          break;
                }
            }
        }
    }

  filetxt.Close();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS DIOWEBPAGEHTMLCREATOR::CheckXTAG(XSTRING* originalline, bool& intag, XSTRING& inioutputline, XSTRING& endoutputline)
* @brief      Check XTAG
* @ingroup    DATAIO
* 
* @param[in]  originalline : Originalline pointer to use.
* @param[in]  intag : Intag value.
* @param[in]  inioutputline : Inioutputline value.
* @param[in]  endoutputline : Endoutputline value.
* 
* @return     DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS DIOWEBPAGEHTMLCREATOR::CheckXTAG(XSTRING* originalline, bool& intag, XSTRING& inioutputline, XSTRING& endoutputline)
{
  int index[3];

  inioutputline.Empty();
  endoutputline.Empty();

  if(!intag)
    {
      index[0] = originalline->Find(_L("<?"), true);
      if(index[0]!=XSTRING_NOTFOUND)
        {
          index[1] = originalline->Find(DIOWEBPAGEHTMLCREATOR_TAGXENGINE, true);
          if(index[1]!=XSTRING_NOTFOUND)
            {
              lastXTAG.Empty();

              originalline->Copy(0, index[0], inioutputline);

              index[2] = originalline->Find(_L("?>"), true);
              if(index[2]!=XSTRING_NOTFOUND)
                {
                  XSTRING endline;

                  originalline->Copy(index[1]+3, index[2], lastXTAG);
                  originalline->Copy(index[2]+2, endoutputline);

                  return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND;

                }
               else
                {
                  originalline->Copy(index[1]+3, lastXTAG);
                  intag = true;
                  return  DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND;
                }
            }
        }

      inioutputline = originalline->Get();
    }
   else
    {
      index[0] = originalline->Find(_L("?>"), true);
      if(index[0]!=XSTRING_NOTFOUND)
        {
          XSTRING TAGstring;

          originalline->Copy(0, index[0], TAGstring);
          lastXTAG+=TAGstring.Get();

          originalline->Copy(index[0]+2, endoutputline);

          intag = false;

          return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDEND;
        }

      lastXTAG += originalline->Get();

      inioutputline.Empty();
      endoutputline.Empty();

      return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_FOUNDNOTEND;
    }

  return DIOWEBPAGEHTMLCREATORTAGXENGINESTATUS_NOTFOUND;
}



