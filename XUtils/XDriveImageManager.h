/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XDriveImageManager.h
* 
* @class      XDRIVEIMAGEMANAGER
* @brief      eXtended Utils Drive Image Manager class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XPath.h"
#include "XVector.h"
#include "XSubject.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XDRIVEIMAGEMANAGER_ERROR
{
  XDRIVEIMAGEMANAGER_ERROR_NONE             = 0 ,

  XDRIVEIMAGEMANAGER_ERROR_READSECTORS          ,
  XDRIVEIMAGEMANAGER_ERROR_WRITESECTORS         ,

  XDRIVEIMAGEMANAGER_ERROR_READIMGFILE          ,
  XDRIVEIMAGEMANAGER_ERROR_WRITEIMGFILE         ,
};

#define XDRIVEIMAGEMANAGER_DEFAULT_NSECTORPERBLOCK     100000

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XDRIVEIMAGEMANAGER_DRIVE
{
  public:

                                XDRIVEIMAGEMANAGER_DRIVE          ();
    virtual                    ~XDRIVEIMAGEMANAGER_DRIVE          ();

    XSTRING*                    GetName                           ();
    XSTRING*                    GetLabel                          ();
    XPATH*                      GetPath                           ();

    int                         GetVolume                         ();
    void                        SetVolume                         (int volume);

    XDWORD                      GetDeviceID                       ();
    void                        SetDeviceID                       (XDWORD deviceID);

    XQWORD                      GetID                             ();
    void                        SetID                             (XQWORD ID);

    XQWORD                      GetSectorSize                     ();
    void                        SetSectorSize                     (XQWORD sectorsize);

    XQWORD                      GetNumberSectors                  ();
    void                        SetNumberSectors                  (XQWORD numbersectors);

    XQWORD                      GetSize                           ();

    XDWORD                      GetSizeInGB                       ();

    XDWORD                      GetHandle                         ();
    void                        SetHandle                         (XDWORD handle);

    XDWORD                      GetRawHandle                      ();
    void                        SetRawHandle                      (XDWORD handle);

  private:

    void                        Clean                             ();

    XSTRING                     name;
    XSTRING                     label;
    XPATH                       path;
    int                         volume;
    XDWORD                      deviceID;
    XQWORD                      ID;
    XQWORD                      sectorsize;
    XQWORD                      numbersectors;
    XDWORD                      handle;
    XDWORD                      rawhandle;
};

class XDRIVEIMAGEMANAGER : public XSUBJECT
{
  public:
                                XDRIVEIMAGEMANAGER                ();
    virtual                    ~XDRIVEIMAGEMANAGER                (); 

    static bool                 GetIsInstanced                    ();
    static XDRIVEIMAGEMANAGER&  GetInstance                       ();
    static bool                 SetInstance                       (XDRIVEIMAGEMANAGER* instance);
    static bool                 DelInstance                       ();


    virtual bool                GetDrives                         (XVECTOR<XDRIVEIMAGEMANAGER_DRIVE*>& drives);

    virtual bool                Open                              (XDRIVEIMAGEMANAGER_DRIVE& drive);

    virtual bool                Read                              (XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer);
    virtual bool                Write                             (XDRIVEIMAGEMANAGER_DRIVE& drive, XQWORD startsector, XQWORD numbersectors, XBUFFER& buffer);

    bool                        ReadToIMGFile                     (XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock = XDRIVEIMAGEMANAGER_DEFAULT_NSECTORPERBLOCK);
    bool                        WriteFromIMGFile                  (XDRIVEIMAGEMANAGER_DRIVE& drive, XPATH& pathfileIMG, XQWORD nsectorsperblock = XDRIVEIMAGEMANAGER_DEFAULT_NSECTORPERBLOCK);

    float                       GetLastOperationPercent           ();
    XDRIVEIMAGEMANAGER_ERROR    GetLastError                      ();

    virtual bool                Close                             (XDRIVEIMAGEMANAGER_DRIVE& drive);

  private:

    void                        Clean                             ();


    static XDRIVEIMAGEMANAGER*  instance;

    float                       lastoperationpercent;
    XDRIVEIMAGEMANAGER_ERROR    lasterror;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





