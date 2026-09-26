/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIORemoteFileCFG.h
* 
* @class      DIOREMOTEFILECFG
* @brief      Data Input/Output Remote File CFG class
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

#include <stdlib.h> 

#include "XFactory.h"
#include "XFile.h"
#include "XFileCFG.h"

#include "DIOURL.h"
#include "DIOPing.h"
#include "DIOWebClient.h"
#include "DIOPublicInternetIP.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

#define DIOREMOTEFILECFG_PREFIXNAMEFILE    __L("remote_")

#define DIOREMOTEFILECFG_SECTIONGENERAL    __L("general")
#define DIOREMOTEFILECFG_URLREMOTECFG      __L("urlremotecfg")

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class DIOREMOTEFILECFG : public XFILECFG
{
  public:
                                        DIOREMOTEFILECFG          (XCHAR* namefile = NULL);
    virtual                            ~DIOREMOTEFILECFG          ();

    template<class T>
    bool                                Ini                       (bool remoteactive = true)
                                        {
                                          bool connexionstatus = false;
                                          bool checkremote     = remoteactive;
 
                                          if(!webclient)         
                                            {
                                              return false;
                                            }

                                          if(checkremote)
                                            {                                              
                                              if(namefile.IsEmpty()) 
                                                {
                                                  return false;
                                                }
                                                                                               
                                              if(!GEN_DIOPUBLICINTERNETIP.FastIsThereInternetConnection())                                              
                                                {
                                                  checkremote = false;
                                                }
                                            }

                                          XPATH  xpathroot;
                                          XPATH  xpathremotefile;
                                          DIOURL downloadURL;
                                          bool   status[2] = { false, false };

                                          DoVariableMapping();

                                          if(checkremote)
                                            {
                                              AddValue(XFILECFG_VALUETYPE_STRING, DIOREMOTEFILECFG_SECTIONGENERAL, DIOREMOTEFILECFG_URLREMOTECFG, &URLremoteCFG);
                                            }

                                          DoDefault();

                                          if(checkremote) 
                                            {
                                              GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

                                              xpathfile.Set(xpathroot.Get());

                                              if(!xpathfile.IsEmpty()) 
                                                {
                                                  xpathfile.Slash_Add();
                                                }

                                              xpathfile.Add(namefile.Get());
                                              xpathfile.Add(XFILECFG_EXTENSIONFILE);                                                                                  
                                            }   

                                          status[0] = Load();

                                          LoadReadjustment();
                                           
                                          if(checkremote)
                                            {                                           
                                              if(!URLremoteCFG.IsEmpty())
                                                {
                                                  downloadURL.Set(URLremoteCFG.Get());      
                                                  if(downloadURL.Find(__L("?"), false) == XSTRING_NOTFOUND)
                                                    {      
                                                      downloadURL.Slash_Add();
                                                      downloadURL.Add(DIOREMOTEFILECFG_PREFIXNAMEFILE);
                                                      downloadURL.Add(namefile.Get());
                                                      downloadURL.Add(XFILECFG_EXTENSIONFILE);
                                                    }

                                                  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
                                                  xpathremotefile.Set(xpathroot.Get());
                                                  if(!xpathremotefile.IsEmpty()) xpathremotefile.Slash_Add();
                                                  xpathremotefile.Add(DIOREMOTEFILECFG_PREFIXNAMEFILE);
                                                  xpathremotefile.Add(namefile.Get());
                                                  xpathremotefile.Add(XFILECFG_EXTENSIONFILE);

                                                  if(webclient->Get(downloadURL, xpathremotefile))
                                                    {
                                                      T* remotefileCFG = &T::GetInstance(false);
                                                      if(remotefileCFG)
                                                        {
                                                          remotefileCFG->GetPathFile()->Set(xpathremotefile); 

                                                          if(remotefileCFG->template Ini<T>(false))
                                                            {                                                               
                                                              AjustValuesFromRemote<T>(remotefileCFG); 
                                                              
                                                              remotefileCFG->End();
                                                            }

                                                          T::DelInstance();
                                                        }    
     
                                                      if(status[0])
                                                        {                                                                                                                      
                                                          XFILE* xfile=GEN_XFACTORY.Create_File();
                                                          if(xfile)
                                                            {
                                                              xfile->Erase(xpathremotefile, true);
                                                              GEN_XFACTORY.Delete_File(xfile);
                                                            }                                                          
                                                        }          
                                                    }
                                                }
                                             }

                                          if(checkremote) 
                                            {
                                              status[1] = Save();
                                            }
                                           else
                                            {
                                              status[1] = true;                        
                                            }

                                          return (status[0] && status[1]);
                                        }

    XSTRING*                            GetURLRemoteCFG           ();

    template<class T>
    bool                                AjustValuesFromRemote     (XFILECFG* remotefileCFG)
                                        {
                                          if(!remotefileCFG)
                                            {
                                              return false;
                                            }
  
                                          for(XDWORD c=1; c<GetValues()->GetSize()+1; c++)
                                            {  
                                              XFILECFGVALUE* localvalue = GetValues()->Get(c);
                                              if(localvalue)
                                                {
                                                  XFILECFGVALUE* remotevalue = remotefileCFG->GetCFGValue(localvalue->GetGroup()->Get(), localvalue->GetID()->Get());
                                                  if(remotevalue)
                                                    {
                                                      XVARIANT* value = remotefileCFG->GetValue(remotevalue);
                                                      if(value)
                                                        {
                                                          SetValue(localvalue, value);

                                                          value->Set();  
                                                          delete value;
                                                        }
                                                    }
                                                }
                                            }

                                          for(XDWORD c=1; c<remotefileCFG->GetValues()->GetSize()+1; c++)
                                            {  
                                              XFILECFGVALUE* remotevalue = remotefileCFG->GetValues()->Get(c);
                                              if(remotevalue)
                                                {
                                                  if(!remotevalue->GetIDBasic()->IsEmpty() && remotevalue->GetIndexSecuence() == 1)           
                                                    {
                                                      XFILECFGVALUE* localvalue = GetCFGValue(remotevalue->GetGroup()->Get(), remotevalue->GetID()->Get());  
                                                      if(localvalue)
                                                        {             
                                                          if(!localvalue->GetIDBasic()->Compare(remotevalue->GetIDBasic()->Get(), true))
                                                            {   
                                                              int difference = (remotevalue->GetNSecuences() - localvalue->GetNSecuences());

                                                              if(difference < 0)
                                                                {
                                                                  bool activedelete = (localvalue->GetModeRemoteMix() & XFILECFG_MODEREMOTEMIX_NOTDELADDKEYS)?false:true;
                                                                  if(activedelete)
                                                                    {
                                                                      difference = abs(difference);                                                                      

                                                                      for(int c=0; c<difference; c++) 
                                                                        {
                                                                          XSTRING key;
                                                                          
                                                                          GenerateKeySecuence(remotevalue->GetIDBasic()->Get(), remotevalue->GetMask()->Get(), remotevalue->GetNSecuences()+c+1, key);

                                                                          DelCFGValue(localvalue->GetGroup()->Get(), key.Get());
                                                                        }
                                                                    }
                                                                }
                                                               else
                                                                {
                                                                  if(difference > 0)
                                                                    {
                                                                      for(XDWORD c=localvalue->GetNSecuences(); c<remotevalue->GetNSecuences()+1; c++) 
                                                                        {                                                                                           
                                                                          XSTRING key;
                                                                          GenerateKeySecuence(remotevalue->GetIDBasic()->Get(), remotevalue->GetMask()->Get(), c, key);
                                                       
                                                                          XVARIANT* value = remotefileCFG->GetValue(remotevalue->GetGroup()->Get(), key.Get());
                                                                          if(value)
                                                                            {                                                                               
                                                                              void* _value  = NULL;

                                                                              switch(remotevalue->GetType())
                                                                                {
                                                                                  case XFILECFG_VALUETYPE_UNKNOWN   : break;

                                                                                  case XFILECFG_VALUETYPE_INT       : { XVECTOR<int*>* vector = (XVECTOR<int*>*)(localvalue->GetValuesVector());
                                                                                                                        if(vector) 
                                                                                                                          {
                                                                                                                            int* integer = GEN_NEW int();  
                                                                                                                            if(integer)
                                                                                                                              {
                                                                                                                                (*integer) = (*(int*)(value->GetData()));
                                                                                                                                vector->Add(integer);
                                                                                                                                _value = (void*)(vector->GetLast());                                                                                                                                 
                                                                                                                              }
                                                                                                                          }
                                                                                                                      }
                                                                                                                      break;                                                                                 

                                                                                  case XFILECFG_VALUETYPE_FLOAT     : { XVECTOR<float*>* vector = (XVECTOR<float*>*)(localvalue->GetValuesVector());
                                                                                                                        if(vector) 
                                                                                                                          {
                                                                                                                            float* numberfloat = GEN_NEW float();  
                                                                                                                            if(numberfloat)
                                                                                                                              {
                                                                                                                                (*numberfloat) = (*(float*)(value->GetData()));
                                                                                                                                vector->Add(numberfloat);                                                                                                                            
                                                                                                                                _value = (void*)(vector->GetLast());                                                                                                                                 
                                                                                                                              }
                                                                                                                          }
                                                                                                                      }
                                                                                                                      break;

                                                                                  case XFILECFG_VALUETYPE_STRING    : { XVECTOR<XSTRING*>* vector = (XVECTOR<XSTRING*>*)(localvalue->GetValuesVector());
                                                                                                                        if(vector) 
                                                                                                                          { 
                                                                                                                            XSTRING* string = GEN_NEW XSTRING();
                                                                                                                            if(string)
                                                                                                                              {
                                                                                                                                XSTRING* string2 = (XSTRING*)value->GetData();  
                                                                                                                                string->Set(string2->Get());          
                                                                                                                                vector->Add(string);                                                                                                                            
                                                                                                                                _value = (void*)(vector->GetLast());                                                                                                                                 
                                                                                                                              }
                                                                                                                          }
                                                                                                                      }
                                                                                                                      break;

                                                                                  case XFILECFG_VALUETYPE_BOOLEAN   : { XVECTOR<bool*>* vector = (XVECTOR<bool*>*)(localvalue->GetValuesVector());
                                                                                                                        if(vector) 
                                                                                                                          { 
                                                                                                                            bool* boolean = GEN_NEW bool();  
                                                                                                                            if(boolean)
                                                                                                                              {      
                                                                                                                                (*boolean) = (*(bool*)(value->GetData()));        
                                                                                                                                vector->Add(boolean);                                                                                                                            
                                                                                                                                _value = (void*)(vector->GetLast());                                                                                                                                 
                                                                                                                              }
                                                                                                                          }
                                                                                                                      }
                                                                                                                      break;

                                                                                                         default    : break;  
                                                                                }

                                                                              if(_value)
                                                                                {
                                                                                  AddValue(localvalue->GetType(), remotevalue->GetGroup()->Get(), key.Get(), _value, remotevalue->GetRemarkText()->IsEmpty()?NULL:remotevalue->GetRemarkText()->Get(), remotevalue->GetRemarkXPos()); 
                                                                                }

                                                                              delete value;                                                                              
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }  
                                                    }
                                                }
                                            }

                                          return true;
                                        }

  protected:

    XSTRING                             URLremoteCFG;

  private:

    void                                Clean                     ();

    DIOWEBCLIENT*                       webclient;
};




/*---- INLINE FUNCTIONS + PROTOTYPES ---------------------------------------------------------------------------------*/





