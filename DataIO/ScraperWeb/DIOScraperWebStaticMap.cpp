/**-------------------------------------------------------------------------------------------------------------------
*
* @file       DIOScraperWebStaticMap.cpp
*
* @class      DIOSCRAPERWEBSTATICMAP
* @brief      Data Input/Output Scraper Web Static Map class
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

#ifdef DIO_SCRAPERWEB_STATICMAP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOScraperWebStaticMap.h"

#include "XFactory.h"
#include "XThread.h"
#include "XFile.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTATICMAP_RESULT::DIOSTATICMAP_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTATICMAP_RESULT::DIOSTATICMAP_RESULT(): DIOSCRAPERWEBCACHE_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSTATICMAP_RESULT::~DIOSTATICMAP_RESULT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSTATICMAP_RESULT::~DIOSTATICMAP_RESULT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* DIOSTATICMAP_RESULT::GetPath()
* @brief      Get path
* @ingroup    DATAIO
*
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSTATICMAP_RESULT::GetPath()
{
  return path.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* DIOSTATICMAP_RESULT::GetProvider()
* @brief      Get provider
* @ingroup    DATAIO
*
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
*
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSTATICMAP_RESULT::GetProvider()
{
  return provider.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTATICMAP_RESULT::IsEmpty()
* @brief      Is empty
* @ingroup    DATAIO
*
* @return     bool : true if the condition is met; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTATICMAP_RESULT::IsEmpty()
{
  return path.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTATICMAP_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy from
* @ingroup    DATAIO
*
* @param[in]  result : Output result.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTATICMAP_RESULT::CopyFrom(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result) return false;

  DIOSTATICMAP_RESULT* staticmap = (DIOSTATICMAP_RESULT*)result;

  if(staticmap->IsEmpty()) return false;

  path     = staticmap->path;
  provider = staticmap->provider;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTATICMAP_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
* @brief      Copy to
* @ingroup    DATAIO
*
* @param[in]  result : Output result.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTATICMAP_RESULT::CopyTo(DIOSCRAPERWEBCACHE_RESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOSTATICMAP_RESULT* staticmap = (DIOSTATICMAP_RESULT*)result;

  staticmap->path     = path;
  staticmap->provider = provider;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTATICMAP_RESULT::Set(XSTRING& path, XSTRING& provider)
* @brief      Set value
* @ingroup    DATAIO
*
* @param[in]  path : Local image path.
* @param[in]  provider : Provider name.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTATICMAP_RESULT::Set(XSTRING& path, XSTRING& provider)
{
  return Set(path.Get(), provider.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSTATICMAP_RESULT::Set(XCHAR* path, XCHAR* provider)
* @brief      Set value
* @ingroup    DATAIO
*
* @param[in]  path : Local image path.
* @param[in]  provider : Provider name.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSTATICMAP_RESULT::Set(XCHAR* path, XCHAR* provider)
{
  if(path)     this->path     = path;
  if(provider) this->provider = provider;

  return !this->path.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSTATICMAP_RESULT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSTATICMAP_RESULT::Clean()
{

}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSCRAPERWEBSTATICMAP::DIOSCRAPERWEBSTATICMAP()
* @brief      Constructor of class
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBSTATICMAP::DIOSCRAPERWEBSTATICMAP()
{
  Clean();

  cache      = GEN_NEW DIOSCRAPERWEBCACHE();
  xmutexdo   = GEN_XFACTORY.Create_Mutex();
  scriptpath = DIOSCRAPERWEBSTATICMAP_SCRIPTPATH;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         DIOSCRAPERWEBSTATICMAP::~DIOSCRAPERWEBSTATICMAP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
DIOSCRAPERWEBSTATICMAP::~DIOSCRAPERWEBSTATICMAP()
{
  if(cache)
    {
      cache->DeleteAll();
      GEN_DELETE cache;
    }

  if(xmutexdo)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexdo);
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSCRAPERWEBSTATICMAP::Get(float latitude, float longitude, XSTRING& outpath, DIOSTATICMAP_RESULT& result, int width, int height, int zoom, XCHAR* address, int timeoutforurl, bool usecache)
* @brief      Get value
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBSTATICMAP::Get(float latitude, float longitude, XSTRING& outpath, DIOSTATICMAP_RESULT& result, int width, int height, int zoom, XCHAR* address, int timeoutforurl, bool usecache)
{
  XPATH path;

  path = outpath.Get();
  return Get(latitude, longitude, path, result, width, height, zoom, address, timeoutforurl, usecache);
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSCRAPERWEBSTATICMAP::Get(float latitude, float longitude, XPATH& outpath, DIOSTATICMAP_RESULT& result, int width, int height, int zoom, XCHAR* address, int timeoutforurl, bool usecache)
* @brief      Get static map image via scraper script (downloads PNG to outpath)
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBSTATICMAP::Get(float latitude, float longitude, XPATH& outpath, DIOSTATICMAP_RESULT& result, int width, int height, int zoom, XCHAR* address, int timeoutforurl, bool usecache)
{
  bool    status = false;
  XSTRING ask;

  if(outpath.IsEmpty()) return false;

  if(width  <= 0) width  = DIOSCRAPERWEBSTATICMAP_DEFAULT_WIDTH;
  if(height <= 0) height = DIOSCRAPERWEBSTATICMAP_DEFAULT_HEIGHT;
  if(zoom   <= 0) zoom   = DIOSCRAPERWEBSTATICMAP_DEFAULT_ZOOM;

  if(xmutexdo) xmutexdo->Lock();

  BuildCacheAsk(latitude, longitude, width, height, zoom, address, ask);

  if(usecache && cache)
    {
      DIOSTATICMAP_RESULT* cached = (DIOSTATICMAP_RESULT*)cache->Get(ask);
      if(cached && (!cached->IsEmpty()))
        {
          result.CopyFrom(cached);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  DIOSCRAPERSCRIPT runner;
  XSTRING          latstr;
  XSTRING          lonstr;
  XSTRING          outpathstr;

  latstr.Format(NL("%.6f"), latitude);
  lonstr.Format(NL("%.6f"), longitude);
  outpathstr = outpath.Get();

  runner.SetArg(NL("latitude") , latstr);
  runner.SetArg(NL("longitude"), lonstr);
  runner.SetArg(NL("outpath")  , outpathstr);
  runner.SetArgInt(NL("width")  , width);
  runner.SetArgInt(NL("height") , height);
  runner.SetArgInt(NL("zoom")   , zoom);
  runner.SetArgInt(NL("timeout"), timeoutforurl);
  if(address && address[0])
    {
      runner.SetArg(NL("address"), address);
    }

  if(runner.Run(scriptpath.Get()))
    {
      XSTRING ok;
      XSTRING path;
      XSTRING provider;

      runner.GetResult(NL("ok")      , ok);
      runner.GetResult(NL("path")    , path);
      runner.GetResult(NL("provider"), provider);

      if(ok.Compare(NL("1")) == 0)
        {
          if(path.IsEmpty()) path = outpathstr;

          // Confirm the PNG landed on disk before caching.
          bool fileok = false;
          XFILE* file = GEN_XFACTORY.Create_File();
          if(file)
            {
              if(file->Exist(path.Get()) && file->Open(path.Get()))
                {
                  fileok = (file->GetSize() > 0);
                  file->Close();
                }
              GEN_XFACTORY.Delete_File(file);
            }

          if(fileok)
            {
              result.Set(path, provider);

              if(usecache && cache && (!result.IsEmpty()))
                {
                  DIOSTATICMAP_RESULT* cached = GEN_NEW DIOSTATICMAP_RESULT();
                  if(cached)
                    {
                      cached->CopyFrom(&result);
                      cache->Add(ask, cached);
                    }
                }

              status = true;
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSCRAPERWEBSTATICMAP::SetScriptPath(XCHAR* relativescriptpath)
* @brief      SetScriptPath
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBSTATICMAP::SetScriptPath(XCHAR* relativescriptpath)
{
  if((!relativescriptpath) || (!relativescriptpath[0])) return false;

  scriptpath = relativescriptpath;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCHAR* DIOSCRAPERWEBSTATICMAP::GetScriptPath()
* @brief      GetScriptPath
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* DIOSCRAPERWEBSTATICMAP::GetScriptPath()
{
  return scriptpath.Get();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool DIOSCRAPERWEBSTATICMAP::BuildCacheAsk(float latitude, float longitude, int width, int height, int zoom, XCHAR* address, XSTRING& ask)
* @brief      BuildCacheAsk
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOSCRAPERWEBSTATICMAP::BuildCacheAsk(float latitude, float longitude, int width, int height, int zoom, XCHAR* address, XSTRING& ask)
{
  if(address && address[0])
    {
      ask.Format(NL("addr|%s|%d|%d|%d"), address, width, height, zoom);
    }
   else
    {
      ask.Format(NL("%.4f|%.4f|%d|%d|%d"), latitude, longitude, width, height, zoom);
    }

  return !ask.IsEmpty();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void DIOSCRAPERWEBSTATICMAP::Clean()
* @brief      Clean
* @note       INTERNAL
* @ingroup    DATAIO
*
* --------------------------------------------------------------------------------------------------------------------*/
void DIOSCRAPERWEBSTATICMAP::Clean()
{
  cache    = NULL;
  xmutexdo = NULL;
}

#endif
