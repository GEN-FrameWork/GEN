/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       SNDFactory.h
* 
* @class      SNDFACTORY
* @brief      Sound Factory class
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

#include "XPath.h"
#include "XSubject.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define SNDFACTORY_UNDEFINED              -1
#define SNDFACTORY_MAXTIMEOUT_INFINITE     0
#define SNDFACTORY_INLOOP                 -1

class SNDITEM;
typedef bool (*SNDFACTORY_WAITFUNCTION)(SNDITEM* item);

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class SNDPLAYCFG;

class SNDFACTORY : public XSUBJECT
{
  public:
                                      SNDFACTORY                  ();
    virtual                          ~SNDFACTORY                  ();

    static bool                       GetIsInstanced              ();
    static SNDFACTORY&                GetInstance                 ();
    static bool                       SetInstance                 (SNDFACTORY* instance);
    static bool                       DelInstance                 ();
    
    virtual bool                      Ini                         ();    
    virtual bool  		    						Update										  ();    
    virtual bool                      End                         (); 

    bool                              IsSoundActive               ();         
  
    virtual bool                      Sound_Play                  (SNDITEM* item, SNDPLAYCFG* playCFG = NULL, int ntimestoplay = 1);
    virtual bool                      Sound_Pause                 (SNDITEM* item);    
    virtual bool                      Sound_Stop                  (SNDITEM* item);   
    virtual bool                      Sound_StopAll               ();  
    virtual bool                      Sound_WaitToEnd             (SNDITEM* item, int maxtimeout = SNDFACTORY_MAXTIMEOUT_INFINITE, SNDFACTORY_WAITFUNCTION waitfunction = NULL);  
    virtual bool                      Sound_WaitAllToEnd          (int maxtimeout = SNDFACTORY_MAXTIMEOUT_INFINITE, SNDFACTORY_WAITFUNCTION waitfunction = NULL); 
    virtual int                       Sound_GetVolume             (SNDITEM* item);
    virtual bool                      Sound_SetVolume             (SNDITEM* item, int volume);   
    virtual float                     Sound_GetPitch              (SNDITEM* item);
    virtual bool                      Sound_SetPitch              (SNDITEM* item, float pitch);   

    virtual bool                      DeleteAllItems              (); 
        
    int                               MasterVolume_Get            ();
    bool                              MasterVolume_Set            (int volume);   

    SNDITEM*                          CreateItem                  (XCHAR* path);  
    SNDITEM*                          CreateItem                  (XPATH& xpath);     
    SNDITEM*                          CreateItem                  (XDWORD frecuency, XDWORD duration);  

    XVECTOR<SNDITEM*>*                GetItems                    ();
        
    bool                              Sound_IsAnyPlaying          ();
    bool                              Sound_IsAnyActive           ();

  protected: 

    bool                              soundactive;
                
  private:

    void                              Clean                       ();

    static SNDFACTORY*                instance;

    
    int                               mastervolume;
    XVECTOR<SNDITEM*>                 sounditems;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/






