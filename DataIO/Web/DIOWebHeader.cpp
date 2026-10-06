/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebHeader.cpp
* 
* @class      DIOWEBHEADER
* @brief      Data Input/Output Web Server Header class
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

#include "DIOWebHeader.h"

#include "XFactory.h"
#include "XSleep.h"
#include "XTrace.h"
#include "XTimer.h"

#include "DIOStreamTCPIP.h"
#include "DIOWebServer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBHEADER::DIOWEBHEADER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBHEADER::DIOWEBHEADER()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBHEADER::~DIOWEBHEADER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBHEADER::~DIOWEBHEADER()
{
  DeleteAllLines();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::Read(DIOSTREAMTCPIP* diostream, int timeout)
* @brief      Read
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* @param[in]  timeout : Timeout value.
* @param[in]  maximumsize : Maximumsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::Read(DIOSTREAMTCPIP* diostream, int timeout, XDWORD maximumsize)
{
  if(!diostream || !maximumsize) return false;

  XSTRING line;
  bool    status;
  bool    completed = false;
  XDWORD  totalsize = 0;

  DeleteAllLines();

  XTIMER* xtimerout=GEN_XFACTORY.CreateTimer();
  if(!xtimerout) return false;

  do{ status =  diostream->ReadStr(line);
      if(status)
        {
          if(line.IsEmpty())
            {
              completed = true;
              break;
            }
            else
            {
              XDWORD linesize = line.GetSize() + 2;
              if((line.GetSize() > DIOWEBHEADER_MAXLINE) || (linesize > maximumsize) ||
                 (totalsize > maximumsize - linesize))
                {
                  completed = false;
                  break;
                }

              totalsize += linesize;
              //XTRACE_PRINTCOLOR(3, _L("WEB CLIENT HEADER LINE: %s"), line.Get());
              if(!AddLine(line)) break;
            }
        }

      if(diostream->GetInXBuffer()->GetSize() > maximumsize)
        {
          completed = false;
          break;
        }

      if(timeout)
        {
           if(xtimerout->GetMeasureSeconds()>=(XDWORD)timeout) break;

          GEN_XSLEEP.MilliSeconds(1);
        }

    } while((diostream->GetStatus() == DIOSTREAMSTATUS_CONNECTED) || diostream->GetInXBuffer()->GetSize());

  GEN_XFACTORY.DeleteTimer(xtimerout);


  return completed && !lines.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::Write(DIOSTREAMTCPIP* diostream, int timeout)
* @brief      Write
* @ingroup    DATAIO
* 
* @param[in]  diostream : Diostream pointer to use.
* @param[in]  timeout : Timeout value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::Write(DIOSTREAMTCPIP* diostream, int timeout)
{
  if(!diostream) return false;

  for(int c=0;c<(int)lines.GetSize();c++)
    {
      XSTRING* line = (XSTRING*)lines.Get(c);
      if(line)
        {
          //XTRACE_PRINTCOLOR(2, _L("%s"), line.Get());

          diostream->WriteStr((*line));
          diostream->WriteStr(_L("\r\n"));
        }
    }

  diostream->WriteStr(_L("\r\n"));
  diostream->WaitToFlushOutXBuffer(timeout);

  if(diostream->GetStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::AddLine(XCHAR* line)
* @brief      Add line
* @ingroup    DATAIO
* 
* @param[in]  line : Line pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::AddLine(XCHAR* line)
{
  XSTRING* _line;

  _line  = GEN_NEW XSTRING();
  if(!_line) return false;

  (*_line) = line;

  lines.Add(_line);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::AddLine(XSTRING& line)
* @brief      Add line
* @ingroup    DATAIO
* 
* @param[in]  line : Line value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::AddLine(XSTRING& line)
{
  return AddLine(line.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XSTRING*>* DIOWEBHEADER::GetLines()
* @brief      Get lines
* @ingroup    DATAIO
* 
* @return     XVECTOR<XSTRING*>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>* DIOWEBHEADER::GetLines()
{
  return &lines;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::GetLines(XSTRING& result)
* @brief      Get lines
* @ingroup    DATAIO
* 
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::GetLines(XSTRING& result)
{
  result.Empty();

  if(!lines.GetSize()) return false;

  for(int c=0;c<(int)lines.GetSize();c++)
    {
      XSTRING* line = (XSTRING*)lines.Get(c);
      if(line)
        {
          result += line->Get();
          result += _L("\r\n");
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOWEBHEADER::GetFieldValue(XCHAR* field)
* @brief      Get field value
* @ingroup    DATAIO
* 
* @param[in]  field : Field pointer to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOWEBHEADER::GetFieldValue(XCHAR* field)
{
  if(!field || !field[0]) return NULL;
  XDWORD fieldsize = XSTRING::GetSize(field);
  bool fieldhascolon = false;
  for(XDWORD c=0; c<fieldsize; c++) if(field[c] == _C(':')) { fieldhascolon = true; break; }

  for(int c=0;c<(int)lines.GetSize();c++)
    {
       XSTRING* line = (XSTRING*)lines.Get(c);
       if(line && line->GetSize() >= fieldsize)
         {
           bool equal = true;
           for(XDWORD d=0; d<fieldsize; d++)
             {
               XCHAR left = (*line)[d];
               XCHAR right = field[d];
               if(left >= _C('A') && left <= _C('Z')) left += _C('a') - _C('A');
               if(right >= _C('A') && right <= _C('Z')) right += _C('a') - _C('A');
               if(left != right) { equal = false; break; }
             }

           if(!equal || (!fieldhascolon && (line->GetSize() == fieldsize || (*line)[fieldsize] != _C(':')))) continue;

           XDWORD position = fieldsize + (fieldhascolon?0:1);
           while(position < line->GetSize() && ((*line)[position] == _C(' ') || (*line)[position] == _C('\t'))) position++;
           return line->Get() + position;
         }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOWEBHEADER::GetFieldValue(XSTRING& field)
* @brief      Get field value
* @ingroup    DATAIO
* 
* @param[in]  field : Field value.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOWEBHEADER::GetFieldValue(XSTRING& field)
{
  return GetFieldValue(field.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::DeleteAllLines()
* @brief      Delete all lines
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::DeleteAllLines()
{
  if(lines.IsEmpty()) return false;

  lines.DeleteContents();

  lines.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBHEADER::GetFormat(bool isserver, int type, void* data)
* @brief      Get format
* @ingroup    DATAIO
* 
* @param[in]  isserver : Isserver value.
* @param[in]  type : Type value.
* @param[in]  data : Data buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBHEADER::GetFormat(bool isserver, int type, void* data)
{
  if(lines.IsEmpty()) return false;

  if(!data) return false;

  XSTRING line;
  int     version    = 0;
  int     subversion = 0;

  line =((XSTRING*)lines.Get(0))->Get();

  if(isserver)
    {
      int     code;
      XSTRING codestring(DIOWEBHEADER_MAXLINE);

      line.UnFormat(_L("HTTP/%d.%d %d %s"), &version, &subversion, &code, codestring.Get());
      codestring.AdjustSize();

      switch(type)
        {
          case  0: (*(int*)data)     = version;     break;
          case  1: (*(int*)data)     = subversion;  break;
          case  2: (*(int*)data)     = code;        break;
          case  3: (*(XSTRING*)data) = codestring;  break;
        }
    }
   else
    {
      XSTRING method(DIOWEBHEADER_MAXLINE);
      XSTRING resource(DIOWEBHEADER_MAXLINE);

      if(line.GetSize())
        {
          int foundindex = line.Find(_L("HTTP/"), true);

          if(foundindex != XSTRING_NOTFOUND) line.UnFormat(_L("%s %s HTTP/%d.%d"), method.Get(), resource.Get(), &version, &subversion);
        }

      method.AdjustSize();
      resource.AdjustSize();

      switch(type)
        {
          case  0: if(data) (*(XSTRING*)data) = method;     break;
          case  1: if(data) (*(XSTRING*)data) = resource;   break;
          case  2: if(data) (*(int*)data)     = version;    break;
          case  3: if(data) (*(int*)data)     = subversion; break;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBHEADER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBHEADER::Clean()
{

}
