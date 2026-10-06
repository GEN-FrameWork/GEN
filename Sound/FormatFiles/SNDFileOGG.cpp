/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFileOGG.cpp
* 
* @class      SNDFILEOGG
* @brief      Sound File OGG format class
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

#include "SNDFileOGG.h"

#include "stb_vorbis.c"      // Very poorly done by the STB Vorbis library. They should be taught C++

#include "XFactory.h"
#include "XTrace.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILEOGG::SNDFILEOGG() : SNDFILE()
* @brief      Constructor of class
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILEOGG::SNDFILEOGG() : SNDFILE()
{
  Clean();

  type = SNDFILE_TYPE_OGG;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SNDFILEOGG::~SNDFILEOGG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
SNDFILEOGG::~SNDFILEOGG()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SNDFILEOGG::LoadFile()
* @brief      Load file
* @ingroup    SOUND
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SNDFILEOGG::LoadFile()
{
  XFILE* xfile = NULL;

  xbuffer = GEN_NEW XBUFFER(false);
  if(!xbuffer)
    {
      return false;
    }

  bool  status = false;

  xfile=GEN_XFACTORY.Create_File();
  if(!xfile)
    {
      xbuffer->Delete();
      GEN_DELETE xbuffer;
      xbuffer = NULL;
      return false;
    }

  if(!xfile->Open(path))
    {
      xbuffer->Delete();
      GEN_DELETE xbuffer;
      xbuffer = NULL;

      GEN_DELETE xfile;
      xfile = NULL;

      return false;
    }

  //XTRACE_PRINTCOLOR(0,_L("Loading file %s"), xfile->GetPathNameFile());

  xbuffer->Resize((XDWORD)xfile->GetSize());
  status = xfile->Read(xbuffer->Get(), xbuffer->GetSize());
  xfile->Close();

  GEN_XFACTORY.Delete_File(xfile);

  // now perform decoding
  stb_vorbis_info info;

  stream = stb_vorbis_open_memory(xbuffer->Get(), xbuffer->GetSize(), NULL, NULL);
  if(!stream)
    {
      XTRACE_PRINTCOLOR(4, _L("[SND File OGG] File Load Failed: %s"), xfile->GetPathNameFile());
      return false; // need to check the specific error
    }

  info = stb_vorbis_get_info(stream);

  channels = info.channels;
  samplerate = info.sample_rate;
  nsamples = stb_vorbis_stream_length_in_samples(stream)*channels; // this product is possibly redundant

  duration = (int)(stb_vorbis_stream_length_in_seconds(stream)*1000);

  xbufferdecodeddata = GEN_NEW XBUFFER(false);
  if(!xbufferdecodeddata)
    {
      return false;
    }

  xbufferdecodeddata->Resize(nsamples*2); // we must multiply by 2 because ogg expects shorts

  // perform the actual decoding
  int read = stb_vorbis_get_samples_short_interleaved(stream, channels, (short*)(xbufferdecodeddata->Get()), nsamples);

  // try to deallocate the allocacated memory
  stb_vorbis_close(stream);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SNDFILEOGG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SOUND
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SNDFILEOGG::Clean()
{
  stream  = NULL;
}


