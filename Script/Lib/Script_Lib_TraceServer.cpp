/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       Script_Lib_TraceServer.cpp
* 
* @class      SCRIPT_LIB_TRACESERVER
* @brief      Script Library XTrace Server (open / get / clear / close)
* @ingroup    SCRIPT
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

#include "Script_Lib_TraceServer.h"

#include "XVariant.h"
#include "XTraceServer.h"

#include "Script.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/




/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_TRACESERVER::SCRIPT_LIB_TRACESERVER()
* @brief      Constructor of class
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_TRACESERVER::SCRIPT_LIB_TRACESERVER() : SCRIPT_LIB(SCRIPT_LIB_NAME_TRACESERVER)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         SCRIPT_LIB_TRACESERVER::~SCRIPT_LIB_TRACESERVER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
SCRIPT_LIB_TRACESERVER::~SCRIPT_LIB_TRACESERVER()
{
  if(traceserver)
    {
      traceserver->End();
      GEN_DELETE traceserver;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_TRACESERVER::AddLibraryFunctions(SCRIPT* script)
* @brief      Add library functions
* @ingroup    SCRIPT
* 
* @param[in]  script : Script pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_TRACESERVER::AddLibraryFunctions(SCRIPT* script)
{
  if(!script)
    {
      return false;
    }

  this->script = script;

  script->AddLibraryFunction(this, _L("TraceServer_Ini")            , Call_TraceServer_Ini);
  script->AddLibraryFunction(this, _L("TraceServer_IniUART")        , Call_TraceServer_IniUART);
  script->AddLibraryFunction(this, _L("TraceServer_End")            , Call_TraceServer_End);
  script->AddLibraryFunction(this, _L("TraceServer_IsOpen")         , Call_TraceServer_IsOpen);
  script->AddLibraryFunction(this, _L("TraceServer_Clear")          , Call_TraceServer_Clear);
  script->AddLibraryFunction(this, _L("TraceServer_GetCount")       , Call_TraceServer_GetCount);
  script->AddLibraryFunction(this, _L("TraceServer_GetDroppedCount"), Call_TraceServer_GetDroppedCount);
  script->AddLibraryFunction(this, _L("TraceServer_SetMaxMessages") , Call_TraceServer_SetMaxMessages);
  script->AddLibraryFunction(this, _L("TraceServer_Pop")            , Call_TraceServer_Pop);
  script->AddLibraryFunction(this, _L("TraceServer_WaitPop")        , Call_TraceServer_WaitPop);
  script->AddLibraryFunction(this, _L("TraceServer_Peek")           , Call_TraceServer_Peek);
  script->AddLibraryFunction(this, _L("TraceServer_Get")            , Call_TraceServer_Get);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACESERVER* SCRIPT_LIB_TRACESERVER::GetTraceServer()
* @brief      Get (or create) the underlying XTrace server
* @ingroup    SCRIPT
* 
* @return     XTRACESERVER* : Server pointer, or NULL on allocation failure.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACESERVER* SCRIPT_LIB_TRACESERVER::GetTraceServer()
{
  if(!traceserver)
    {
      traceserver = GEN_NEW XTRACESERVER();
    }

  return traceserver;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_TRACESERVER::FormatMessage(XTRACESERVER_MSG& msg, XSTRING& out)
* @brief      Format one queued message as a single script-friendly line
* @ingroup    SCRIPT
* 
* @param[in]  msg : Message to format.
* @param[out] out : Formatted line: level|seq|datetime|publicIP|localIP|text
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_TRACESERVER::FormatMessage(XTRACESERVER_MSG& msg, XSTRING& out)
{
  XSTRING timestr;
  XSTRING text;

  msg.time.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, timestr);

  text = msg.text;
  text.DeleteCharacter(0x0D);
  text.DeleteCharacter(0x0A);

  out.Format(_L("%d|%u|%s|%d.%d.%d.%d|%d.%d.%d.%d|%s")
            , (int)msg.level
            , msg.sequence
            , timestr.Get()
            , (int)((msg.publicIP >> 24) & 0xFF)
            , (int)((msg.publicIP >> 16) & 0xFF)
            , (int)((msg.publicIP >>  8) & 0xFF)
            , (int)( msg.publicIP        & 0xFF)
            , (int)((msg.localIP  >> 24) & 0xFF)
            , (int)((msg.localIP  >> 16) & 0xFF)
            , (int)((msg.localIP  >>  8) & 0xFF)
            , (int)( msg.localIP         & 0xFF)
            , text.Get());

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool SCRIPT_LIB_TRACESERVER::MessageMatches(XTRACESERVER_MSG& msg, int levelfilter, XSTRING& textfilter)
* @brief      Check whether a message matches optional level / text filters
* @ingroup    SCRIPT
* 
* @param[in]  msg : Message to test.
* @param[in]  levelfilter : Exact level, or -1 for any level.
* @param[in]  textfilter : Case-insensitive substring, or empty for any text.
* 
* @return     bool : true if the message matches; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool SCRIPT_LIB_TRACESERVER::MessageMatches(XTRACESERVER_MSG& msg, int levelfilter, XSTRING& textfilter)
{
  if(levelfilter >= 0)
    {
      if((int)msg.level != levelfilter)
        {
          return false;
        }
    }

  if(!textfilter.IsEmpty())
    {
      if(msg.text.Find(textfilter.Get(), true) == XSTRING_NOTFOUND)
        {
          return false;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void SCRIPT_LIB_TRACESERVER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    SCRIPT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void SCRIPT_LIB_TRACESERVER::Clean()
{
  traceserver = NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_Ini(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_Ini([port | config]) — open UDP listener (default port 10001)
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_Ini(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  bool                    status = false;

  if(!srv)
    {
      (*returnvalue) = false;
      return;
    }

  #if defined(DIO_STREAMUDP_ACTIVE)

  if(!params->GetSize())
    {
      status = srv->Ini();
    }
   else
    {
      XVARIANT* param0 = params->Get(0);

      if(param0 && ((param0->GetType() == XVARIANT_TYPE_STRING) || (param0->GetType() == XVARIANT_TYPE_XCHAR) || (param0->GetType() == XVARIANT_TYPE_CHAR)))
        {
          XSTRING config;

          library->GetParamConverted(param0, config);
          status = srv->Ini(config);
        }
       else
        {
          int port = XTRACESERVER_DEFAULT_PORT;

          library->GetParamConverted(param0, port);
          status = srv->Ini((XWORD)port);
        }
    }

  #endif

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_IniUART(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_IniUART(config) — open UART listener
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_IniUART(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  bool                    status = false;

  if(!srv)
    {
      (*returnvalue) = false;
      return;
    }

  #if defined(DIO_STREAMUART_ACTIVE)

  XSTRING config;

  library->GetParamConverted(params->Get(0), config);
  status = srv->IniUART(config);

  #endif

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_End(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_End() — close the server
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_End(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  bool                    status = false;

  if(srv)
    {
      status = srv->End();
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_IsOpen(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_IsOpen() — true if UDP and/or UART listener is open
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_IsOpen(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  bool                    status = false;

  if(srv)
    {
      status = srv->IsOpen();
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_Clear(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_Clear() — erase all queued traces
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_Clear(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  bool                    status = false;

  if(srv)
    {
      status = srv->Clear();
    }

  (*returnvalue) = status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_GetCount(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_GetCount() — number of queued messages
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_GetCount(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  int                     count = 0;

  if(srv)
    {
      count = (int)srv->GetCount();
    }

  (*returnvalue) = count;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_GetDroppedCount(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_GetDroppedCount() — messages dropped when queue was full
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_GetDroppedCount(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  int                     count = 0;

  if(srv)
    {
      count = (int)srv->GetDroppedCount();
    }

  (*returnvalue) = count;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_SetMaxMessages(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_SetMaxMessages(n) — bound the receive queue
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_SetMaxMessages(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  int                     maxmessages = 0;

  library->GetParamConverted(params->Get(0), maxmessages);

  if(srv && (maxmessages > 0))
    {
      srv->SetMaxMessages((XDWORD)maxmessages);
      (*returnvalue) = true;
    }
   else
    {
      (*returnvalue) = false;
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_Pop(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_Pop() — consume next message (empty string if none)
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_Pop(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  XSTRING                 line;

  if(srv)
    {
      XTRACESERVER_MSG msg;

      if(srv->Pop(msg))
        {
          lib->FormatMessage(msg, line);
        }
    }

  (*returnvalue) = line;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_WaitPop(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_WaitPop(timeout_ms) — wait for next message (-1 = infinite)
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_WaitPop(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  XSTRING                 line;
  int                     timeout_ms = -1;

  library->GetParamConverted(params->Get(0), timeout_ms);

  if(srv)
    {
      XTRACESERVER_MSG msg;

      if(srv->WaitPop(msg, timeout_ms))
        {
          lib->FormatMessage(msg, line);
        }
    }

  (*returnvalue) = line;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_Peek(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_Peek(index) — copy message at index without removing it
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_Peek(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPT_ERRORCODE_INSUF_PARAMS);
      return;
    }

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  XSTRING                 line;
  int                     index = 0;

  library->GetParamConverted(params->Get(0), index);

  if(srv && (index >= 0))
    {
      XTRACESERVER_MSG msg;

      if(srv->Peek((XDWORD)index, msg))
        {
          lib->FormatMessage(msg, line);
        }
    }

  (*returnvalue) = line;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void Call_TraceServer_Get(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
* @brief      TraceServer_Get([level=-1], [textfilter=""], [maxcount=0]) — peek matching messages (non-destructive)
* @ingroup    SCRIPT
* 
* @param[in]  library : Library pointer to use.
* @param[in]  script : Script pointer to use.
* @param[in]  params : Params pointer to use.
* @param[in]  returnvalue : Returnvalue pointer to use.
* 
* @note       level=-1 any level; empty textfilter any text; maxcount=0 all matches.
*             Result is newline-separated lines: level|seq|datetime|publicIP|localIP|text
* 
* --------------------------------------------------------------------------------------------------------------------*/
void Call_TraceServer_Get(SCRIPT_LIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if((!library) || (!script) || (!params) || (!returnvalue))
    {
      return;
    }

  returnvalue->Set();

  SCRIPT_LIB_TRACESERVER* lib = (SCRIPT_LIB_TRACESERVER*)library;
  XTRACESERVER*           srv = lib->GetTraceServer();
  XSTRING                 result;
  int                     levelfilter = -1;
  XSTRING                 textfilter;
  int                     maxcount = 0;

  if(params->GetSize() >= 1)
    {
      library->GetParamConverted(params->Get(0), levelfilter);
    }

  if(params->GetSize() >= 2)
    {
      library->GetParamConverted(params->Get(1), textfilter);
    }

  if(params->GetSize() >= 3)
    {
      library->GetParamConverted(params->Get(2), maxcount);
    }

  if(srv)
    {
      XDWORD count = srv->GetCount();
      int    matched = 0;

      for(XDWORD c=0; c<count; c++)
        {
          XTRACESERVER_MSG msg;

          if(!srv->Peek(c, msg))
            {
              continue;
            }

          if(!lib->MessageMatches(msg, levelfilter, textfilter))
            {
              continue;
            }

          XSTRING line;

          lib->FormatMessage(msg, line);

          if(!result.IsEmpty())
            {
              result.Add(_L("\n"));
            }

          result.Add(line);
          matched++;

          if((maxcount > 0) && (matched >= maxcount))
            {
              break;
            }
        }
    }

  (*returnvalue) = result;
}
