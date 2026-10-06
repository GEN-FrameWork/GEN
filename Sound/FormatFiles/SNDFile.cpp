/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFile.cpp
* 
* @class      SNDFILE
* @brief      Sound File class
* @ingroup    SOUND
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

#include "SNDFile.h"

#include "XFactory.h"
#include "XFile.h"

#include "SNDFileOGG.h"
#include "SNDFileWAV.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE::SNDFILE()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE::SNDFILE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE::~SNDFILE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE::~SNDFILE()
{  
  if(xbuffer)             
    {
      GEN_DELETE xbuffer;
    }

  if(xbufferdecodeddata)  
    {
      GEN_DELETE xbufferdecodeddata;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE* SNDFILE::Create(XCHAR* path)
* @brief      Create resource
* @ingroup    SOUND
* 
* @param[in]  path : Path to use.
* 
* @return     SNDFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE* SNDFILE::Create(XCHAR* path)
{
  XPATH     xpath;
  XSTRING   ext;
  SNDFILE*  sndfile = NULL;

  xpath = path;
  if(xpath.IsEmpty())
    {
      return NULL;
    }

  xpath.GetExt(ext);
  
  if(!ext.Compare(_L(".ogg"), true))
    {
      sndfile = GEN_NEW SNDFILEOGG();  
    }

  if(!ext.Compare(_L(".wav"), true))
    {
      sndfile = GEN_NEW SNDFILEWAV();  
    }

  if(sndfile)
    {
      XFILE* file = GEN_XFACTORY.Create_File();
      if(file)
        {
          bool exist = file->Exist(xpath);
          GEN_XFACTORY.Delete_File(file);

          if(!exist)
            {
              GEN_DELETE sndfile;  
              return NULL;
            }
        }

      sndfile->GetPath()->Set(path);

      if(!sndfile->LoadFile())
        {
          GEN_DELETE sndfile;  
          return NULL;
        }


    }

  return sndfile;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE* SNDFILE::Create(XPATH& xpath)
* @brief      Create resource
* @ingroup    SOUND
* 
* @param[in]  xpath : Path to use.
* 
* @return     SNDFILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE* SNDFILE::Create(XPATH& xpath)
{
  return SNDFILE::Create(xpath.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILE_TYPE SNDFILE::GetType()
* @brief      Get type
* @ingroup    SOUND
* 
* @return     SNDFILE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILE_TYPE SNDFILE::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XPATH* SNDFILE::GetPath()
* @brief      Get path
* @ingroup    SOUND
* 
* @return     XPATH* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XPATH* SNDFILE::GetPath()
{
  return &path;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBUFFER* SNDFILE::GetData()
* @brief      Get data
* @ingroup    SOUND
* 
* @return     XBUFFER* : Pointer to the requested buffer; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBUFFER* SNDFILE::GetData()
{ 
  return xbufferdecodeddata;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD SNDFILE::GetChannels()
* @brief      Get channels
* @ingroup    SOUND
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD SNDFILE::GetChannels()
{ 
  return channels;            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDFILE::GetNSamples()
* @brief      Get N samples
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDFILE::GetNSamples()
{ 
  return nsamples;             
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDFILE::GetSampleRate()
* @brief      Get sample rate
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDFILE::GetSampleRate()
{ 
  return samplerate;          
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD SNDFILE::GetDuration()
* @brief      Get duration
* @ingroup    SOUND
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD SNDFILE::GetDuration()
{ 
  return duration;            
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFILE::LoadFile()
* @brief      Load file
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFILE::LoadFile()      
{ 
  return false;               
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDFILE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDFILE::Clean()
{  
  type                = SNDFILE_TYPE_UNKNOWN;

  channels            = 0;
  nsamples            = 0;
  samplerate          = 0;
  duration            = 0;
  
  xbuffer             = NULL;
  xbufferdecodeddata  = NULL;
}


