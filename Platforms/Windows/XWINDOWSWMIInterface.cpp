/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSWMIInterface.cpp
* 
* @class      XWINDOWSWMIINTERFACE
* @brief      WINDOWS eXtended Utils Management Instrumentation Interface class
* @ingroup    PLATFORM_WINDOWS
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

#include "XWINDOWSWMIInterface.h"

#define _WIN32_DCOM

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <Propvarutil.h>

#include "XTrace.h"

#include "XWINDOWSRegistryManager.h"

#include "XMemory_Control.h"

using namespace std;
#pragma comment(lib, "wbemuuid.lib")



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE_RESULT::XWINDOWSWMIINTERFACE_RESULT()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE_RESULT::XWINDOWSWMIINTERFACE_RESULT()
{
  Clean();

  DeleteAllResults();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE_RESULT::~XWINDOWSWMIINTERFACE_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE_RESULT::~XWINDOWSWMIINTERFACE_RESULT()
{
  DeleteAllResults();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XSTRING*>* XWINDOWSWMIINTERFACE_RESULT::GetResultsString()
* @brief      Get results string
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XVECTOR<XSTRING*>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>* XWINDOWSWMIINTERFACE_RESULT::GetResultsString()
{
  return &resultsstring;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XVARIANT*>* XWINDOWSWMIINTERFACE_RESULT::GetResultsVariant()
* @brief      Get results variant
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XVECTOR<XVARIANT*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XVARIANT*>* XWINDOWSWMIINTERFACE_RESULT::GetResultsVariant()
{
  return &resultsvariant;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWMIINTERFACE_RESULT::DeleteAllResults()
* @brief      Delete all results
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWMIINTERFACE_RESULT::DeleteAllResults()
{
  resultsstring.DeleteContents();
  resultsstring.DeleteAll();

  resultsvariant.DeleteContents();
  resultsvariant.DeleteAll();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE_ERROR XWINDOWSWMIINTERFACE_RESULT::GetError()
* @brief      Get error
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XWINDOWSWMIINTERFACE_ERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE_ERROR XWINDOWSWMIINTERFACE_RESULT::GetError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWMIINTERFACE_RESULT::SetError(XWINDOWSWMIINTERFACE_ERROR error)
* @brief      Set error
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  error : Error value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWMIINTERFACE_RESULT::SetError(XWINDOWSWMIINTERFACE_ERROR error)
{
  this->error = error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XWINDOWSWMIINTERFACE_RESULT::GetErrorDescription()
* @brief      Get error description
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XWINDOWSWMIINTERFACE_RESULT::GetErrorDescription()
{
  return &errordescription;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWMIINTERFACE_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWMIINTERFACE_RESULT::Clean()
{
  error = XWINDOWSWMIINTERFACE_ERROR_NONE;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE::XWINDOWSWMIINTERFACE()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE::XWINDOWSWMIINTERFACE()
{
  Clean();

  rootdir = __L("ROOT\\CIMV2");
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE::~XWINDOWSWMIINTERFACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE::~XWINDOWSWMIINTERFACE()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XWINDOWSWMIINTERFACE::GetRootDir()
* @brief      get root dir
* @ingroup    PLATFORM_WINDOWS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XWINDOWSWMIINTERFACE::GetRootDir()
{
  return &rootdir;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::Ini()
* @brief      Initialize the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::Ini()
{
  HRESULT hres;

  hres = CoInitializeEx(0, COINIT_MULTITHREADED);
  if(FAILED(hres))
    {
      return false;
    }
   else
    {
      //-----------------------------------------------------------------------------------------
      // Step 2: Set general COM security levels

      hres = CoInitializeSecurity(NULL,
                                  -1,                            // COM authentication
                                  NULL,                          // Authentication services
                                  NULL,                          // Reserved
                                  RPC_C_AUTHN_LEVEL_DEFAULT,     // Default authentication
                                  RPC_C_IMP_LEVEL_IMPERSONATE,   // Default Impersonation
                                  NULL,                          // Authentication info
                                  EOAC_NONE,                     // Additional capabilities
                                  NULL);                         // Reserved
      if(hres == RPC_E_TOO_LATE)
        {
          return true;
        }

      if(FAILED(hres))
        {
          return false;
        }
    }


  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSWMIINTERFACE_RESULT* XWINDOWSWMIINTERFACE::DoQuery(XCHAR* query, XCHAR* propertynameofresultobject, bool allowemptyitems)
* @brief      Do query
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  query : Query pointer to use.
* @param[in]  propertynameofresultobject : Output propertynameofresultobject.
* @param[in]  allowemptyitems : Allowemptyitems value.
* 
* @return     XWINDOWSWMIINTERFACE_RESULT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSWMIINTERFACE_RESULT* XWINDOWSWMIINTERFACE::DoQuery(XCHAR* query, XCHAR* propertynameofresultobject, bool allowemptyitems)
{

  XWINDOWSWMIINTERFACE_RESULT* result       = NULL;
  HRESULT                      hres;
  IWbemLocator*                ploc         = NULL;
  IWbemServices*               psvc         = NULL;
  IEnumWbemClassObject*        penumerator  = NULL;


  result = GEN_NEW XWINDOWSWMIINTERFACE_RESULT();
  if(!result) return NULL;

  //-----------------------------------------------------------------------------------------
  // Step 3: Obtain the initial locator to WMI

  ploc = NULL;

  hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&ploc);
  if(FAILED(hres))
    {
      result->SetError(XWINDOWSWMIINTERFACE_ERROR_IWBEMLOCATORFAILURE);
      result->GetErrorDescription()->Format(__L("Failed to create IWbemLocator object. Error code: %d"), hres);
    }
   else
    {
      //-----------------------------------------------------------------------------------------
      // Step 4: Connect to WMI through the IWbemLocator::ConnectServer method

      psvc = NULL;

      // Connect to the root\cimv2 namespace with the current user and obtain pointer pSvc
      // to make IWbemServices calls.
      hres = ploc->ConnectServer(rootdir.Get(),  // Object path of WMI namespace
                                 NULL,           // User name. NULL = current user
                                 NULL,           // User password. NULL = current
                                 NULL,           // Locale. NULL indicates current
                                 0,              // Security flags
                                 NULL,           // Authority (for example, Kerberos)
                                 NULL,           // Context object
                                 &psvc);         // pointer to IWbemServices proxy

        // Connected to ROOT\\CIMV2 WMI namespace
        if(FAILED(hres))
          {
            result->SetError(XWINDOWSWMIINTERFACE_ERROR_IWBEMSERVICECONNECTIONFAILURE);
            result->GetErrorDescription()->Format(__L("Could not connect to Wbem service. Error code: %d"), hres);
          }
          else
          {
            //-----------------------------------------------------------------------------------------
            // Step 5: Set security levels on the proxy

            hres = CoSetProxyBlanket(psvc,                        // Indicates the proxy to set
                                     RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                                     RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                                     NULL,                        // Server principal name
                                     RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                                     RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                                     NULL,                        // client identity
                                     EOAC_NONE);                  // proxy capabilities

            if(FAILED(hres))
              {
                result->SetError(XWINDOWSWMIINTERFACE_ERROR_BLANKETPROXYSETFAILURE);
                result->GetErrorDescription()->Format(__L("Could not set proxy blanket. Error code: %d"), hres);
              }
              else
              {
                //-----------------------------------------------------------------------------------------
                // Step 6: Use the IWbemServices pointer to make requests of WMI

                penumerator = NULL;
                hres = psvc->ExecQuery(bstr_t("WQL"), bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &penumerator);

                if(FAILED(hres))
                  {
                    result->SetError(XWINDOWSWMIINTERFACE_ERROR_BADQUERYFAILURE);
                    result->GetErrorDescription()->Format(__L("Bad query. Error code: %d"), hres);
                  }
                  else
                  {
                    //-----------------------------------------------------------------------------------------
                    // Step 7:  Get the data from the query in step 6

                    #define MAXBLOCK_PCLSOBJ   500

                    HRESULT           hr        = WBEM_S_NO_ERROR;
                    IWbemClassObject* pclsobj[MAXBLOCK_PCLSOBJ];
                    VARIANT           vtprop;
                    ULONG             ureturned = 0;

                    while( hr == WBEM_S_NO_ERROR)
                      {
                        hr = penumerator->Next(WBEM_INFINITE, MAXBLOCK_PCLSOBJ, pclsobj, &ureturned);
                        if(SUCCEEDED(hr) && (ureturned!=0))
                          {
                            // VARIANT vtProp;
                            // Get the value of desired property
                            for( ULONG c=0; c<ureturned; c++)
                              {
                                hr = pclsobj[c]->Get(propertynameofresultobject, 0, &vtprop, 0, 0);
                                if(hr != S_OK)
                                  {
                                    result->SetError(XWINDOWSWMIINTERFACE_ERROR_PROPERTYEXTRACTIONFAILURE);
                                    result->GetErrorDescription()->Format(__L("Couldn't extract property: %s from result of query. Error code: %d"), propertynameofresultobject, hres);

                                    break;
                                  }
                                  else
                                  {
                                    switch(vtprop.vt)
                                      {
                                        case VT_NULL  : { XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr) 
                                                            {
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = NULL;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }
                                                        }
                                                        break;

                                        case VT_I2    : { XWORD data = vtprop.intVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromInt(data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }
                                                        }
                                                        break;

                                        case VT_I4    : { XDWORD data = vtprop.lVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromInt(data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }
                                                        }
                                                        break;

                                        case VT_BOOL  : { BOOL data = vtprop.lVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->Format(__L("%s"), (data?__L("true"):__L("false")));
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }

                                                        }
                                                        break;

                                        case VT_BSTR  : { BSTR val = vtprop.bstrVal;

                                                          // Sometimes val might be NULL even when result is S_OK
                                                          // Convert NULL to empty string (otherwise "std::wstring(val)" would throw exception)
                                                          if(val == NULL)
                                                            {
                                                              if(allowemptyitems)
                                                                {
                                                                  XSTRING* resultstr = GEN_NEW XSTRING();
                                                                  if(resultstr)
                                                                    {
                                                                      result->GetResultsString()->Add(resultstr);
                                                                    }

                                                                  XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                                  if(resultvariant) 
                                                                    {
                                                                      (*resultvariant) = (*resultstr);
                                                                      result->GetResultsVariant()->Add(resultvariant);
                                                                    }
                                                                }
                                                            }
                                                           else
                                                            {
                                                              XSTRING* resultstr = GEN_NEW XSTRING();
                                                              if(resultstr)
                                                                {
                                                                  (*resultstr) = (XCHAR*)val;
                                                                  result->GetResultsString()->Add(resultstr);
                                                                  
                                                                }

                                                              XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                              if(resultvariant) 
                                                                {
                                                                  (*resultvariant) = (*resultstr);
                                                                  result->GetResultsVariant()->Add(resultvariant);
                                                                }
                                                            }
                                                        }
                                                        break;

                                        case VT_UI1   : { XBYTE data = vtprop.bVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromDWord((XDWORD)data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }                                                        
                                                        }  
                                                        break;
                                        
                                        case VT_I1    : { char data = vtprop.cVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromInt((int)data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }                                                        
                                                        }  
                                                        break;   

                                        case VT_UI2   : { XWORD data = vtprop.uiVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromDWord((XWORD)data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }                                                        
                                                        }  
                                                        break;    

                                        case VT_UI4   : { XDWORD data = vtprop.ulVal;

                                                          XSTRING* resultstr = GEN_NEW XSTRING();
                                                          if(resultstr)
                                                            {
                                                              resultstr->ConvertFromDWord((XWORD)data);
                                                              result->GetResultsString()->Add(resultstr);
                                                            }

                                                          XVARIANT* resultvariant = GEN_NEW XVARIANT();
                                                          if(resultvariant) 
                                                            {
                                                              (*resultvariant) = data;
                                                              result->GetResultsVariant()->Add(resultvariant);
                                                            }                                                        
                                                        }  
                                                        break;    
                                      }

                                    VariantClear(&vtprop);
                                  }

                                  pclsobj[c]->Release();
                              }

                            if(ureturned < MAXBLOCK_PCLSOBJ) break;

                          } else break;
                      }
                  }
              }
          }
      }

  //-----------------------------------------------------------------------------------------
  // Cleanup

  if(psvc)          psvc->Release();
  if(ploc)          ploc->Release();
  if(penumerator)   penumerator->Release();

  return result;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XSTRING& answer)
* @brief      Do query
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  _class : Class pointer to use.
* @param[in]  namedata : Namedata pointer to use.
* @param[in]  answer : Answer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XSTRING& answer)
{
  XWINDOWSWMIINTERFACE_RESULT*  result       = NULL;
  bool                          status       = false;

  answer.Empty();

  XSTRING xstringclass;

  xstringclass.Format(__L("SELECT * FROM %s"), _class);

  result = DoQuery(xstringclass.Get(), namedata);
  if(result)
    {
      if(result->GetError() == XWINDOWSWMIINTERFACE_ERROR_NONE)
        {
          if(result->GetResultsString()->GetSize())
            {
              answer = (XCHAR*)result->GetResultsString()->Get(0)->Get();
              status = true;
            }
        }
    }

  GEN_DELETE result;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVECTOR<XSTRING*>* answers)
* @brief      Do query
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  _class : Class pointer to use.
* @param[in]  namedata : Namedata pointer to use.
* @param[in]  answers : Answers pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVECTOR<XSTRING*>* answers)
{
  XWINDOWSWMIINTERFACE_RESULT*  result       = NULL;
  bool                          status       = false;

  XSTRING xstringclass;

  xstringclass.Format(__L("SELECT * FROM %s"), _class);

  result = DoQuery(xstringclass.Get(), namedata);
  if(result)
    {
      if(result->GetError() == XWINDOWSWMIINTERFACE_ERROR_NONE)
        {
          if(result->GetResultsString()->GetSize())
            {
              for(XDWORD c=0; c<result->GetResultsString()->GetSize(); c++)
                {
                  XSTRING* resultstring = GEN_NEW XSTRING();
                  if(resultstring)
                    {
                      (*resultstring) = (XCHAR*)result->GetResultsString()->Get(c)->Get();
                      answers->Add(resultstring);
                    }
                }

              status = true;
            }
        }
    }

  GEN_DELETE result;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVARIANT& answer)
* @brief      Do query
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  _class : Class pointer to use.
* @param[in]  namedata : Namedata pointer to use.
* @param[in]  answer : Answer value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVARIANT& answer)
{
  XWINDOWSWMIINTERFACE_RESULT*  result       = NULL;
  bool                          status       = false;

  answer = NULL;

  XSTRING xstringclass;

  xstringclass.Format(__L("SELECT * FROM %s"), _class);

  result = DoQuery(xstringclass.Get(), namedata);
  if(result)
    {
      if(result->GetError() == XWINDOWSWMIINTERFACE_ERROR_NONE)
        {
          if(result->GetResultsString()->GetSize())
            {
              answer = (XVARIANT)(*result->GetResultsVariant()->Get(0));
              status = true;
            }
        }
    }

  GEN_DELETE result;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVECTOR<XVARIANT*>* answers)
* @brief      Do query
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  _class : Class pointer to use.
* @param[in]  namedata : Namedata pointer to use.
* @param[in]  answers : Answers pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::DoQuery(XCHAR* _class, XCHAR* namedata, XVECTOR<XVARIANT*>* answers)
{
  XWINDOWSWMIINTERFACE_RESULT*  result = NULL;
  bool                          status = false;

  XSTRING xstringclass;

  xstringclass.Format(__L("SELECT * FROM %s"), _class);

  result = DoQuery(xstringclass.Get(), namedata);
  if(result)
    {
      if(result->GetError() == XWINDOWSWMIINTERFACE_ERROR_NONE)
        {
          if(result->GetResultsString()->GetSize())
            {
              for(XDWORD c=0; c<result->GetResultsString()->GetSize(); c++)
                {
                  XVARIANT* resultvariant = GEN_NEW XVARIANT();
                  if(resultvariant)
                    {
                      (*resultvariant) = (XVARIANT)(*result->GetResultsVariant()->Get(c));
                      answers->Add(resultvariant);
                    }
                }

              status = true;
            }
        }
    }

  GEN_DELETE result;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::NetWorkInterfaceEnable(int ID, bool enable)
* @brief      Net work interface enable
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  enable : Enable value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::NetWorkInterfaceEnable(int ID, bool enable)
{
  IWbemLocator*   ploc = NULL;
  HRESULT         hres;
  bool            status = true;

  hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &ploc);
  if(FAILED(hres)) return false;

  // Connect to WMI through the IWbemLocator::ConnectServer method
  IWbemServices* psvc = NULL;

  // Connect to the root\\CIMV2 namespace
  // and obtain pointer pSvc to make IWbemServices calls.
  hres = ploc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                             NULL,                      // User name. NULL = current user
                             NULL,                      // User password. NULL = current
                             0,                         // Locale. NULL indicates current
                             NULL,                      // Security flags
                             0,                         // Authority (e.g. Kerberos)
                             0,                         // Context object
                             &psvc);                    // pointer to IWbemServices proxy
  if(FAILED(hres))
    {
      ploc->Release();
      return false;
    }

  hres = CoSetProxyBlanket(psvc,                        // Indicates the proxy to set
                           RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                           RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                           NULL,                        // Server principal name
                           RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                           RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                           NULL,                        // client identity
                           EOAC_NONE);                  // proxy capabilities


  if(FAILED(hres))
    {
      psvc->Release();
      ploc->Release();
      return false;
    }

  BSTR methodname = SysAllocString(enable?__L("Enable"):__L("Disable"));
  BSTR classname  = SysAllocString(L"Win32_NetworkAdapter");

  IWbemClassObject* pclass = NULL;
  hres = psvc->GetObject(classname, 0, NULL, &pclass, NULL);

  hres = pclass->GetMethod(methodname, 0, NULL, NULL);

  // Execute Method
  XSTRING           devicename;
  IWbemClassObject* poutparams = NULL;

  devicename.Format(__L("Win32_NetworkAdapter.DeviceID=\"%d\""), ID);

  hres = psvc->ExecMethod(devicename.Get(), methodname, 0, NULL, NULL /*pClassInstance*/, &poutparams, NULL);
  if(FAILED(hres)) status = false;

  SysFreeString(classname);
  SysFreeString(methodname);

  if(pclass)     pclass->Release();
  if(poutparams) poutparams->Release();
  if(ploc)       ploc->Release();
  if(psvc)       psvc->Release();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::NetWorkInterfaceSetMetric(int ID, int metric)
* @brief      Net work interface set metric
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  ID : Identifier to use.
* @param[in]  metric : Metric value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::NetWorkInterfaceSetMetric(int ID, int metric)
{
  IWbemLocator*   ploc = NULL;
  HRESULT         hres;
  bool            status = true;
  

  hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &ploc);
  if(FAILED(hres)) return false;

  // Connect to WMI through the IWbemLocator::ConnectServer method
  IWbemServices* psvc = NULL;

  // Connect to the root\\CIMV2 namespace
  // and obtain pointer pSvc to make IWbemServices calls.
  hres = ploc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // Object path of WMI namespace
                             NULL,                      // User name. NULL = current user
                             NULL,                      // User password. NULL = current
                             0,                         // Locale. NULL indicates current
                             NULL,                      // Security flags
                             0,                         // Authority (e.g. Kerberos)
                             0,                         // Context object
                             &psvc);                    // pointer to IWbemServices proxy
  if(FAILED(hres))
    {
      ploc->Release();
      return false;
    }

  hres = CoSetProxyBlanket(psvc,                        // Indicates the proxy to set
                           RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
                           RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
                           NULL,                        // Server principal name
                           RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
                           RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
                           NULL,                        // client identity
                           EOAC_NONE);                  // proxy capabilities


  if(FAILED(hres))
    {
      psvc->Release();
      ploc->Release();
      return false;
    }

  
  // Execute Method

  BSTR              methodname = SysAllocString(L"SetIPConnectionMetric");
  BSTR              classname  = SysAllocString(L"Win32_NetWorkAdapterConfiguration"); 
  XSTRING           devicename;
  IWbemClassObject* poutparams  = NULL;
  IWbemClassObject* pinparams   = NULL;


  IWbemClassObject* pclass = NULL;
  hres = psvc->GetObject(classname, 0, NULL, &pclass, NULL);

  hres = pclass->GetMethod(methodname, 0, &pinparams, NULL);  
 
  devicename.Format(__L("Win32_NetWorkAdapterConfiguration.Index=\"%d\""), ID);

  hres = pclass->SpawnInstance(0, &pinparams);
  if(FAILED(hres)) status = false;
  	
  if(status)
    {
      VARIANT vararg;

	    VariantInit(&vararg);

      //V_VT(&vararg)    = VT_BSTR;
	    //V_BSTR(&vararg)  = SysAllocString(metricstr.Get());      

      vararg.vt   = VT_I4;
      vararg.iVal = metric;

	    hres = pinparams->Put(L"IPConnectionMetric", 0, &vararg, CIM_UINT32);
      if(FAILED(hres)) status = false;
  	
      if(status)
        {
          hres = psvc->ExecMethod(devicename.Get(), methodname, 0, NULL, pinparams, &poutparams, NULL); 
          if(FAILED(hres)) 
            {
              status = false;
            }
        }       
    }

  
  SysFreeString(classname);
  SysFreeString(methodname);

  if(pclass)     pclass->Release();
  if(pinparams)  pinparams->Release();
  if(poutparams) poutparams->Release();
  if(ploc)       ploc->Release();
  if(psvc)       psvc->Release();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::NetWorkInterfaceSetMetric(int metric)
* @brief      Net work interface set metric
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  metric : Metric value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::NetWorkInterfaceSetMetric(int metric)
{
  HRESULT hr; 

	// WMI connection

  IWbemLocator *pLoc = 0;

  hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &pLoc); 
  if(FAILED(hr)) 
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Failed to create IWbemLocator object. Error code [0x%08X]"), hr);
      return false;
    }

	IWbemServices *pSvc = 0;

  // Connect to the root\default namespace with the current user.

  hr = pLoc->ConnectServer( BSTR(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
  if(FAILED(hr))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Could not connect. Error code [0x%08X]"), hr);
      return false;      
    }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Connected to WMI"));

	// setting security for WMI
  // Set the proxy so that impersonation of the client occurs.

  hr = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
  if(FAILED(hr))
    {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Could not set proxy blanket. Error code [0x%08X]"), hr);
      return false;      
    }

	// retreive all the interfaces.

	BSTR                  Language = SysAllocString(L"WQL");
  BSTR                  Query    = SysAllocString(L"Select * From Win32_NetworkAdapterConfiguration Where IPEnabled = True");
	IEnumWbemClassObject* pEnum    = 0;

	hr = pSvc->ExecQuery(Language, Query, WBEM_FLAG_FORWARD_ONLY, 0, &pEnum);

	SysFreeString(Query);
  SysFreeString(Language);

	if(SUCCEEDED(hr))
    {
		  ULONG uTotal = 0;

		  // Retrieve the objects in the result set.

		  for(;;)
		    {
			    IWbemClassObject* pObj      = 0;
			    ULONG             uReturned = 0;

			    // 0 - time out, 1 - One object

			    hr = pEnum->Next(0, 1, &pObj, &uReturned);
			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Get(IPConnectionMetric). Error code [0x%08X]"), hr);				      
				      break;
      			}

			    uTotal += uReturned;

			    if(uReturned == 0)  break;


          // -------------------------------------------------------------------
			    // read Index

			    VARIANT vtPropIndex;

			    VariantInit(&vtPropIndex);

			    hr = pObj->Get(L"Index", 0, &vtPropIndex, 0, 0);

			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Get(Index). Error code [0x%08X]"), hr);					      
				      break;
      			}

			    unsigned int index = vtPropIndex.lVal;
          
    			VariantClear(&vtPropIndex);


          // -------------------------------------------------------------------
			    // read IPConnectionMetric

			    VARIANT vtProp;

			    VariantInit(&vtProp);

			    hr = pObj->Get(L"IPConnectionMetric", 0, &vtProp, 0, 0);

			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Get(IPConnectionMetric). Error code [0x%08X]"), hr);				      
				      break;
			      }

			    unsigned int IPConnectionMetric = vtProp.lVal;
          
			    VariantClear(&vtProp);


          // -------------------------------------------------------------------
			    // read Description

			    VARIANT vtPropDescription;

			    VariantInit(&vtPropDescription);

			    hr = pObj->Get(L"Description", 0, &vtPropDescription, 0, 0);

			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Get(Description). Error code [0x%08X]"), hr);				      
				      break;
			      }

          XSTRING description_str;

          description_str = vtPropDescription.bstrVal;

			    VariantClear(&vtProp);

         

          XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Index [%2d] Metric [%2d] [%s] "), index, IPConnectionMetric, description_str.Get());			 


          // -------------------------------------------------------------------
			    // Execute SetIPConnectionMetric 

			    IWbemClassObject* pClass    = NULL;
			    IWbemClassObject* pOutInst  = NULL;
			    IWbemClassObject* pInClass  = NULL;
			    IWbemClassObject* pInInst   = NULL;

			    // Get the class object

          BSTR Win32_NetWorkAdapterConfigurationStr = SysAllocString(L"Win32_NetWorkAdapterConfiguration");

    			hr = pSvc->GetObject(Win32_NetWorkAdapterConfigurationStr, 0, NULL, &pClass, NULL);
			    if(hr != WBEM_S_NO_ERROR)
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("getting object Win32_NetWorkAdapterConfiguration. Error code [0x%08X]"), hr);					      
				      break;
			      }


  	      // Get the input argument and set the property

			    hr = pClass->GetMethod(L"SetIPConnectionMetric", 0, &pInClass, NULL); 

			    if(hr != WBEM_S_NO_ERROR)
			      {				      
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("getting method SetIPConnectionMetric. Error code [0x%08X]"), hr);	
				      break;
			      }

			    hr = pInClass->SpawnInstance(0, &pInInst);
			    if(hr != WBEM_S_NO_ERROR)
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("spawning instance. Error code [0x%08X]"), hr);					      
				      break;
			      }


  
			    // Set up the query string with the adapter index we obtained above.

			    wchar_t dest[10];

			    memset((char*)dest, 0, 20);
			    _itow(index, dest, 10);

			    wstring objPath = L"Win32_NetWorkAdapterConfiguration.Index='" + wstring(dest);

			    objPath = objPath + L"'";

			    BSTR bstrObjPath = SysAllocString(objPath.c_str());

		      VARIANT varArg1;

    			VariantInit(&varArg1);

		    	//V_VT(&varArg1)    = VT_BSTR;
			    //V_BSTR(&varArg1)  = SysAllocString(L"2");

          varArg1.vt    = VT_I4;
          varArg1.iVal = metric;

          
			    hr = pInInst->Put(L"IPConnectionMetric", 0, &varArg1, CIM_UINT32);
			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Put(IPConnectionMetric). Error code [0x%08X]"), hr);							      
      				break;
			      }
          BSTR SetIPConnectionMetricStr = SysAllocString(L"SetIPConnectionMetric");
			    hr = pSvc->ExecMethod(bstrObjPath, SetIPConnectionMetricStr, 0, NULL, pInInst, &pOutInst, NULL);
			    if(hr != WBEM_S_NO_ERROR)
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("executing SetIPConnectionMetric. Error code [0x%08X]"), hr);							      				      
				      break;
			      }
    

			    // Get the EnableStatic method return value 

    			VARIANT ret_value;

		    	hr = pOutInst->Get(L"ReturnValue", 0, &ret_value, 0, 0);
			    if(FAILED(hr))
			      {
              XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("Get(ReturnValue). Error code [0x%08X]"), hr);							      
				      break;
			      }

			    long ret = V_I4(&ret_value);
    			if(ret != 0)
			      {
				      hr = E_FAIL;           
			      }

			    // Free up BSTR and VARIANT objects

			    VariantClear(&ret_value);
		      VariantClear(&varArg1);

			    pObj->Release();    // Release objects not owned.            
		    }
      
      XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Number of objects returned is %d"), uTotal);    
	  }
   else
	  {
      XTRACE_PRINTCOLOR(XTRACE_COLOR_RED, __L("querying WMI for network adapters. Error code [0x%08X]"), hr);	      
	  }

  XTRACE_PRINTCOLOR(XTRACE_COLOR_BLUE, __L("Done"));

  pSvc->Release();
  pLoc->Release();     
     
	return 0;   // Program successfully completed.
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::ConvertDateTimeToXDateTime(XSTRING& datetime, XDATETIME& xdatetime)
* @brief      convert date time to Xdate time
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  datetime : Datetime value.
* @param[in]  xdatetime : Xdatetime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::ConvertDateTimeToXDateTime(XSTRING& datetime, XDATETIME& xdatetime)
{
  XSTRING year;
  XSTRING month;
  XSTRING day;
  XSTRING hour;
  XSTRING min;
  XSTRING sec;

  xdatetime.SetToZero();

  if(datetime.GetSize() < 16)
    {
      return false;
    }

  datetime.Copy(0, 4, year);
  datetime.Copy(4, 6, month);
  datetime.Copy(6, 8, day);  
  datetime.Copy( 8, 10, hour);
  datetime.Copy(10, 12, min);
  datetime.Copy(12, 14, sec);

  xdatetime.SetYear(year.ConvertToInt());
  xdatetime.SetMonth(month.ConvertToInt());
  xdatetime.SetDay(day.ConvertToInt());
  xdatetime.SetHours(hour.ConvertToInt());
  xdatetime.SetMinutes(min.ConvertToInt());
  xdatetime.SetSeconds(sec.ConvertToInt());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSWMIINTERFACE::End()
* @brief      End the object
* @ingroup    PLATFORM_WINDOWS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSWMIINTERFACE::End()
{
  CoUninitialize();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSWMIINTERFACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSWMIINTERFACE::Clean()
{

}



