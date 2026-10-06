/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOScraperWeb.cpp
* 
* @class      DIOSCRAPERWEB
* @brief      Data Input/Output Scraper Web class
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWeb.h"

#include "XFactory.h"
#include "XTimer.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XFile.h"
#include "XFileXML.h"
#include "XFileJSON.h"
#include "XTrace.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStream.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOWebClient.h"
#include "DIOCheckInternetConnection.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEB::DIOSCRAPERWEB()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEB::DIOSCRAPERWEB()
{
  Clean();

  xml = GEN_NEW XFILEXML();
  if(!xml)  return;

  xmutexdo=GEN_XFACTORY.Create_Mutex();
  if(!xmutexdo) return;

  webclient = GEN_NEW DIOWEBCLIENT;
  if(!webclient) return;

  cache = GEN_NEW DIOSCRAPERWEBCACHE();
  if(!cache) return;

  URLdownload = GEN_NEW DIOURL();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOSCRAPERWEB::~DIOSCRAPERWEB()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEB::~DIOSCRAPERWEB()
{
  DeleteAllValues();

  if(URLdownload)
    {
      GEN_DELETE URLdownload;
    }

  if(cache)
    {
      cache->DeleteAll();
      GEN_DELETE cache;
    }

  if(xml)
    {
      if(xml->IsOpen()) xml->Close();
      GEN_DELETE xml;
    }

  if(webclient) GEN_DELETE webclient;

  if(xmutexdo) GEN_XFACTORY.Delete_Mutex(xmutexdo);

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::Load(XCHAR* namefile)
* @brief      Load
* @ingroup    DATAIO
* 
* @param[in]  namefile : Namefile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::Load(XCHAR* namefile)
{
  static bool webupdate = false;

  if(!xml)  return false;

  XPATH xpath;

  xpath.Empty();

  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRAPERWEB, xpath);
  if(!xpath.IsEmpty()) xpath.Slash_Add();
  xpath += DIOSCRAPERWEB_NAMEFILE;

  if((!webupdate) && (URLdownload))
    {
      webupdate = Download((*URLdownload), xpath);
    }

  if(!xml->Open(xpath, true))
    {
      GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_SCRAPERWEB, xpath);
      if(!xpath.IsEmpty()) xpath.Slash_Add();
      xpath += namefile;

      if(!xml->Open(xpath, true)) return false;
    }

  xml->ReadAndDecodeAllLines();

  xml->Close();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::Load(XPATH& namefile)
* @brief      Load
* @ingroup    DATAIO
* 
* @param[in]  namefile : Namefile value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::Load(XPATH& namefile)
{
  return Load(namefile.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOURL* DIOSCRAPERWEB::GetURLDownload()
* @brief      Get URL download
* @ingroup    DATAIO
* 
* @return     DIOURL* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOURL* DIOSCRAPERWEB::GetURLDownload()
{
  return URLdownload;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::Download(DIOURL& URL, XPATH& xpath)
* @brief      Download
* @ingroup    DATAIO
* 
* @param[in]  URL : URL to use.
* @param[in]  xpath : Path to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::Download(DIOURL& URL, XPATH& xpath)
{
  if(URL.IsEmpty())   return false;

  DIOURL downloadURL;
  bool   status;

  DIOWEBCLIENT* webclient = GEN_NEW DIOWEBCLIENT();
  if(!webclient)  return false;

  downloadURL.Set(URL.Get());
  downloadURL.Slash_Add();
  downloadURL.Add(DIOSCRAPERWEB_NAMEFILE);

  status = webclient->Get(downloadURL, xpath);

  GEN_DELETE webclient;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::Do(XCHAR* namewebservice, int timeoutforurl, XSTRING* localIP)
* @brief      Execute the operation
* @ingroup    DATAIO
* 
* @param[in]  namewebservice : Namewebservice pointer to use.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::Do(XCHAR* namewebservice, int timeoutforurl, XSTRING* localIP)
{
  if(!xml) return false;

  XFILEXMLELEMENT* root = xml->GetRoot();
  if(!root) return false;

  XSTRING namews;

  XFILEXMLELEMENT* nodewebservice = NULL;

  for(int c=0; c<root->GetNElements(); c++)
    {
      nodewebservice = root->GetElement(c);
      if(nodewebservice)
        {
          namews = nodewebservice->GetValueAttribute(_L("name"));
          if(!namews.Compare(namewebservice)) break;

          nodewebservice = NULL;
        }
    }

  if(nodewebservice)
    {
      DeleteAllValues();

      for(int c=0; c<nodewebservice->GetNElements(); c++)
        {
          bool priorityvaluesobtain = true;

          XFILEXMLELEMENT* nodeweb = nodewebservice->GetElement(c);
          if(nodeweb)
            {
              XBUFFER  webpage;
              DIOURL   url;

              if(!ChangeURL(nodeweb->GetValueAttribute(_L("url")), url))
                {
                  return false;
                }

              if(webclient->Get(url, webpage, DIOSCRAPERWEB_DEFAULTUSERAGENT, timeoutforurl, localIP))
                {
                  if(webpage.GetSize())
                    {
                      XSTRING format;

                      format = nodeweb->GetValueAttribute(_L("format"));

                      if(!format.Compare(_L("JSON"), true))
                        {
                          XFILEJSON* json = GEN_NEW XFILEJSON();
                          if(json)
                            {
                              json->DeleteAllLines();
                              json->AddBufferLines(XFILETXTFORMATCHAR_ASCII, webpage);
                              json->DecodeAllLines();

                              for(int d=0; d<nodeweb->GetNElements(); d++)
                                {
                                  XFILEXMLELEMENT* nodevalue = nodeweb->GetElement(d);
                                  if(nodevalue)
                                    {
                                      XSTRING  name;
                                      XSTRING  prioritystring;
                                      int      priority;
                                      XSTRING  namevalue;

                                      name           = nodevalue->GetValueAttribute(_L("name"));

                                      prioritystring = nodevalue->GetValueAttribute(_L("priority"));
                                      priority       = prioritystring.ConvertToInt();

                                      namevalue      = nodevalue->GetValueAttribute(_L("namevalue"));

                                      XFILEJSONVALUE* jsonvalue = json->GetValue(namevalue, NULL);
                                      if(jsonvalue)
                                        {
                                          switch(jsonvalue->GetType())
                                            {                                              
                                              case XFILEJSONVALUETYPE_INTEGER           : { int     pvalue = (int)jsonvalue->GetValueInteger();
                                                                                            XSTRING string;

                                                                                            string.Format(_L("%d"), pvalue);

                                                                                            AddValue(name, string);
                                                                                          }
                                                                                          break;

                                              case XFILEJSONVALUETYPE_DOUBLEINTEGER     : { long pvalue = (long)jsonvalue->GetValueDoubleInteger();
                                                                                            XSTRING string;

                                                                                            string.Format(_L("%ld"), pvalue);

                                                                                            AddValue(name, string);
                                                                                          }
                                                                                          break;

                                              case XFILEJSONVALUETYPE_FLOAT             : { float   pvalue = (float)jsonvalue->GetValueFloating();
                                                                                            XSTRING string;

                                                                                            string.Format(_L("%f"), pvalue);

                                                                                            AddValue(name, string);
                                                                                          }
                                                                                          break;

                                              case XFILEJSONVALUETYPE_DOUBLEFLOAT       : { double  pvalue = (float)jsonvalue->GetValueDoubleFloat();
                                                                                            XSTRING string;

                                                                                            string.Format(_L("%f"), pvalue);

                                                                                            AddValue(name, string);
                                                                                          }
                                                                                          break;

                                              case XFILEJSONVALUETYPE_STRING            : { XSTRING pvalue = (XSTRING)jsonvalue->GetValueString();
                                                                                            AddValue(name, pvalue);
                                                                                          }
                                                                                          break;

                                              case XFILEJSONVALUETYPE_OBJECT            : break;
                                              case XFILEJSONVALUETYPE_ARRAY             : break;
                                              case XFILEJSONVALUETYPE_BOOLEAN           : break;
                                              case XFILEJSONVALUETYPE_NULL              : 
                                                                      default           : break;

                                            }
                                        }
                                       else
                                        {
                                          if(priority) priorityvaluesobtain = false;
                                        }
                                    }
                                }

                              GEN_DELETE json;
                            }
                        }

                      if(!format.Compare(_L("HTML"), true))
                        {
                          for(int d=0; d<nodeweb->GetNElements(); d++)
                            {
                              XFILEXMLELEMENT* nodevalue = nodeweb->GetElement(d);
                              if(nodevalue)
                                {
                                  XSTRING  searchini;
                                  XSTRING  searchend;
                                  XSTRING  namevalue;
                                  XSTRING  prioritystring;
                                  int      priority;

                                  XSTRING  value;

                                  namevalue = nodevalue->GetValueAttribute(_L("name"));
                                  namevalue.ConvertHexFormatChars();

                                  prioritystring = nodevalue->GetValueAttribute(_L("priority"));
                                  priority = prioritystring.ConvertToInt();

                                  searchini = nodevalue->GetValueAttribute(_L("start"));
                                  searchini.ConvertHexFormatChars();

                                  searchend = nodevalue->GetValueAttribute(_L("end"));;
                                  searchend.ConvertHexFormatChars();

                                  if(ExtractString(searchini, searchend, &webpage, false, value))
                                    {
                                      if(!GetValue(namevalue))
                                        {
                                          AddValue(namevalue, value);
                                        }
                                    }
                                   else
                                    {
                                      if(priority) priorityvaluesobtain = false;
                                    }
                                }
                            }
                        }

                      if(values.GetSize() && priorityvaluesobtain)
                        {
                          return true;
                        }
                    }
                }
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::Do(XSTRING& namewebservice, int timeoutforurl, XSTRING* localIP)
* @brief      Execute the operation
* @ingroup    DATAIO
* 
* @param[in]  namewebservice : Namewebservice value.
* @param[in]  timeoutforurl : Timeoutforurl value.
* @param[in]  localIP : Local IP pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::Do(XSTRING& namewebservice, int timeoutforurl, XSTRING* localIP)
{
  return Do(namewebservice.Get(), timeoutforurl, localIP);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::ChangeURL(XCHAR* maskurl, DIOURL& url)
* @brief      Change URL
* @ingroup    DATAIO
* 
* @param[in]  maskurl : Maskurl pointer to use.
* @param[in]  url : URL to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  url = maskurl;

  if(url.GetSize()) return true;

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::AddValue(XCHAR* name, XCHAR* value)
* @brief      Add value
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::AddValue(XCHAR* name, XCHAR* value)
{
  XSTRING* _name  = GEN_NEW XSTRING();
  XSTRING* _value = GEN_NEW XSTRING();

  if((!_name )||(!_value))
    {
      GEN_DELETE _name;
      GEN_DELETE _value;

      return false;
    }

  (*_name)  = name;
  (*_value) = value;

  values.Add(_name, _value);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::AddValue(XCHAR* name, XSTRING& value)
* @brief      Add value
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::AddValue(XCHAR* name, XSTRING& value)
{
  return AddValue(name, value.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::AddValue(XSTRING& name, XCHAR* value)
* @brief      Add value
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::AddValue(XSTRING& name, XCHAR* value)
{
  return AddValue(name.Get(), value);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::AddValue(XSTRING& name, XSTRING& value)
* @brief      Add value
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::AddValue(XSTRING& name, XSTRING& value)
{
  return AddValue(name.Get(), value.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEB::GetValue(XCHAR* namevalue)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  namevalue : Namevalue pointer to use.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEB::GetValue(XCHAR* namevalue)
{
  for(XDWORD c=0;c<values.GetSize();c++)
    {
      XSTRING* name = (XSTRING*)values.GetKey(c);
      if(name)
        {
          if(!name->Compare(namevalue))
            {
              XSTRING* value  = values.GetElement(c);

              if(value)
                {
                  value->ConvertHexFormatChars();
                  return value->Get();
                }

              return NULL;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* DIOSCRAPERWEB::GetValue(XSTRING& namevalue)
* @brief      Get value
* @ingroup    DATAIO
* 
* @param[in]  namevalue : Namevalue value.
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEB::GetValue(XSTRING& namevalue)
{
  return DIOSCRAPERWEB::GetValue(namevalue.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::DeleteAllValues()
* @brief      Delete all values
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::DeleteAllValues()
{
  if(values.IsEmpty())  return false;

  values.DeleteKeyContents();
  values.DeleteElementContents();
  values.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBCLIENT* DIOSCRAPERWEB::GetWebClient()
* @brief      Get web client
* @ingroup    DATAIO
* 
* @return     DIOWEBCLIENT* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBCLIENT* DIOSCRAPERWEB::GetWebClient()
{
  return webclient;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::ExtractString(XCHAR* searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
* @brief      Extract string
* @ingroup    DATAIO
* 
* @param[in]  searchini : Searchini pointer to use.
* @param[in]  searchend : Searchend pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  usebufferpos : Usebufferpos value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::ExtractString(XCHAR* searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
{
  if((!searchini) || (!searchend)) return false;

  XSTRING _searchini;
  XSTRING _searchend;

  _searchini = searchini;
  _searchend = searchend;

  return ExtractString(_searchini, _searchend, xbuffer, usebufferpos, result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::ExtractString(XCHAR* searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
* @brief      Extract string
* @ingroup    DATAIO
* 
* @param[in]  searchini : Searchini pointer to use.
* @param[in]  searchend : Searchend value.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  usebufferpos : Usebufferpos value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::ExtractString(XCHAR* searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
{
  if(!searchini) return false;

  XSTRING _searchini;

  _searchini = searchini;

  return ExtractString(_searchini, searchend, xbuffer, usebufferpos, result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::ExtractString(XSTRING& searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
* @brief      Extract string
* @ingroup    DATAIO
* 
* @param[in]  searchini : Searchini value.
* @param[in]  searchend : Searchend pointer to use.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  usebufferpos : Usebufferpos value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::ExtractString(XSTRING& searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
{
  if(!searchend) return false;

  XSTRING _searchend;

  _searchend = searchend;

  return ExtractString(searchini, _searchend, xbuffer, usebufferpos, result);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOSCRAPERWEB::ExtractString(XSTRING& searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
* @brief      Extract string
* @ingroup    DATAIO
* 
* @param[in]  searchini : Searchini value.
* @param[in]  searchend : Searchend value.
* @param[in]  xbuffer : Buffer to use.
* @param[in]  usebufferpos : Usebufferpos value.
* @param[in]  result : Output result.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEB::ExtractString(XSTRING& searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result)
{
  result.Empty();

  if(!xbuffer)            return false;

  if(searchini.IsEmpty()) return false;
  if(searchend.IsEmpty()) return false;

  XSTRING html;
  int        indexini;
  int        indexend;

  //html = (char*)xbuffer->Get();
  html = *xbuffer; // #Imanol, changed to avoid a buffer overrun

  indexini = html.Find(searchini, false, usebufferpos?xbuffer->GetPosition():0);
  if(indexini==XSTRING_NOTFOUND) return false;

  indexend = html.Find(searchend, false, indexini + searchini.GetSize());
  if(indexend==XSTRING_NOTFOUND) return false;

  if((int)(indexini + searchini.GetSize())>=indexend) return false;

  html.Copy(indexini + searchini.GetSize(), indexend, result);

  result.DeleteCharacter(_C(' '), XSTRINGCONTEXT_FROM_FIRST);
  result.DeleteCharacter(_C(' '), XSTRINGCONTEXT_TO_END);

  if(result.IsEmpty()) return false;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEB::HandleEvent(XEVENT* xevent)
* @brief      Handle event
* @note       INTERNAL
* @ingroup    DATAIO
* 
* @param[in]  xevent : Xevent pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEB::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOSCRAPERWEB::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEB::Clean()
{
  xmutexdo                = NULL;
  webclient               = NULL;
  cache                   = NULL;

  URLdownload             = NULL;

  xml                     = NULL;
}



