/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XBuffer.cpp
* 
* @class      XBUFFER
* @brief      eXtended Utils Buffer functions class
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

#include "XBuffer.h"

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XRand.h"
#include "XTimer.h"
#include "XVariant.h"
#include "XSystem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

bool XBUFFER::globalhardwareuselittleendian = true;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER::XBUFFER(bool threadsafe)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  threadsafe : Threadsafe value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER::XBUFFER(bool threadsafe)

{
  Clean();

  localhardwareuselittleendian = globalhardwareuselittleendian;

  if(threadsafe) 
    {
      xmutex=GEN_XFACTORY.Create_Mutex();
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER::XBUFFER(XDWORD size, bool threadsafe)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  size : initial size of the buffer
* @param[in]  threadsafe : true is thread safe operative buffer
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER::XBUFFER(XDWORD size, bool threadsafe)
{
  Clean();

  localhardwareuselittleendian = globalhardwareuselittleendian;

  if(threadsafe) 
    {
      xmutex=GEN_XFACTORY.Create_Mutex();
    }

  Resize(size);
  FillBuffer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER::XBUFFER(const XBUFFER& xbuffer)
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER::XBUFFER(const XBUFFER& xbuffer)
{
  Clean();

  localhardwareuselittleendian = globalhardwareuselittleendian;

  xmutex=GEN_XFACTORY.Create_Mutex();

  CopyFrom((XBUFFER&)xbuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER::~XBUFFER()
* @brief      Destructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER::~XBUFFER()
{
  Delete();

  if(xmutex) 
    {
      GEN_XFACTORY.Delete_Mutex(xmutex);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::GetSize()
* @brief      Get size
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::GetSize()
{
  SetBlocked(true);

  XDWORD rsize = size;

  SetBlocked(false);

  return rsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::SetSize(XDWORD size)
* @brief      Set size
* @ingroup    XUTILS
* 
* @param[in]  size : GEN_NEW size of buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetSize(XDWORD size)
{
  SetBlocked(true);

  this->size = size;

  SetBlocked(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::SetAddBlockMemSize(XDWORD addblockmemsize)
* @brief      set add block mem size
* @ingroup    XUTILS
* 
* @param[in]  addblockmemsize : Addblockmemsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetAddBlockMemSize(XDWORD addblockmemsize)
{
  if(!addblockmemsize)
    {
      addblockmemsize = XBUFFER_ADDBLOCKMEMSIZE_DEFAULT;

      return false;
    }

  this->addblockmemsize = addblockmemsize;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::IsEmpty()
* @brief      Is empty
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::IsEmpty()
{
  return (!GetSize())?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBUFFER::ResetPosition()
* @brief      Reset position
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::ResetPosition()
{
  SetPosition(0);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::GetPosition()
* @brief      Get position
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::GetPosition()
{
  return position;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::SetPosition(XDWORD position)
* @brief      Set position
* @ingroup    XUTILS
* 
* @param[in]  position : GEN_NEW position of pointer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetPosition(XDWORD position)
{
  this->position = position;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::IsBlocked()
* @brief      Is blocked
* @ingroup    XUTILS
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::IsBlocked()
{
  if(!xmutex) return false;

  return xmutex->IsLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::SetBlocked(bool blocked)
* @brief      Set blocked
* @ingroup    XUTILS
* 
* @param[in]  blocked : true to blocked buffer.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SetBlocked(bool blocked)
{
  if(!xmutex) return false;

  return (blocked)?xmutex->Lock():xmutex->UnLock();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XBYTE* pbuffer, XDWORD psize)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : pointer of bytes to add
* @param[in]  psize : size of array of bytes
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBYTE* pbuffer, XDWORD psize)
{
  if(!pbuffer)  return false;
  if(!psize)    return false;

  SetBlocked(true);

  XDWORD ppos     = size;
  bool   status   = true;

  if(!Resize(size+psize, false))
    {
      SetBlocked(false);
      return false;
    }

  if(buffer && (size+psize))
         memcpy(&buffer[ppos], pbuffer, psize);
    else status = false;

  SetBlocked(false);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XBUFFER* buffer)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  buffer : pointer of buffer to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBUFFER* buffer)
{
  return Add(buffer->buffer, buffer->size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XBUFFER& buffer)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  buffer : buffer to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBUFFER& buffer)
{
  return Add(buffer.buffer, buffer.size);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XBYTE data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : byte to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XBYTE data)
{
  return Add((XBYTE*)&data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(bool data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : boolean to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(bool data)
{
  XBYTE _data = data?1:0;

  return Add((XBYTE*)&_data, sizeof(XBYTE));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XWORD data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : WORD to add.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XWORD data)
{
  if(localhardwareuselittleendian) SWAPWORD(data);

  return Add((XBYTE*)&data, sizeof(XWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XDWORD data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : DWORD to Add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XDWORD data)
{
  if(localhardwareuselittleendian) SWAPDWORD(data);

  return Add((XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XQWORD data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : QWORD to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XQWORD data)
{
  if(localhardwareuselittleendian) SWAPQWORD(data);

  return Add((XBYTE*)&data, sizeof(XQWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(float data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : float to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(float data)
{
  return Add((XBYTE*)&data, sizeof(XDWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(double data)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  data : double to add
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(double data)
{
  return Add((XBYTE*)&data, sizeof(XQWORD));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Add(XSTRING& string, bool normalize)
* @brief      Add value
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* @param[in]  normalize : Normalize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Add(XSTRING& string, bool normalize)
{
  if(string.IsEmpty()) return false;

  for(XDWORD c=0; c<string.GetSize(); c++)
    {
      if(normalize)
        {
          XWORD data = (XWORD)string.Get()[c];
          if(localhardwareuselittleendian) SWAPWORD(data);
          Add((XWORD)data);
        }
       else
        {
          XDWORD data = (XDWORD)string.Get()[c];
          if(localhardwareuselittleendian) SWAPDWORD(data);
          Add((XDWORD)data);
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddWithMaskArg(XCHAR* mask, va_list& arg)
* @brief      Add with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMaskArg(XCHAR* mask, va_list& arg)
{
  return AddXBufferWithMask((*this), mask, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddWithMaskArg(XSTRING* mask, va_list& arg)
* @brief      Add with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMaskArg(XSTRING* mask, va_list& arg)
{
  return AddXBufferWithMask((*this), mask, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddWithMask(XCHAR* mask, ...)
* @brief      Add with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer XCHAR with a mask to use in add
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMask(XCHAR* mask, ...)
{
  if(!mask) return false;

  va_list arg;

  va_start(arg, mask);

  bool status = AddXBufferWithMask((*this), mask, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddWithMask(XSTRING* mask, ...)
* @brief      Add with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : string with a mask to use in add
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddWithMask(XSTRING* mask, ...)
{
  if(!mask) return false;

  va_list arg;

  va_start(arg, mask);

  bool status = AddXBufferWithMask((*this), mask, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XBYTE* pbuffer, XDWORD psize, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : pointer to bytes to add
* @param[in]  psize : size of bytes to add
* @param[in]  frompos : position in buffer to insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBYTE* pbuffer, XDWORD psize, int frompos)
{
  if(!pbuffer)                  return false;
  if(!psize)                    return false;

  if(frompos < 0)               return false;
  if(frompos >= (int)GetSize()) return false;

  XBUFFER xbuffertempo;

  if(frompos) xbuffertempo.Add(Get(), frompos);

  xbuffertempo.Add(pbuffer, psize);
  xbuffertempo.Add(&Get()[frompos], (GetSize()-frompos));

  Empty();

  Add(xbuffertempo);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XBUFFER* xbuffer, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : pointer of buffer to insert into buffer
* @param[in]  frompos : position to insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER* xbuffer, int frompos)
{
  if(!xbuffer)  return false;

  return Insert(xbuffer->Get(), xbuffer->GetSize(), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XBUFFER& xbuffer, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : buffer to insert into buffer
* @param[in]  frompos : position to insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBUFFER& xbuffer, int frompos)
{
  return Insert(xbuffer.Get(), xbuffer.GetSize(), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XBYTE data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : byte to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XBYTE data, int frompos)
{
  return Insert(&data, 1, frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(bool data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : boolean to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(bool data, int frompos)
{
  XBYTE _data = data?1:0;

  return Insert(&_data, 1, frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XWORD data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : WORD to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XWORD data, int frompos)
{
  if(localhardwareuselittleendian) SWAPWORD(data);

  return Insert((XBYTE*)&data, sizeof(XWORD), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XDWORD data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : Double word to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XDWORD data, int frompos)
{
  if(localhardwareuselittleendian) SWAPDWORD(data);

  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XQWORD data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : Quad word to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XQWORD data, int frompos)
{
  if(localhardwareuselittleendian) SWAPQWORD(data);

  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(float data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : float to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(float data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XDWORD), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(double data, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  data : double to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(double data, int frompos)
{
  return Insert((XBYTE*)&data, sizeof(XQWORD), frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Insert(XSTRING& string, int frompos)
* @brief      Insert
* @ingroup    XUTILS
* 
* @param[in]  string : string to insert
* @param[in]  frompos : position of insert
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Insert(XSTRING& string, int frompos)
{
  if(string.IsEmpty()) return false;

  for(XDWORD c=0; c<string.GetSize(); c++)
    {
      XDWORD data = (XDWORD)string.Get()[c];

      if(localhardwareuselittleendian) SWAPDWORD(data);

      Insert((XDWORD)data, frompos+c);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::InsertWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
* @brief      Insert with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
{
  XBUFFER xbuffer;

  AddXBufferWithMask(xbuffer, mask, arg);

  return Insert(xbuffer, frompos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::InsertWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
* @brief      Insert with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
{
  return InsertWithMaskArg(mask->Get(), frompos, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::InsertWithMask(XCHAR* mask, int frompos, ...)
* @brief      Insert with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer XCHAR with a mask to use in insert
* @param[in]  frompos : position of insert
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = InsertWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::InsertWithMask(XSTRING* mask, int frompos, ...)
* @brief      Insert with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : string with a mask to use in insert
* @param[in]  frompos : position of insert
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::InsertWithMask(XSTRING* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = InsertWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::Extract(XBYTE* pbuffer, XDWORD ppos, XDWORD psize)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : pointer to array of bytes.
* @param[in]  ppos : position of buffer to extract
* @param[in]  psize : size bytes to extract
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::Extract(XBYTE* pbuffer, XDWORD ppos, XDWORD psize)
{
  SetBlocked(true);

  if((ppos>=size)||(!size))
    {
      SetBlocked(false);
      return 0;
    }

  int esize = psize;

  if((ppos+psize)>=size) esize = (size-ppos);

  if(pbuffer) memcpy(pbuffer, &buffer[ppos], esize);

  // #Imanol changed to avoid copyng overlapped memory, as that's undefined
  memmove(&buffer[ppos], &buffer[ppos+esize], size-(ppos+esize));

  //memcpy(&buffer[ppos],&buffer[ppos+esize],size-(ppos+esize));

  Resize(size-esize, false);

  SetBlocked(false);

  return esize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(XBYTE& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : byte extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XBYTE& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(bool& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : boolean extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(bool& data, XDWORD ppos)
{
  XBYTE _data = 0;

  XDWORD size = Extract((XBYTE*)&_data, ppos, sizeof(XBYTE));
  if(size!=sizeof(XBYTE)) return false;

  data = _data?true:false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(XWORD& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : WORD extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XWORD));
  if(size!=sizeof(XWORD)) return false;

  if(localhardwareuselittleendian) SWAPWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(XDWORD& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : DWORD extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XDWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  if(localhardwareuselittleendian) SWAPDWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(XQWORD& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : QWORD extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XQWORD& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  if(localhardwareuselittleendian) SWAPQWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(float& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : float extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(float& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XDWORD));
  if(size!=sizeof(XDWORD)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(double& data, XDWORD ppos)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  data : double extracted
* @param[in]  ppos : position of buffer to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(double& data, XDWORD ppos)
{
  XDWORD size = Extract((XBYTE*)&data, ppos, sizeof(XQWORD));
  if(size!=sizeof(XQWORD)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Extract(XSTRING& string, XDWORD ppos, XDWORD psize)
* @brief      Extract
* @ingroup    XUTILS
* 
* @param[in]  string : string extracted
* @param[in]  ppos : position of buffer to extract
* @param[in]  psize : size of string to extract
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Extract(XSTRING& string, XDWORD ppos, XDWORD psize)
{
  string.Empty();

  XDWORD* _string = GEN_NEW XDWORD[psize];
  int     _size   = psize * sizeof(XDWORD);

  if(!_string) return false;

  int rsize = Extract((XBYTE*)_string, ppos, _size);
  if(rsize == _size)
    {
      for(int c=0; c<(int)psize; c++)
        {
          XDWORD data = (XDWORD)_string[c];
          string.Add((XCHAR)data);
        }
    }

  GEN_DELETE_ARRAY _string;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
* @brief      Extract with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
{
  #ifndef BUILDER

  if(!mask)                         return false;
  if(frompos<0)                     return false;
  if((XDWORD)frompos >= GetSize())  return false;

  XBUFFER buffer;
  va_list arg2;
  XDWORD  sizearg = 0;

  va_copy(arg2, arg);

  sizearg = CalculeExtractSizeArgWithMask((*this), frompos, mask, arg2);

  va_end(arg2);

  if(!sizearg) return false;

  buffer.Add(&Get()[frompos], sizearg);

  bool status = ExtractXBufferWithMask(buffer, mask, arg);
  if(status) Extract(NULL, frompos, sizearg);

  return status;
  #else
  return false;
  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
* @brief      Extract with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
{
  return ExtractWithMaskArg(mask->Get(), frompos, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractWithMask(XCHAR* mask, int frompos, ...)
* @brief      Extract with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer XCHAR with a mask to use in extract
* @param[in]  frompos : position of buffer to extract
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = ExtractWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractWithMask(XSTRING* mask, int frompos, ...)
* @brief      Extract with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : string with a mask to use in extract
* @param[in]  frompos : position of buffer to extract
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractWithMask(XSTRING* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = ExtractWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* XBUFFER::Get()
* @brief      Get value
* @ingroup    XUTILS
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* XBUFFER::Get()
{
  if(!size) return NULL;

  return buffer;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBUFFER::GetByte(XDWORD index)
* @brief      Get byte
* @ingroup    XUTILS
* 
* @param[in]  index : index of byte into buffer
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::GetByte(XDWORD index)
{
  SetBlocked(true);

  XBYTE data;

  if(index > size)
    {
      SetBlocked(false);
      return 0;
    }

  data = buffer[index];

  position++;

  SetBlocked(false);

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBUFFER::GetLastByte()
* @brief      Get last byte
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::GetLastByte()
{
  SetBlocked(true);

  XBYTE data = buffer[size-1];

  SetBlocked(false);

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         char* XBUFFER::GetPtrChar()
* @brief      Get ptr char
* @ingroup    XUTILS
* 
* @return     char* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
char* XBUFFER::GetPtrChar()
{
  return (char*)Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD* XBUFFER::GetPtrWord()
* @brief      Get ptr word
* @ingroup    XUTILS
* 
* @return     XWORD* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD* XBUFFER::GetPtrWord()
{
  return (XWORD*)Get();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XBYTE* pbuffer, int psize, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : byte array obtained
* @param[in]  psize : size of array
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XBYTE* pbuffer, int psize, int frompos)
{
  if(!pbuffer) return false;

  SetBlocked(true);

  int pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  if((!buffer) || (pos>(int)size))
    {
      SetBlocked(false);
      return false;
    }

  if(buffer)
    {
      memcpy(pbuffer, &buffer[pos], psize);
      position = pos + psize;
    }

  SetBlocked(false);

  if(!buffer) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XBYTE& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : byte obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XBYTE& data, int frompos)
{
  SetBlocked(true);

  int pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  if(pos>(int)size)
    {
      SetBlocked(false);
      return false;
    }

  data = buffer[pos];

  position = pos + 1;

  SetBlocked(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(bool& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : boolean obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(bool& data, int frompos)
{
  XBYTE _data = 0;
  bool  status;

  status = Get(_data, frompos);

  if(status) data = _data?true:false;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XWORD& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : WORD obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XWORD), frompos)) return false;

  if(localhardwareuselittleendian) SWAPWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XDWORD& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : double WORD obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XDWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XDWORD), frompos)) return false;

  if(localhardwareuselittleendian) SWAPDWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XQWORD& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : quad WORD obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XQWORD& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XQWORD), frompos)) return false;

  if(localhardwareuselittleendian) SWAPQWORD(data);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(float& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : float obtained
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(float& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XDWORD), frompos)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(double& data, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : double obtained.
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(double& data, int frompos)
{
  if(!Get((XBYTE*)&data, sizeof(XQWORD), frompos)) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Get(XSTRING& data, int psize, int frompos)
* @brief      Get value
* @ingroup    XUTILS
* 
* @param[in]  data : strint obtained
* @param[in]  psize : size n bytes to obtain
* @param[in]  frompos : position of buffer to get
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Get(XSTRING& data, int psize, int frompos)
{
  XDWORD pos = frompos;
  if(frompos == XBUFFER_INVALIDPOSITION) pos = position;

  XDWORD c;

  for(XDWORD i = 0; i < (XDWORD)psize; i++)
    {
      if(!Get(c, pos)) return false;

      if(localhardwareuselittleendian) SWAPDWORD(c);

      data.Add((XCHAR)c);

      pos += sizeof(XDWORD);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
* @brief      Get with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMaskArg(XCHAR* mask, int frompos, va_list& arg)
{
  XBUFFER xbuffer;

  if(!mask)                         return false;
  if(frompos<0)                     return false;
  if((XDWORD)frompos >= GetSize())  return false;

  xbuffer.Add(&Get()[frompos], (GetSize()-frompos));

  bool status = ExtractXBufferWithMask(xbuffer, mask, arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
* @brief      Get with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMaskArg(XSTRING* mask, int frompos, va_list& arg)
{
  if(!mask) return false;

  return GetWithMaskArg(mask->Get(), frompos, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetWithMask(XCHAR* mask, int frompos, ...)
* @brief      Get with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer to string with a mask to use in get
* @param[in]  frompos : position of buffer to get
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMask(XCHAR* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = GetWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetWithMask(XSTRING* mask, int frompos, ...)
* @brief      Get with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer to string with a mask to use in get
* @param[in]  frompos : position of buffer to get
* @param[in]  ... : params with a variant data
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetWithMask(XSTRING* mask, int frompos, ...)
{
  if(!mask) return false;

  va_list  arg;

  va_start(arg, frompos);

  bool status = GetWithMaskArg(mask, frompos, arg);

  va_end(arg);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XBYTE* pbuffer, int psize, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : pointer of buffer to set
* @param[in]  psize : size of buffer to set.
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XBYTE* pbuffer, int psize, int topos)
{
  SetBlocked(true);

  int pos = topos;
  if(topos == XBUFFER_INVALIDPOSITION) pos = position;

  if((pos+psize) > (int)size)
    {
      SetBlocked(false);
      return false;
    }

  if(pbuffer)
          memcpy(&buffer[pos], pbuffer, psize);
    else  memset(&buffer[pos], 0, psize);

  if(pos==(int)position)  position += psize;

  SetBlocked(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XBYTE data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : byte to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XBYTE data, int topos)
{
  SetBlocked(true);

  int pos = topos;
  if(topos == XBUFFER_INVALIDPOSITION) pos = position;

  if(pos>(int)size)
    {
      SetBlocked(false);
      return false;
    }

  buffer[pos]= data;

  if(pos==(int)position) position++;

  SetBlocked(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(bool data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : boolean to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(bool data, int topos)
{
  XBYTE _data = data?1:0;

  return Set(_data, topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XWORD data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : WORD to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XWORD data, int topos)
{
  if(localhardwareuselittleendian) SWAPWORD(data);

  return Set((XBYTE*)&data, sizeof(XWORD), topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XDWORD data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : DWORD to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XDWORD data, int topos)
{
  if(localhardwareuselittleendian) SWAPDWORD(data);

  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XQWORD data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : QWORD to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XQWORD data, int topos)
{
  if(localhardwareuselittleendian) SWAPQWORD(data);

  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(float data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : float to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(float data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XDWORD), topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(double data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : double to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(double data, int topos)
{
  return Set((XBYTE*)&data, sizeof(XQWORD), topos);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Set(XSTRING& data, int topos)
* @brief      Set value
* @ingroup    XUTILS
* 
* @param[in]  data : string to set
* @param[in]  topos : position of buffer to set
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Set(XSTRING& data, int topos)
{
  // should create temporary array and then put that
  // but on stack it's too much?

  XDWORD* tmp = GEN_NEW XDWORD[data.GetSize()];
  if(!tmp) return false;

  for(XDWORD i = 0; i < data.GetSize(); i++)
    {
      XDWORD _data = data[i];
      if(localhardwareuselittleendian) SWAPDWORD(_data);
      tmp[i] = _data;
    }

  if(!Set((XBYTE*)tmp, data.GetSize()*sizeof(XDWORD), topos)) return false;

  GEN_DELETE_ARRAY tmp;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::SetWithMaskArg(XCHAR* mask, int topos, va_list& arg)
* @brief      Set with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  topos : Topos value.
* @param[in]  arg : Arg value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMaskArg(XCHAR* mask, int topos, va_list& arg)
{
  XBUFFER xbuffer;

  AddXBufferWithMask(xbuffer, mask, arg);

  Set(xbuffer.Get(), xbuffer.GetSize(), topos);

  return xbuffer.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::SetWithMaskArg(XSTRING* mask, int topos, va_list& arg)
* @brief      Set with mask arg
* @ingroup    XUTILS
* 
* @param[in]  mask : Mask pointer to use.
* @param[in]  topos : Topos value.
* @param[in]  arg : Arg value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMaskArg(XSTRING* mask, int topos, va_list& arg)
{
  if(!mask) return false;

  return SetWithMaskArg(mask->Get(), topos, arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::SetWithMask(XCHAR* mask, int topos, ...)
* @brief      Set with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : pointer XCHAR with a mask to use in extract
* @param[in]  topos : position of buffer to set
* @param[in]  ... : params with a variant data
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMask(XCHAR* mask, int topos, ...)
{
  if(!mask) return 0;

  va_list  arg;

  va_start(arg, topos);

  XDWORD result = SetWithMaskArg(mask, topos, arg);

  va_end(arg);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::SetWithMask(XSTRING* mask, int topos, ...)
* @brief      Set with mask
* @ingroup    XUTILS
* 
* @param[in]  mask : string with a mask to use in extract
* @param[in]  topos : position of buffer to set
* @param[in]  ... : params with a variant data
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::SetWithMask(XSTRING* mask, int topos, ...)
{
  if(!mask) return 0;

  va_list  arg;

  va_start(arg, topos);

  XDWORD result = SetWithMaskArg(mask, topos, arg);

  va_end(arg);

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Resize(XDWORD newsize, bool setblocked)
* @brief      Resize
* @ingroup    XUTILS
* 
* @param[in]  newsize : GEN_NEW size of bufffer.
* @param[in]  setblocked : blocked this operation.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Resize(XDWORD newsize, bool setblocked)
{
  bool status = true;

  if(setblocked) SetBlocked(true);

  if(!newsize)
    {
      size     = 0;
      position = 0;

      if(sizeassign) memset(buffer, 0, sizeassign);
    }
   else
    {     
      if(newsize > sizeassign)
        { 
          XBYTE* newbuffer = NULL;         

          sizeassign = (newsize + addblockmemsize); 

          newbuffer = GEN_NEW XBYTE[sizeassign];
          if(newbuffer)
            {             
              memset(newbuffer, 0, sizeassign);

              if(buffer)
                {
                  memcpy(newbuffer , buffer, size);

                  GEN_DELETE_ARRAY buffer;

                  buffer   = NULL;
                  size     = 0;
                  position = 0;
                }

              buffer      = newbuffer;              
              size        = newsize;
              position    = 0;
            }
           else
            {
              status = false;
            }
        }
       else
        {
          size        = newsize;
          position    = 0;

          if(sizeassign > size)
            {
              memset(&buffer[size], 0, (sizeassign-size));
            }
        }

      if(size)
        {
          if(position>=size) position = (size-1);
        }
   }

  if(setblocked) SetBlocked(false);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Delete(bool setblocked)
* @brief      Delete resource
* @ingroup    XUTILS
* 
* @param[in]  setblocked : blocked this operation.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Delete(bool setblocked)
{
  if(setblocked) 
    {
      SetBlocked(true);
    }

  if(buffer) GEN_DELETE_ARRAY buffer;

  buffer        = NULL;
  size          = 0;
  sizeassign    = 0;
  position      = 0;

  if(setblocked) 
    {
      SetBlocked(false);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::SecureDelete(bool setblocked)
* @brief      Overwrite the complete allocated storage through volatile writes before releasing it
* @ingroup    XUTILS
* 
* @param[in]  setblocked : 
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::SecureDelete(bool setblocked)
{
  if(setblocked)
    {
      SetBlocked(true);
    }

  if(buffer)
    {
      volatile XBYTE* securebuffer = (volatile XBYTE*)buffer;

      for(XDWORD c=0; c<sizeassign; c++) securebuffer[c] = 0;

      GEN_DELETE_ARRAY buffer;
    }

  buffer     = NULL;
  size       = 0;
  sizeassign = 0;
  position   = 0;

  if(setblocked)
    {
      SetBlocked(false);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::DeleteByte(XBYTE data, bool setblocked)
* @brief      Delete byte
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  setblocked : Setblocked value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::DeleteByte(XBYTE data, bool setblocked)
{  
  XDWORD  c=0;
  bool    status = false;

  if(setblocked) 
    {
      SetBlocked(true);
    }

  while(c<GetSize())
    {
      XBYTE  none;

      if(buffer[c] == data)
        {
          Extract(none, c);

          status = true;
        } 
       else
        {
          c++;
        }
    }

  if(setblocked) 
    {
      SetBlocked(false);
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::FillBuffer(XBYTE fillchar)
* @brief      Fill buffer
* @ingroup    XUTILS
* 
* @param[in]  fillchar : byte to fill buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::FillBuffer(XBYTE fillchar)
{
  SetBlocked(true);

  bool status = false;

  if(buffer && size)
    {
      memset(buffer, fillchar, size);
      status = true;
    }

  SetBlocked(false);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Empty()
* @brief      Empty
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Empty()
 {
   return Delete();
 }


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Swap()
* @brief      Swap
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Swap()
{
  SetBlocked(true);

  for(int c=0; c<(int)(size/2); c++)
    {
      XBYTE tembyte;

      tembyte          = buffer[c];
      buffer[c]        = buffer[size-c-1];
      buffer[size-c-1] = tembyte;
    }

  SetBlocked(false);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Compare(XBYTE* pbuffer, XDWORD psize)
* @brief      Compare
* @ingroup    XUTILS
* 
* @param[in]  pbuffer : pointer to buffer to compare
* @param[in]  psize : size of buffer to compare
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Compare(XBYTE* pbuffer, XDWORD psize)
{
  if(!pbuffer)  return false;
  if(!psize)    return false;

  bool status;

  SetBlocked(true);

  if(psize != size)
    {
      SetBlocked(false);
      return false;
    }

  status = (memcmp(pbuffer, buffer, psize) == 0);

  SetBlocked(false);

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Compare(XBUFFER* buffer)
* @brief      Compare
* @ingroup    XUTILS
* 
* @param[in]  buffer : pointer to buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Compare(XBUFFER* buffer)
{
  if(!buffer) return false;

  return Compare(buffer->Get(), buffer->GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Compare(XBUFFER& buffer)
* @brief      Compare
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Compare(XBUFFER& buffer)
{
  return Compare(&buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XBUFFER::Find(XBUFFER* buffer, int startindex)
* @brief      Find
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* @param[in]  startindex : Startindex value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XBUFFER::Find(XBUFFER* buffer, int startindex)
{
  if(!buffer)                        return NOTFOUND;

  if(IsEmpty())                      return NOTFOUND;
  if(!buffer->GetSize())             return NOTFOUND;

  XBYTE* _buffer = buffer->Get();
  XDWORD _size   = buffer->GetSize();

  if(_size > size)                    return NOTFOUND;
  if(startindex>=(int)(size-_size)+1) return NOTFOUND;
  if(startindex<0)                    return NOTFOUND;

  bool found;

  for(XDWORD c=startindex; c<(size-_size)+1; c++)
    {
      found = true;

      for(XDWORD d=0; d<_size; d++)
        {
          if(this->buffer[c+d] != _buffer[d])
            {
              found = false;
              break;
            }
        }

      if(found) return c;
    }

  return NOTFOUND;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XBUFFER::Find(XSTRING& string, bool normalize, int startindex)
* @brief      Find
* @ingroup    XUTILS
* 
* @param[in]  string : String to use.
* @param[in]  normalize : Normalize value.
* @param[in]  startindex : Startindex value.
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XBUFFER::Find(XSTRING& string, bool normalize, int startindex)
{
  XBUFFER buffer;

  buffer.Add(string, normalize);

  return XBUFFER::Find(&buffer, startindex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBUFFER::operator = (const XBUFFER& xbuffer)
* @brief      operator =
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::operator = (const XBUFFER& xbuffer)
{
  CopyFrom((XBUFFER&)xbuffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::operator == (XBUFFER buffer)
* @brief      operator ==
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool XBUFFER:: : Requested buffer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::operator == (XBUFFER buffer)
{ 
  return Compare(buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::operator != (XBUFFER buffer)
* @brief      operator !=
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool XBUFFER:: : Requested buffer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::operator != (XBUFFER buffer)
{ 
  return !Compare(buffer);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::CopyFrom(XBUFFER& buffer)
* @brief      Copy from
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::CopyFrom(XBUFFER& buffer)
{
  Empty();

  localhardwareuselittleendian = buffer.GetLocalHardwareUseLittleEndian();
  
  return Add(buffer.Get(), buffer.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::CopyTo(XBUFFER& buffer)
* @brief      Copy to
* @ingroup    XUTILS
* 
* @param[in]  buffer : Buffer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::CopyTo(XBUFFER& buffer)
{
  buffer.Empty();

  buffer.SetLocalHardwareUseLittleEndian(localhardwareuselittleendian);
    
  return buffer.Add(Get(), GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Padding_Add(XBYTE bitsadjust, XBUFFER_PADDINGTYPE type)
* @brief      Padding add
* @ingroup    XUTILS
* 
* @param[in]  bitsadjust : n bits of padding
* @param[in]  type : type of padding
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Add(XBYTE bitsadjust, XBUFFER_PADDINGTYPE type)
{
  if(paddinghas) return false;

  paddingbitadjust = bitsadjust;
  paddingsize     = bitsadjust - (size % bitsadjust);

  switch(type)
    {
      case XBUFFER_PADDINGTYPE_ZEROS        :
      case XBUFFER_PADDINGTYPE_SPACES       :
      case XBUFFER_PADDINGTYPE_PKCS7        :
      case XBUFFER_PADDINGTYPE_PKCS5        : for(int c=0;c<paddingsize;c++)
                                                {
                                                  switch(type)
                                                    {
                                                      case XBUFFER_PADDINGTYPE_ZEROS        : Add((XBYTE)0x00);
                                                                                              break;

                                                      case XBUFFER_PADDINGTYPE_SPACES       : Add((XBYTE)0x20);
                                                                                              break;

                                                      case XBUFFER_PADDINGTYPE_PKCS7        :
                                                      case XBUFFER_PADDINGTYPE_PKCS5        : Add((XBYTE)paddingsize);
                                                                                              break;
                                                      default: break;
                                                    }
                                                }
                                              break;

      case XBUFFER_PADDINGTYPE_ANSIX923     : for(int c=0;c<paddingsize-1;c++)
                                                {
                                                  Add((XBYTE)0x00);
                                                }
                                              Add((XBYTE)paddingsize);
                                              break;

      case XBUFFER_PADDINGTYPE_ISO10126     : { XRAND* xrand=GEN_XFACTORY.CreateRand();
                                                if(!xrand) return false;

                                                for(int c=0;c<paddingsize-1;c++)
                                                  {
                                                    Add((XBYTE)0x00);
                                                  }

                                                Add((XBYTE)paddingsize);
                                                GEN_XFACTORY.DeleteRand(xrand);
                                              }
                                              break;

      case XBUFFER_PADDINGTYPE_ISOIEC7816_4 : Add((XBYTE)0x80);
                                              for(int c=0;c<paddingsize-1;c++)
                                                {
                                                  Add((XBYTE)0x00);
                                                }
                                              break;
      default: break;
    }

  paddingtype     = type;
  paddinghas      = true;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Padding_Has()
* @brief      Padding has
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Has()
{
  return paddinghas;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER_PADDINGTYPE XBUFFER::Padding_GetType()
* @brief      Padding get type
* @ingroup    XUTILS
* 
* @return     XBUFFER_PADDINGTYPE : Requested buffer.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER_PADDINGTYPE XBUFFER::Padding_GetType()
{
  return paddingtype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBUFFER::Padding_GetSize()
* @brief      Padding get size
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::Padding_GetSize()
{
  return paddingsize;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Padding_Delete()
* @brief      Padding delete
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Padding_Delete()
{
  if(!paddinghas) return false;

  int newsize = GetSize() - Padding_GetSize();

  Resize(newsize , true);

  if((int)GetSize() != newsize)  return false;

  paddinghas        = false;
  paddingbitadjust    = 0;
  paddingsize       = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBUFFER::Bit_GetNBits()
* @brief      Bit get N bits
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::Bit_GetNBits()
{
  return nbits;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Bit_SetNBits(int nbits)
* @brief      Bit set N bits
* @ingroup    XUTILS
* 
* @param[in]  nbits : Nbits value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Bit_SetNBits(int nbits)
{
  if(nbits > (int)(sizeof(XDWORD)*8)) return false;

  this->nbits = nbits;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Bit_AddData(XDWORD data, int nbits)
* @brief      Bit add data
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  nbits : Nbits value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Bit_AddData(XDWORD data, int nbits)
{
  int _nbits = nbits;

  if(_nbits == XBUFFER_MASTER_NBITS)  _nbits = this->nbits;
  if(_nbits > (int)(sizeof(XDWORD)*8)) return false;

  if(_nbits > nbitsfree)
    {
      XBYTE missingbits   = (_nbits - nbitsfree);
      XBYTE missingbytes  = (missingbits / 8);

      if(missingbits % 8)  missingbytes++;

       for(int c=0; c<missingbytes; c++)
        {
           Add((XBYTE)0);
        }

       nbitsfree +=  (missingbytes * 8);
    }

  int     posbyte;
  int     posbit        = GetSize() * 8;
  int     possubbyte;
  //int     nbytes        = (_nbits / 8);
  XDWORD  mask          = 0;
  bool    status;

  //if(_nbits % 8)  nbytes++;

  posbit     -= nbitsfree;
  posbyte     = (posbit / 8);
  possubbyte  = (posbit % 8);

  for(int c=0; c<_nbits; c++)
    {
       mask  |= 0x01;
       if(c != _nbits-1) mask <<= 0x01;
    }

  XQWORD  _data = data;
  XQWORD  _dataorg      = 0;

  _data &= (XQWORD)mask;
  _data <<= ((sizeof(XQWORD)*8) - _nbits);
  _data >>= possubbyte;

  int nbytescpy = ((_nbits + possubbyte) / 8);

  if((_nbits + possubbyte) % 8) nbytescpy++;

  if((posbyte + nbytescpy) >= (int)GetSize())
    {
      nbytescpy = GetSize() - posbyte;
    }

  status = Get((XBYTE*)&_dataorg, nbytescpy, posbyte);
  _dataorg = SwapQWORD(_dataorg);

  _data |= _dataorg;

  _data = SwapQWORD(_data);
  status = Set((XBYTE*)&_data, nbytescpy, posbyte);

  if(status) nbitsfree -= _nbits;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::Bit_GetData(int indexbit, int nbits)
* @brief      Bit get data
* @ingroup    XUTILS
* 
* @param[in]  indexbit : Indexbit value.
* @param[in]  nbits : Nbits value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::Bit_GetData(int indexbit, int nbits)
{
  int _nbits = nbits;

  if(_nbits == XBUFFER_MASTER_NBITS)  _nbits = this->nbits;
  if(_nbits > (int)(sizeof(XDWORD)*8)) return false;

  XDWORD  data      = 0;
  int     posbyte   = (indexbit / 8);
  int     posbit    = (indexbit % 8);
  XDWORD  mask      = 0;
  int     nbytescpy = (_nbits / 8);

  if(_nbits % 8)  nbytescpy++;

  nbytescpy++;

  if(posbyte + nbytescpy > (int)size)
    {
      nbytescpy = (size - posbyte);
    }

  if(!Get((XBYTE*)&data, nbytescpy, posbyte)) return 0;

  data = SwapDWORD(data);

  for(int c=0; c<_nbits; c++)
    {
       mask  |= 0x01;
       if(c != _nbits-1) mask <<= 0x01;
    }

  int displacement = (posbit + _nbits);
  if(displacement > (int)(sizeof(XDWORD)*8))  displacement = (sizeof(XDWORD)*8);

  data >>= ((sizeof(XDWORD)*8) - displacement);

  data &= mask;

  return data;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Bit_SetData(XDWORD data, int indexbit, int nbits)
* @brief      Bit set data
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  indexbit : Indexbit value.
* @param[in]  nbits : Nbits value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Bit_SetData(XDWORD data, int indexbit, int nbits)
{
  int _nbits = nbits;

  if(_nbits == XBUFFER_MASTER_NBITS)  _nbits = this->nbits;
  if(_nbits > (int)(sizeof(XDWORD)*8))   return false;

  int     posbyte   = (indexbit / 8);
  int     posbit    = (indexbit % 8);
  XDWORD  mask      = 0;
  int     nbytescpy = (_nbits / 8);

  if(_nbits % 8)  nbytescpy++;

  if(posbyte + nbytescpy > (int)size)
    {
      nbytescpy = (size - posbyte);
    }

  for(int c=0; c<_nbits; c++)
    {
       mask  |= 0x01;
       if(c != _nbits-1) mask <<= 0x01;
    }

  data &= mask;

  data <<= ((sizeof(XDWORD)*8) - (posbit + _nbits));

  data = SwapDWORD(data);

  return Set((XBYTE*)&data, nbytescpy, posbyte);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE XBUFFER::Bit_GetBitsFree()
* @brief      Bit get bits free
* @ingroup    XUTILS
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE XBUFFER::Bit_GetBitsFree()
{
  return nbitsfree;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::Bit_SetBitsFree(XBYTE nbitsfree)
* @brief      Bit set bits free
* @ingroup    XUTILS
* 
* @param[in]  nbitsfree : Nbitsfree value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::Bit_SetBitsFree(XBYTE nbitsfree)
{
  this->nbitsfree = nbitsfree;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ConvertFromBase64(XSTRING& in_stringbase64)
* @brief      Convert from base64
* @ingroup    XUTILS
* 
* @param[in]  in_stringbase64 : In stringbase64 value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ConvertFromBase64(XSTRING& in_stringbase64)
{
  return in_stringbase64.ConvertBase64ToBinary((*this));
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ConvertToBase64(XSTRING& out_stringbase64)
* @brief      Convert to base64
* @ingroup    XUTILS
* 
* @param[in]  out_stringbase64 : Output out stringbase64.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ConvertToBase64(XSTRING& out_stringbase64)
{
  return out_stringbase64.ConvertBinaryToBase64((*this));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::DecodeBCD(XDWORD ppos, XDWORD psize)
* @brief      Decode BCD
* @ingroup    XUTILS
* 
* @param[in]  ppos : position of DWORD
* @param[in]  psize : size of decode
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::DecodeBCD(XDWORD ppos, XDWORD psize)
{
  XDWORD result   = 0;
  int    position = ppos;

  while(psize--)
    {
      XBYTE nibble1 = ( buffer[position] & 0x0F);
      XBYTE nibble2 = ((buffer[position] & 0xF0)>>4);

      if(nibble1 > 9)  return 0;
      if(nibble2 > 9)  return 0;

      result = (result * 100) + ((buffer[position] >> 4) * 10) + (buffer[position] & 0x0F);
      position++;
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XBUFFER::DecodeBCDLong(XDWORD ppos, XDWORD psize)
* @brief      Decode BCD long
* @ingroup    XUTILS
* 
* @param[in]  ppos : Ppos value.
* @param[in]  psize : Psize value.
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XBUFFER::DecodeBCDLong(XDWORD ppos, XDWORD psize)
{
  XQWORD result   = 0;
  int    position = ppos;

  while(psize--)
    {
      XBYTE nibble1 = ( buffer[position] & 0x0F);
      XBYTE nibble2 = ((buffer[position] & 0xF0)>>4);

      if(nibble1 > 9)  return 0;
      if(nibble2 > 9)  return 0;

      result  = (result * 100);
      result += ((buffer[position] >> 4) * 10);
      result += (buffer[position] & 0x0F);

      position++;
    }

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetGlobalHardwareUseLittleEndian()
* @brief      Get global hardware use little endian
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetGlobalHardwareUseLittleEndian()
{
 return  XBUFFER::globalhardwareuselittleendian;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBUFFER::SetGlobalHardwareUseLittleEndian(bool globalhardwareuselittleendian)
* @brief      Set global hardware use little endian
* @ingroup    XUTILS
* 
* @param[in]  globalhardwareuselittleendian : Globalhardwareuselittleendian value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::SetGlobalHardwareUseLittleEndian(bool globalhardwareuselittleendian)
{
  XBUFFER::globalhardwareuselittleendian = globalhardwareuselittleendian;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::GetLocalHardwareUseLittleEndian()
* @brief      Get local hardware use little endian
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::GetLocalHardwareUseLittleEndian()
{
  return localhardwareuselittleendian;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBUFFER::SetLocalHardwareUseLittleEndian(bool localhardwareuselittleendian)
* @brief      Set local hardware use little endian
* @ingroup    XUTILS
* 
* @param[in]  localhardwareuselittleendian : Localhardwareuselittleendian value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::SetLocalHardwareUseLittleEndian(bool localhardwareuselittleendian)
{
  this->localhardwareuselittleendian = localhardwareuselittleendian;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XCHAR* mask, va_list& arg)
* @brief      Add X buffer with mask
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XCHAR* mask, va_list& arg)
{
  if(!mask)    return false;
  if(!mask[0]) return false;

  int c = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case 'B': { XBYTE bvar  = (XBYTE)va_arg(arg, int);
                      xbuffer.Add((XBYTE)bvar);
                    }
                    break;

          case 'W': { XWORD wvar  = (XWORD)va_arg(arg, int);
                      xbuffer.Add((XWORD)wvar);
                    }
                    break;

          case 'D': { XDWORD dvar = va_arg(arg, XDWORD);
                      xbuffer.Add((XDWORD)dvar);
                    }
                    break;

          case 'Q': { XQWORD dvar = va_arg(arg, XQWORD);
                      xbuffer.Add((XQWORD)dvar);
                    }
                    break;

          case 'F': { float fvar = (float)va_arg(arg, double);
                      xbuffer.Add((float)fvar);
                    }
                    break;

          case 'L': { double lvar = va_arg(arg, double);
                      xbuffer.Add((double)lvar);
                    }
                    break;

          case 'O': { int _bvar = va_arg(arg, int);
                      XBYTE bvar  = (_bvar?1:0);
                      xbuffer.Add((XBYTE)bvar);
                    }
                    break;

          case 'A': { XBYTE* avar = va_arg(arg, XBYTE*);
                      int    size;
                      c++;

                      if(mask[c]==_C('X'))
                        {
                          size = va_arg(arg, XDWORD);
                        }
                       else
                        {
                          XSTRING sizestr;

                          while((mask[c]>=_C('0')) && (mask[c]<=_C('9')))
                            {
                              sizestr.Add(mask[c]);
                              c++;
                            }

                          c--;

                          size = sizestr.ConvertToInt();
                        }

                      if(size && avar) xbuffer.Add(avar, size);
                    }
                    break;

          case 'S': { XSTRING*  svar = va_arg(arg, XSTRING*);                      
                      XBUFFER   bufferexchange;

                      (*svar).ConvertToExchangeXBuffer(bufferexchange);
                      xbuffer.Add(bufferexchange);                        
                    }
                    break;

          case 'V': { XVARIANT* svar        = va_arg(arg, XVARIANT*);
                      XDWORD    type        = svar->GetType();
                      XBYTE     isnull      = svar->IsNull();
                      XDWORD    sizevariant = svar->GetSize();

                      xbuffer.Add((XDWORD)type);
                      xbuffer.Add((XBYTE)isnull);
                      xbuffer.Add((XDWORD)sizevariant);

                      if(svar->GetData() && sizevariant) xbuffer.Add((XBYTE*)svar->GetData(), sizevariant);
                    }
                    break;

        }

       c++;
    }

  return xbuffer.GetSize()?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
* @brief      Add X buffer with mask
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::AddXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
{
  if(!mask) return false;

  return AddXBufferWithMask(xbuffer, mask->Get(), arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XBUFFER::CalculeExtractSizeArgWithMask(XBUFFER& xbuffer, int frompos, XCHAR* mask, va_list& arg)
* @brief      Calcule extract size arg with mask
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  frompos : Frompos value.
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XBUFFER::CalculeExtractSizeArgWithMask(XBUFFER& xbuffer, int frompos, XCHAR* mask, va_list& arg)
{
  if(!mask)    return false;
  if(!mask[0]) return false;

  int size = 0;
  int c    = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case 'B': { //XBYTE* bvar  = va_arg(arg, XBYTE*);
                      size += sizeof(XBYTE);
                    }
                    break;

          case 'W': { //XWORD wvar  = (XWORD)va_arg(arg, int);
                      size += sizeof(XWORD);
                    }
                    break;

          case 'D': { //XDWORD dvar = va_arg(arg, XDWORD);
                      size += sizeof(XDWORD);
                    }
                    break;

          case 'Q': { //XQWORD dvar = va_arg(arg, XQWORD);
                      size += sizeof(XQWORD);
                    }
                    break;

          case 'F': { //float fvar = (float)va_arg(arg, double);
                      size += sizeof(double);
                    }
                    break;

          case 'L': { //double lvar = va_arg(arg, double);
                      size += sizeof(double);
                    }
                    break;

          case 'O': { //int bvar = va_arg(arg, int);
                      size += sizeof(XBYTE);
                    }
                    break;

          case 'A': { //XBYTE* avar = va_arg(arg, XBYTE*);
                      XDWORD sizearray = 0;
                      c++;

                      if(mask[c]==_C('X'))
                        {
                          XDWORD dvar = va_arg(arg, XDWORD);
                          if(dvar) sizearray = dvar;
                        }
                       else
                        {
                          XSTRING sizestr;

                          while((mask[c]>=_C('0')) && (mask[c]<=_C('9')))
                            {
                              sizestr.Add(mask[c]);
                              c++;
                            }
                          c--;
                          sizearray = sizestr.ConvertToInt();
                        }

                      size += sizearray;
                    }
                    break;

          case 'S': { XSTRING* svar       = va_arg(arg, XSTRING*);
                      XDWORD   sizestring = 0;

                      if(svar)
                        {
                          sizestring  = XSTRING::GetSize((XWORD*)(&xbuffer.Get()[frompos + size]));
                          sizestring += 1;
                          sizestring *= sizeof(XWORD);
                        }

                      size += sizestring;
                    }
                    break;

          case 'V': { XVARIANT* svar        = va_arg(arg, XVARIANT*);
                      XDWORD    sizevariant = 0;

                      sizevariant += sizeof(XDWORD);
                      sizevariant += sizeof(XBYTE);
                      sizevariant += sizeof(XDWORD);

                      if(svar) sizevariant += svar->GetSize();
                      size += sizevariant;
                    }
                    break;
        }

       c++;
    }

  return size;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XCHAR* mask, va_list& arg)
* @brief      Extract X buffer with mask
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XCHAR* mask, va_list& arg)
{
  if(!mask)    return false;
  if(!mask[0]) return false;

  int c = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case 'B': { XBYTE* bvar  = va_arg(arg, XBYTE*);
                      xbuffer.Extract((XBYTE&)(*bvar));
                    }
                    break;

          case 'W': { XWORD* wvar  = va_arg(arg, XWORD*);
                      xbuffer.Extract((XWORD&)(*wvar));
                    }
                    break;

          case 'D': { XDWORD* dvar = va_arg(arg, XDWORD*);
                      xbuffer.Extract((XDWORD&)(*dvar));
                    }
                    break;

          case 'Q': { XQWORD* dvar = va_arg(arg, XQWORD*);
                      xbuffer.Extract((XQWORD&)(*dvar));
                    }
                    break;

          case 'F': { double* fvar = va_arg(arg, double*);
                      xbuffer.Extract((float&)(*fvar), 0);
                    }
                    break;

          case 'L': { double* dvar = va_arg(arg, double*);
                      xbuffer.Extract((double&)(*dvar), 0);
                    }
                    break;

          case 'O': { bool*  bvar  = va_arg(arg, bool*);
                      XBYTE _bvar  = 0;
                      xbuffer.Extract(_bvar);
                      (*bvar) = _bvar?true:false;
                    }
                    break;

          case 'A': { XBYTE* avar = va_arg(arg, XBYTE*);

                      XDWORD size = 0;
                      c++;

                      if(mask[c]==_C('X'))
                        {
                          XDWORD dvar = va_arg(arg, XDWORD);
                          if(dvar) size = dvar;
                        }
                       else
                        {
                          XSTRING sizestr;

                          while((mask[c]>=_C('0')) && (mask[c]<=_C('9')))
                            {
                              sizestr.Add(mask[c]);
                              c++;
                            }
                          c--;
                          size = sizestr.ConvertToInt();
                        }

                      if(size && avar) xbuffer.Extract(avar, 0, size);
                    }
                    break;

          case 'S': { XSTRING* svar = va_arg(arg, XSTRING*);

                      svar->Set((XWORD*)(xbuffer.Get()));
                      XDWORD size = (svar->GetSize()+1)*sizeof(XWORD);

                      xbuffer.Extract(NULL, 0, size);
                    }
                    break;

          case 'V': { XVARIANT* svar        = va_arg(arg, XVARIANT*);
                      XDWORD    type        = 0;
                      XBYTE     isnull      = svar->IsNull();
                      XDWORD    sizevariant = 0;

                      xbuffer.Extract(type);
                      xbuffer.Extract(isnull);
                      xbuffer.Extract(sizevariant);

                      // Imanol : if xbuffer.GetSize() - 9 != sizevariant something went wront and this variant might not be complete(but a full data array is allocated anyways)
                      svar->Set((XVARIANT_TYPE)type, xbuffer.Get(), sizevariant);
                      if(sizevariant) xbuffer.Extract(NULL, 0, sizevariant);
                    }
                    break;
        }

       c++;
    }

  return xbuffer.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
* @brief      Extract X buffer with mask
* @ingroup    XUTILS
* 
* @param[in]  xbuffer : Buffer to use.
* @param[in]  mask : Mask pointer to use.
* @param[in]  arg : Arg value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XBUFFER::ExtractXBufferWithMask(XBUFFER& xbuffer, XSTRING* mask, va_list& arg)
{
  if(!mask) return false;

  return ExtractXBufferWithMask(xbuffer, mask->Get(), arg);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XBUFFER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XBUFFER::Clean()
{
  buffer              = NULL;
  size                = 0;
  sizeassign          = 0;
  position            = 0;

  addblockmemsize     = XBUFFER_ADDBLOCKMEMSIZE_DEFAULT;

  nbits               = 0;
  nbitsfree           = 0;

  paddingtype         = XBUFFER_PADDINGTYPE_NONE;
  paddinghas          = false;
  paddingbitadjust    = 0;
  paddingsize         = 0;

  xmutex              = NULL;

  localhardwareuselittleendian = false;
}


