/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDOpenALBuffer.h
* 
* @class      SNDOPENALBUFFER
* @brief      Sound OpenAL Buffer class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "AL/al.h"
#include "AL/alc.h"

#include "XBuffer.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SNDOPENALBUFFER
{
  public:
                    SNDOPENALBUFFER             ();       
    virtual        ~SNDOPENALBUFFER             ();
    
    void            Create                      ();
    void            Delete                      ();

    ALuint          GetHandle                   ();
    bool            SetHandle                   (ALuint handle);

    XBUFFER*        GetXBuffer                  ();

    bool            GenerateNote                (XDWORD frequency, XDWORD duration);
    XDWORD          GenerateNote                (XDWORD frequency, XDWORD duration, XDWORD samplerate);

    bool            Assign                      (XWORD channels, size_t size, XDWORD samplerate);

  private:

    void            Clean                       ();   
    
    ALuint          buffer;  
    XBUFFER         xbuffer;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/




