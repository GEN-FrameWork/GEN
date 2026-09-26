/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XLog.cpp
* 
* @class      XLOG
* @brief      eXtended Utils Log class
* @ingroup    XUTILS
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

#include "XLog.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "XFactory.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XDir.h"
#include "XFileTXT.h"
#include "XFileZIP.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XLOG* XLOG::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOGENTRY::XLOGENTRY()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOGENTRY::XLOGENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOGENTRY::~XLOGENTRY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOGENTRY::~XLOGENTRY()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLOGENTRY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLOGENTRY::Clean()
{
  position  = 0;
  size      = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOGBASE::XLOGBASE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOGBASE::XLOGBASE()
{
  Clean();

  xdatetime=GEN_XFACTORY.CreateDateTime();
  xtimer=GEN_XFACTORY.CreateTimer();
  mutex=GEN_XFACTORY.Create_Mutex();
  
  filelog = GEN_NEW XFILETXT();
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOGBASE::~XLOGBASE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOGBASE::~XLOGBASE()
{
  if(filelog)     filelog->Close();
  if(filelog)     GEN_DELETE filelog;
  if(mutex)       GEN_XFACTORY.Delete_Mutex(mutex);
  if(xtimer)      GEN_XFACTORY.DeleteTimer(xtimer);
  if(xdatetime)   GEN_XFACTORY.DeleteDateTime(xdatetime);

  this->entrys.DeleteContents();
  this->entrys.DeleteAll();

  Clean();
}
  

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Ini(XCHAR* pathfile, XCHAR* nameapplication, bool isdumpintrace)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  pathfile : Pathfile pointer to use.
* @param[in]  nameapplication : Nameapplication pointer to use.
* @param[in]  isdumpintrace : Isdumpintrace value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Ini(XCHAR* pathfile, XCHAR* nameapplication, bool isdumpintrace)
{
  if(isactive) return false;

  this->xpathfile     = pathfile;
  this->isdumpintrace = isdumpintrace;

  if(nameapplication) this->nameapplication = nameapplication;

  if(!filelog) return false;

  bool status = filelog->Open(xpathfile, false);
  if(!status) status = filelog->Create(xpathfile);

  if(status) CalculateInitialStatus();

  isactive = status;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Ini(XPATH& xpathfile, XCHAR* nameapplication, bool isdumpintrace)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  xpathfile : Xpathfile value.
* @param[in]  nameapplication : Nameapplication pointer to use.
* @param[in]  isdumpintrace : Isdumpintrace value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Ini(XPATH& xpathfile, XCHAR* nameapplication, bool isdumpintrace)
{
  return Ini(xpathfile.Get(), nameapplication, isdumpintrace);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Ini(XPATH& xpathfile, XSTRING& nameapplication, bool isdumpintrace)
* @brief      Initialize the object
* @ingroup    XUTILS
* 
* @param[in]  xpathfile : Xpathfile value.
* @param[in]  nameapplication : Nameapplication value.
* @param[in]  isdumpintrace : Isdumpintrace value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Ini(XPATH& xpathfile, XSTRING& nameapplication, bool isdumpintrace)
{
  return Ini(xpathfile.Get(), nameapplication.Get(), isdumpintrace);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::SetLimit(XLOGTYPELIMIT typelimit, int limit, int reductionpercent)
* @brief      Set limit
* @ingroup    XUTILS
* 
* @param[in]  typelimit : Typelimit value.
* @param[in]  limit : Limit value.
* @param[in]  reductionpercent : Reductionpercent value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::SetLimit(XLOGTYPELIMIT typelimit, int limit, int reductionpercent)
{
  this->typelimit        = typelimit;
  this->limit            = limit;
  this->reductionpercent = reductionpercent;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::SetBackup(bool isactive, int maxfiles, bool iscompress)
* @brief      Set backup
* @ingroup    XUTILS
* 
* @param[in]  isactive : Isactive value.
* @param[in]  maxfiles : Maxfiles value.
* @param[in]  iscompress : Iscompress value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::SetBackup(bool isactive, int maxfiles, bool iscompress)
{
  this->backupisactive    = isactive;
  this->backupmaxfiles    = maxfiles;
  this->backupiscompress  = iscompress;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::SetFilters(XCHAR* sectionsID, XBYTE level)
* @brief      Set filters
* @ingroup    XUTILS
* 
* @param[in]  sectionsID : Sections ID pointer to use.
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::SetFilters(XCHAR* sectionsID, XBYTE level)
{
  if(sectionsID) filtersectionsID = sectionsID;
  filterlevel = level;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XLOGBASE::GetPathFile()
* @brief      Get path file
* @ingroup    XUTILS
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XLOGBASE::GetPathFile()
{ 
  return xpathfile.Get();                 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLOGBASE::GetNameApplication()
* @brief      Get name application
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLOGBASE::GetNameApplication()
{ 
  return &nameapplication;                
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLOGBASE::GetPrevLabel()
* @brief      Get prev label
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLOGBASE::GetPrevLabel()                                                                    
{ 
  return &prevlabel;                      
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::IsActive()
* @brief      Is active
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::IsActive()
{
  if(!filelog)            return false;
  if(!filelog->IsOpen())  return false;
  if(!isactive)           return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLOGBASE::SetIsActive(bool isactive)
* @brief      Set is active
* @ingroup    XUTILS
* 
* @param[in]  isactive : Isactive value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLOGBASE::SetIsActive(bool isactive)                                                 
{ 
  this->isactive = isactive;              
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLOGBASE::GetNEntrys()
* @brief      Get N entrys
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLOGBASE::GetNEntrys()
{ 
  return nentrys;                         
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XLOGBASE::GetNLines()
* @brief      Get N lines
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XLOGBASE::GetNLines()
{ 
  return nlines;                          
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XLOGBASE::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XLOGBASE::GetSize()                                                                    
{ 
  return size;                            
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::GetLevelString(XLOGLEVEL level, XSTRING& strlevel, XDWORD size)
* @brief      Get level string
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  strlevel : Strlevel value.
* @param[in]  size : Size value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::GetLevelString(XLOGLEVEL level, XSTRING& strlevel, XDWORD size)
{
  strlevel.Empty();

  switch(level)
    {
      case XLOGLEVEL_INFO     : strlevel = __L("INFO");       break;
      case XLOGLEVEL_WARNING  : strlevel = __L("WARNING");    break;
      case XLOGLEVEL_ERROR    : strlevel = __L("ERROR");      break;
      default: break;
    }

  if(strlevel.IsEmpty()) return false;

  if(size>0) strlevel.AdjustSize(size, false, __L(" "));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XLOGBASE::GetSectionsIDFilter()
* @brief      Get sections ID filter
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XLOGBASE::GetSectionsIDFilter()
{
  return &filtersectionsID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::PassFilters(XLOGLEVEL level, XCHAR* sectionID)
* @brief      Pass filters
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  sectionID : Section ID pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::PassFilters(XLOGLEVEL level, XCHAR* sectionID)
{
  if(!(filterlevel&level)) return false;

  if(sectionID) lastsectionID = sectionID;

  if(filtersectionsID.IsEmpty())  return false;

  if(filtersectionsID.Find(sectionID?sectionID:lastsectionID.Get(), true) == XSTRING_NOTFOUND)  return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XCHAR* mask, ...)
* @brief      Add entry
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  sectionID : Section ID pointer to use.
* @param[in]  inmemory : Inmemory value.
* @param[in]  mask : Mask pointer to use.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XCHAR* mask, ...)
{
  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);

  if(!IsActive()) return false;
  if(!mask)       return false;
  if(!mutex)      return false;

  mutex->Lock();

  if(sectionID)
    {
      if(sectionID[0])
        {
          if(!PassFilters(level, sectionID))
            {
              mutex->UnLock();
              return false;
            }
        }
    }

  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);

  Backup_ControlLimits();

  XSTRING outstring;
  XSTRING outstringtrace;
  XSTRING string;

  XSTRING datestring;
  XSTRING levelstring;

  if(xdatetime)
    {
      xdatetime->Read();
      xdatetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datestring);
    }

  GetLevelString(level, levelstring, 7);
  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);
  if(sectionID)
    {
      string = datestring;
      string.AdjustSize(XLOG_MAXDATETIMESIZE, false, __L(" "));
      outstring      += string;      

      string = levelstring.Get();
      string.AdjustSize(XLOG_MAXLEVELSIZE, false, __L(" "));
      outstring      += string;
      outstringtrace += string;

      string = sectionID;
      string.AdjustSize(XLOG_MAXSECTIONIDSIZE, false, __L(" "));
    }
   else
    {
      string = __L(" ");
      string.AdjustSize(XLOG_MAXDATETIMESIZE + XLOG_MAXLEVELSIZE + XLOG_MAXSECTIONIDSIZE, false, __L(" "));
    }
  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);
  outstring      += string;
  outstringtrace += string;
  outstring      += __L(" ");
  outstringtrace += __L(" ");

 if(!prevlabel.IsEmpty())
   {
     outstring += prevlabel.Get();
   }

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  outstring      += string;
  outstringtrace += string;

  if(isdumpintrace)
    {
      int leveldebug = 0;

      switch(level)
        {
          case XLOGLEVEL_WARNING  : leveldebug = 3; break;
          case XLOGLEVEL_ERROR    : leveldebug = 4; break;

          default:
          case XLOGLEVEL_INFO     : leveldebug = 0; break;
        }

     XTRACE_PRINTCOLOR(leveldebug , __L("[Log] %s "), outstringtrace.Get());
    }

  if(inmemory)
    {
      filelog->AddLine(outstring);
    }
   else
    {
      XDWORD resultsizeline = 0;
      XDWORD resultsizeLF   = 0;

      if(filelog->GetNLines()) FlushMemoryEntrys();

      if(outstring.Get()[0] != 0x20)
        {
          XQWORD position;

          filelog->GetPrimaryFile()->GetPosition(position);

          nentrys++;

          lastentry = GEN_NEW XLOGENTRY();
          if(lastentry)
            {
              lastentry->position = position;
              entrys.Add(lastentry);
            }
        }

      filelog->AddLineAlready(outstring.Get(), &resultsizeline, &resultsizeLF);

      if(lastentry) 
        {
          lastentry->size += (resultsizeline + resultsizeLF);
        }

      size += (resultsizeline + resultsizeLF);

      nlines++;
    }

  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);
  mutex->UnLock();
  //XTRACE_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBYTE* data, XDWORD size, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Add entry
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  sectionID : Section ID pointer to use.
* @param[in]  inmemory : Inmemory value.
* @param[in]  data : Data buffer to use.
* @param[in]  size : Size value.
* @param[in]  sizeline : Sizeline value.
* @param[in]  showoffset : Showoffset value.
* @param[in]  showtext : Showtext value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBYTE* data, XDWORD size, XDWORD sizeline, bool showoffset, bool showtext)
{
  if(!IsActive()) return false;
  if(!mutex)      return false;

  mutex->Lock();

  if(!PassFilters(level, sectionID))
    {
      mutex->UnLock();
      return false;
    }

  XSTRING    strdata;
  XDWORD      _size     = 0;
  XDWORD      _sizeline = sizeline;
  int        index     = 0;
  XDWORD     lines     = 0;

  while(_size<size)
    {
      XSTRING string;

      if(_sizeline > (size - _size)) _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(__L("%04X"), index);
          string += strdata;
          string += __L(" ");
        }

      for(XDWORD c=0; c<_sizeline; c++)
        {
          strdata.Format(__L("%02X "), data[index]);
          string += strdata;
          _size++;
          index++;
        }

      if(_sizeline != sizeline)
        {
          for(XDWORD c=0; c<(sizeline-_sizeline); c++)
            {
              strdata.Format(__L(" "));
              string += strdata;
            }
        }

      if(showtext)
        {
          index -= _sizeline;
          string += __L(" ");

          for(XDWORD c=0; c<_sizeline; c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==__C('%')) || (character==__C('\\'))) character = __C('.');

              strdata.Format(__L("%c"), character);
              string += strdata;
              index++;
            }
        }

      mutex->UnLock();

      AddEntry(level, lines?NULL:sectionID, inmemory, string.Get());

      mutex->Lock();

      lines++;
    }

  mutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBUFFER& data, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Add entry
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  sectionID : Section ID pointer to use.
* @param[in]  inmemory : Inmemory value.
* @param[in]  data : Data buffer to use.
* @param[in]  sizeline : Sizeline value.
* @param[in]  showoffset : Showoffset value.
* @param[in]  showtext : Showtext value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBUFFER& data, XDWORD sizeline, bool showoffset, bool showtext)
{
  return AddEntry(level, sectionID, inmemory, data.Get(), data.GetSize(), sizeline, showoffset, showtext);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Flush()
* @brief      Flush
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Flush()
{
  if(!mutex)      return false;
  if(!IsActive()) return false;

  mutex->Lock();

  if(filelog->GetNLines()) FlushMemoryEntrys();

  mutex->UnLock();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::End(bool doflush)
* @brief      End the object
* @ingroup    XUTILS
* 
* @param[in]  doflush : Doflush value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::End(bool doflush)
{
  if(!isactive) return false;

  if(doflush) Flush();

  if(!filelog) return false;

  filelog->Close();

  isactive = false;

  nentrys = 0;
  nlines  = 0;
  size    = 0;

  entrys.DeleteContents();
  entrys.DeleteAll();

  lastentry = NULL;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::CalculateInitialStatus()
* @brief      Calculate initial status
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::CalculateInitialStatus()
{
  XFILE* xfile = filelog->GetPrimaryFile();
  if(!xfile) return false;

  nlines  = 0;
  nentrys = 0;
  size    = xfile->GetSize();

  entrys.DeleteContents();
  entrys.DeleteAll();

  XDWORD             sizeBOM              = 0;
  XFILETXTFORMATCHAR formatchar           = filelog->GetFormatCharFromFile(&sizeBOM);
  int                sizebytescharacter   = filelog->SizeOfCharacter(formatchar);

  xfile->SetPosition(sizeBOM);

  bool    endfile;
  XDWORD  br;
  XBUFFER dataline;
  XBYTE*  readbuffer = GEN_NEW XBYTE[XFILETXT_MAXBUFFER];
  if(!readbuffer) return false;

  memset(readbuffer, 0, XFILETXT_MAXBUFFER);

  do{ XDWORD  bufferpos = 0;

      br      = XFILETXT_MAXBUFFER;
      endfile = !xfile->Read(readbuffer, &br);
      if(!br) break;

      do{ XFILETXTTYPELF typeLF;
          XDWORD         sizeLF   = 0;
          XDWORD         sizeline = 0;
          bool           endline = filelog->GetSizeOfLine(formatchar, &readbuffer[bufferpos], typeLF, sizeLF, sizeline, (br-bufferpos));

          if(endline)
            {
              if(readbuffer[bufferpos] != 0x20)
                {
                  nentrys++;

                  lastentry = GEN_NEW XLOGENTRY();
                  if(lastentry)
                    {
                      lastentry->position = bufferpos;
                      entrys.Add(lastentry);
                    }
                }

              if(lastentry) lastentry->size += (sizeline + sizeLF);
              nlines++;

              sizeline  *= sizebytescharacter;
              sizeline  += (sizeLF*sizebytescharacter);
              bufferpos += sizeline;
            }
           else
            {
              int lack = (br-bufferpos);
              bufferpos += lack;

              if(endfile && (bufferpos == br))
                {
                  if(readbuffer[bufferpos] != 0x20)
                    {
                      nentrys++;

                      lastentry = GEN_NEW XLOGENTRY();
                      if(lastentry)
                        {
                          lastentry->position = bufferpos;
                          entrys.Add(lastentry);
                        }
                    }

                  if(lastentry) 
                    {
                      lastentry->size += lack;
                    }

                  nlines++;
                }
            }

        } while(bufferpos < br);

    } while(!endfile);

  GEN_DELETE_ARRAY readbuffer;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::FlushMemoryEntrys()
* @brief      Flush memory entrys
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::FlushMemoryEntrys()
{
  int c=0;
  while(c<filelog->GetNLines())
    {
      XSTRING* line = filelog->GetLine(c);
      if(line)
        {
          XDWORD resultsizeline = 0;
          XDWORD resultsizeLF   = 0;

          if(line->Get()[0] != 0x20)
            {
              XQWORD position;

              filelog->GetPrimaryFile()->GetPosition(position);

              nentrys++;

              lastentry = GEN_NEW XLOGENTRY();
              if(lastentry)
                {
                  lastentry->position = position;
                  entrys.Add(lastentry);
                }
            }

          filelog->AddLineAlready(line->Get(), &resultsizeline, &resultsizeLF);

          if(lastentry) 
            {
              lastentry->size += (resultsizeline + resultsizeLF);
            }

          size += (resultsizeline + resultsizeLF);

          nlines++;
        }

      c++;
    }

  filelog->DeleteAllLines();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XLOGBASE::Backup_GetNFiles()
* @brief      Backup get N files
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XLOGBASE::Backup_GetNFiles()
{
  XPATH         xpathnamelog;
  XPATH         xpath;
  int           nfiles = 0;
  XDIRELEMENT   xdirelement;

  xpathnamelog = xpathfile.Get();

  xpathnamelog.GetPath(xpath);

  XDIR* xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return 0;

  if(xdir->FirstSearch(xpath.Get(), XLOG_SEARCHMASKBACKUPFILE , &xdirelement))
    {
      do{ if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE) nfiles++;

        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  return nfiles;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Backup_GetNameFileMoreOld(XPATH& xpathselect)
* @brief      Backup get name file more old
* @ingroup    XUTILS
* 
* @param[in]  xpathselect : Xpathselect value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Backup_GetNameFileMoreOld(XPATH& xpathselect)
{
  if(!xdatetime) return false;

  XPATH         xpathnamelog;
  XPATH         xpath;
  XPATH         selectnamefile;
  XDIRELEMENT   xdirelement;
  XQWORD        lastseconds = 0x8FFFFFFFFFFFFFFFULL;

  xpathnamelog = xpathfile.Get();

  xpathnamelog.GetPath(xpath);

  XDIR* xdir=GEN_XFACTORY.Create_Dir();
  if(!xdir) return false;

  if(xdir->FirstSearch(xpath.Get(), XLOG_SEARCHMASKBACKUPFILE, &xdirelement))
    {
      do{ if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE)
            {
              XSTRING* namefile = xdirelement.GetNameFile();
              if(namefile)
                {
                  int day     = 0;
                  int month   = 0;
                  int year    = 0;
                  int hours   = 0;
                  int minutes = 0;
                  int seconds = 0;

                  namefile->UnFormat(XLOG_CREATEMASKBACKUPFILE, &year, &month, &day, &hours, &minutes, &seconds);

                  xdatetime->SetDay(day);
                  xdatetime->SetMonth(month);
                  xdatetime->SetYear(year);
                  xdatetime->SetHours(hours);
                  xdatetime->SetMinutes(minutes);
                  xdatetime->SetSeconds(seconds);

                  if(lastseconds > xdatetime->GetSeconsFromDate())
                    {
                      lastseconds = xdatetime->GetSeconsFromDate();
                      selectnamefile = xdirelement.GetNameFile()->Get();
                    }
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  GEN_XFACTORY.Delete_Dir(xdir);

  xpathselect  = xpath;
  xpathselect += selectnamefile;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Backup_CreateNameZIP(XPATH& xpathzipfile)
* @brief      Backup create name ZIP
* @ingroup    XUTILS
* 
* @param[in]  xpathzipfile : Xpathzipfile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Backup_CreateNameZIP(XPATH& xpathzipfile)
{
  xpathzipfile.Empty();

  xpathzipfile  = xpathfile.Get();
  xpathzipfile.SetOnlyPathAndNamefile();
  xpathzipfile += __L(".zip");
  xpathzipfile.AddToNameFile(true, XLOG_PREFIX);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Backup_AdjustNFilesInCompressed()
* @brief      Backup adjust N files in compressed
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Backup_AdjustNFilesInCompressed()
{
  XFILEUNZIP xfileunzip;
  XPATH      xpathzipfile;

  Backup_CreateNameZIP(xpathzipfile);

  if(xfileunzip.Open(xpathzipfile))
    {
      if(backupmaxfiles)
        {
          if(xfileunzip.GetNFiles()>=backupmaxfiles)
            {
              xfileunzip.DelFile(0);
            }
        }

      xfileunzip.Close();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Backup_AdjustNFiles(XCHAR* pathnamelog, bool iscompress)
* @brief      Backup adjust N files
* @ingroup    XUTILS
* 
* @param[in]  pathnamelog : Pathnamelog pointer to use.
* @param[in]  iscompress : Iscompress value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Backup_AdjustNFiles(XCHAR* pathnamelog, bool iscompress)
{
  XFILE* xfilebackup=GEN_XFACTORY.Create_File();
  if(!xfilebackup)  return false;

  XPATH    xpathnamelog;
  XSTRING  datestr;
  bool     status      = false;
  //int      indexbackup = 0;

  xpathnamelog = pathnamelog;

  if(!iscompress)
    {

      if(backupmaxfiles)
        {
          if(Backup_GetNFiles()>backupmaxfiles)
            {
              Backup_GetNameFileMoreOld(xpathnamelog);
              xfilebackup->Erase(xpathnamelog);
            }
        }
    }
   else
    {
      Backup_AdjustNFilesInCompressed();
    }

  if(iscompress)
    {
      XFILEZIP* xfilezip = GEN_NEW XFILEZIP();
      if(xfilezip)
        {
          XPATH xpathzipfile;

          Backup_CreateNameZIP(xpathzipfile);

          if(xfilezip->Open(xpathzipfile))
            {
              XPATH namefileinzip;

              xpathnamelog.GetNamefileExt(namefileinzip);

              if(!xfilezip->AddFile(xpathnamelog, namefileinzip)) status = false;

              xfilezip->Close();
            }

          GEN_DELETE xfilezip;

          status = xfilebackup->Erase(pathnamelog);
        }
    }

  GEN_XFACTORY.Delete_File(xfilebackup);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOGBASE::Backup_ControlLimits()
* @brief      Backup control limits
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOGBASE::Backup_ControlLimits()
{
  if(!IsActive()) return false;

  if(!limit) return false;

  switch(typelimit)
    {
      case XLOGTYPELIMIT_NENTRYS  : if(nentrys<limit)   return false;
                                    break;

      case XLOGTYPELIMIT_SIZE     : if(size<limit)      return false;
                                    break;
      default: break;
    }

  Flush();

  bool status = true;

  if(backupisactive)
    {
      XPATH    xpathnamelog;
      XSTRING  datestr;

      //  If have backup DONT HAVE reduction
      //  The actual log file start
      xpathnamelog       = xpathfile.Get();
      backupxpathnamelog = xpathfile.Get();

      xdatetime->Read();
      datestr.Format(XLOG_CREATEMASKBACKUPFILE, xdatetime->GetYear(), xdatetime->GetMonth(), xdatetime->GetDay(), xdatetime->GetHours(), xdatetime->GetMinutes(), xdatetime->GetSeconds());

      backupxpathnamelog.AddToNameFile(true, datestr.Get());

      End(false);

      XFILE* xfile=GEN_XFACTORY.Create_File();
      if(xfile)
        {
          xfile->Rename(xpathnamelog, backupxpathnamelog);
          GEN_XFACTORY.Delete_File(xfile);
        }

      status = Ini(xpathnamelog, NULL, isdumpintrace);
      if(status) status = Backup_AdjustNFiles(backupxpathnamelog.Get(), backupiscompress);
    }
   else
    {
      // Reduction control: NOT Backup.

      XDWORD nentrystoerase = nentrys - ((nentrys * reductionpercent) / 100);
      XQWORD sizetocopy = 0;

      for(int c=0; c<(int)nentrystoerase; c++)
        {
          XLOGENTRY* entry = entrys.Get(c);
          if(entry) sizetocopy += entry->size;
        }

      if(!sizetocopy) return false;

      filelog->GetPrimaryFile()->SetPosition(0);
      filelog->GetPrimaryFile()->EraseBlock(sizetocopy);
    }

  if(status) status = CalculateInitialStatus();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XLOGBASE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XLOGBASE::Clean()
{
  xdatetime        = NULL;
  xtimer           = NULL;
  mutex            = NULL;
  filelog          = NULL;

  isdumpintrace    = false;

  isactive         = false;
  typelimit        = XLOGTYPELIMIT_NONE;

  backupisactive   = false;
  backupmaxfiles   = 0;
  backupiscompress = false;

  limit            = 0;
  reductionpercent = XLOG_DEFAULTREDUCTIONPERCENT;

  lastsectionID.Empty();

  filtersectionsID.Empty();
  filterlevel      = XLOGLEVEL_ALL;

  nentrys          = 0;
  nlines           = 0;
  size             = 0;
  lastentry        = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOG::GetIsInstanced()
* @brief      Get is instanced
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOG::GetIsInstanced()
{
  return instance!=NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOG& XLOG::GetInstance()
* @brief      Get instance
* @ingroup    XUTILS
* 
* @return     XLOG& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOG& XLOG::GetInstance()
{
  if(!instance) instance = GEN_NEW XLOG();

  return (*instance);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XLOG::DelInstance()
* @brief      Del instance
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XLOG::DelInstance()
{
  if(instance)
    {
      GEN_DELETE instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOG::XLOG() : XLOGBASE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOG::XLOG() : XLOGBASE()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XLOG::~XLOG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XLOG::~XLOG()
{
  
}




