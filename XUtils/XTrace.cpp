/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XTrace.cpp
* 
* @class      XTRACE
* @brief      eXtended Utils Trace class 
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

/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Defines.h"



/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/


#include "XTrace.h"

#include "XFactory.h"
#include "XPath.h"

#if defined(XFILE_ACTIVE) && defined(XFILE_JSON_ACTIVE)
#include "XFileJSON.h"
#endif

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIODNSResolver.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamEnumDevices.h"
#include "DIOStreamIPLocalEnumDevices.h"
#include "DIOPublicInternetIP.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XTRACE* XTRACE::instance = NULL;



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_TARGET::XTRACE_TARGET()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_TARGET::XTRACE_TARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_TARGET::~XTRACE_TARGET()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_TARGET::~XTRACE_TARGET()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_TYPE XTRACE_TARGET::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XTRACE_TYPE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_TYPE XTRACE_TARGET::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::SetType(XTRACE_TYPE type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : type of debug trace (enum XTRACE_TYPE)
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::SetType(XTRACE_TYPE type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XTRACE_TARGET::GetAim()
* @brief      Get aim
* @ingroup    XUTILS
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XTRACE_TARGET::GetAim()
{
  return aim;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE_TARGET::GetAim(XSTRING& aim)
* @brief      Get aim
* @ingroup    XUTILS
* 
* @param[in]  aim : Get aim of target
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE_TARGET::GetAim(XSTRING& aim)
{
  aim.Empty();
  aim = this->aim;

  return aim.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::SetAim(XSTRING& aim)
* @brief      Set aim
* @ingroup    XUTILS
* 
* @param[in]  aim : GEN_NEW aim of target
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::SetAim(XSTRING& aim)
{
  memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));
  memcpy(this->aim, aim.Get(), (aim.GetSize() * sizeof(XCHAR)));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::SetAim(XCHAR* aim)
* @brief      Set aim
* @ingroup    XUTILS
* 
* @param[in]  aim : GEN_NEW aim of target
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::SetAim(XCHAR* aim)
{
  XDWORD size = XSTRING::GetSize(aim);
  memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));
  memcpy(this->aim, aim, size * sizeof(XCHAR));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE_TARGET::GetNSendings()
* @brief      Get N sendings
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE_TARGET::GetNSendings()
{
  return nsendings;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::AddNSendings()
* @brief      Add N sendings
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::AddNSendings()
{
  nsendings++;
}


#if (defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWORD XTRACE_TARGET::GetPort()
* @brief      Get port
* @ingroup    XUTILS
* 
* @return     XWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWORD XTRACE_TARGET::GetPort()
{
  return port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::SetPort(XWORD port)
* @brief      Set port
* @ingroup    XUTILS
* 
* @param[in]  port : port of target
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::SetPort(XWORD port)
{
  this->port = port;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE_TARGET::IPTarget()
* @brief      IP target
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE_TARGET::IPTarget()
{
  if(type != XTRACE_TYPE_NET) return false;

  XSTRING  _aim;
  DIOIP    _IP;
  XSTRING  _IPstring;

  _aim = aim;

  if(!_aim.Compare(_L("*"), true))
    {
      _IPstring = _L("*");
    }
   else
    {          
      #ifndef XTRACE_NOINTERNET

      GEN_DIODNSRESOLVER.ResolveURL(_aim.Get(), _IP);
      _IP.GetXString(_IPstring);

      #else
 
      _IPstring = _aim;

      #endif
    }
    
    
  XBUFFER charstr;
 
  _IPstring.ConvertToASCII(charstr);    
  memcpy(IP, charstr.GetPtrChar(), strlen(charstr.GetPtrChar())+1);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         char* XTRACE_TARGET::GetIP()
* @brief      Get IP
* @ingroup    XUTILS
* 
* @return     char* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
char* XTRACE_TARGET::GetIP()
{
  return IP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XQWORD XTRACE_TARGET::GetNETHandle()
* @brief      Get NET handle
* @ingroup    XUTILS
* 
* @return     XQWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XQWORD XTRACE_TARGET::GetNETHandle()
{
  return NEThandle;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET:: SetNETHandle(XQWORD NEThandle)
* @brief      Set NET handle
* @ingroup    XUTILS
* 
* @param[in]  NEThandle : GEN_NEW Net Handle of target
* 
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET:: SetNETHandle(XQWORD NEThandle)
{
  this->NEThandle = NEThandle;
}


#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_TARGET::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_TARGET::Clean()
{
  type          = XTRACE_TYPE_NONE;

  memset(aim, 0, (_MAXSTR * sizeof(XCHAR)));
  nsendings     = 0;

  #if (defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  port          = 0;
  memset(IP, 0, _MAXSTR * sizeof(char));
  NEThandle     = 0;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSG::XTRACE_STATUS_MSG()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSG::XTRACE_STATUS_MSG()
{
  Clean();

  xtimerlastupdate=GEN_XFACTORY.CreateTimer();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSG::~XTRACE_STATUS_MSG()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSG::~XTRACE_STATUS_MSG()
{
  if(xtimerlastupdate)
    {
      GEN_XFACTORY.DeleteTimer(xtimerlastupdate);
      xtimerlastupdate = NULL;
    }

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_TYPE_STATUS_MSG XTRACE_STATUS_MSG::GetType()
* @brief      Get type
* @ingroup    XUTILS
* 
* @return     XTRACE_TYPE_STATUS_MSG : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_TYPE_STATUS_MSG XTRACE_STATUS_MSG::GetType()
{
  return type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::SetType(XTRACE_TYPE_STATUS_MSG type)
* @brief      Set type
* @ingroup    XUTILS
* 
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::SetType(XTRACE_TYPE_STATUS_MSG type)
{
  this->type = type;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XTRACE_STATUS_MSG::GetName()
* @brief      Get name
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACE_STATUS_MSG::GetName()
{
  return &name;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTIMER* XTRACE_STATUS_MSG::GetXTimerLastUpdate()
* @brief      Get X timer last update
* @ingroup    XUTILS
* 
* @return     XTIMER* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTIMER* XTRACE_STATUS_MSG::GetXTimerLastUpdate()
{
  return xtimerlastupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE_STATUS_MSG::Value_GetBoolean()
* @brief      Value get boolean
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE_STATUS_MSG::Value_GetBoolean()
{
  return value_boolean;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Value_SetBoolean(bool value)
* @brief      Value set boolean
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Value_SetBoolean(bool value)
{
  value_boolean = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int XTRACE_STATUS_MSG::Value_GetInteger()
* @brief      Value get integer
* @ingroup    XUTILS
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int XTRACE_STATUS_MSG::Value_GetInteger()
{
  return value_integer;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Value_SetInteger(int value)
* @brief      Value set integer
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Value_SetInteger(int value)
{
  this->value_integer = value;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* XTRACE_STATUS_MSG::Value_GetString()
* @brief      Value get string
* @ingroup    XUTILS
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* XTRACE_STATUS_MSG::Value_GetString()
{
  return &value_string;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE_STATUS_MSG::Value_GetDword()
* @brief      Value get dword
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE_STATUS_MSG::Value_GetDword()
{
  return value_dword;
}   


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Value_SetDword(XDWORD value)
* @brief      Value set dword
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Value_SetDword(XDWORD value)
{
  value_dword = value;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         float XTRACE_STATUS_MSG::Value_GetFloat()
* @brief      Value get float
* @ingroup    XUTILS
* 
* @return     float : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
float XTRACE_STATUS_MSG::Value_GetFloat()
{
  return value_float;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Value_SetFloat(float value)
* @brief      Value set float
* @ingroup    XUTILS
* 
* @param[in]  value : Value value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Value_SetFloat(float value)
{
  value_float = value;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE* XTRACE_STATUS_MSG::Value_GetColor()
* @brief      Value get color
* @ingroup    XUTILS
* 
* @return     XBYTE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE* XTRACE_STATUS_MSG::Value_GetColor()
{
  return value_color;
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Value_SetColor(XBYTE* value)
* @brief      Value set color
* @ingroup    XUTILS
* 
* @param[in]  value : Value pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Value_SetColor(XBYTE* value)
{
  for(int c=0; c<3; c++)
    {
      value_color[c] = value[c];
    }
}
    

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSG::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSG::Clean()
{
  type              = XTRACE_TYPE_STATUS_MSG_UNKNOWN;  

  xtimerlastupdate  = NULL;

  value_boolean     = false;
  value_integer     = 0;
  value_string.Empty();
  value_dword       = 0;
  value_float       = 0.0f;

  for(int c=0; c<3; c++)
    {
      value_color[c] = 0;        
    }
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSGS::XTRACE_STATUS_MSGS()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSGS::XTRACE_STATUS_MSGS()
{
  Clean();
}
      

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSGS::~XTRACE_STATUS_MSGS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSGS::~XTRACE_STATUS_MSGS()
{
  StatusMsg_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Add(XSTRING* line)
* @brief      Status msg add
* @ingroup    XUTILS
* 
* @param[in]  line : Line pointer to use.
* 
* @return     XTRACE_STATUS_MSG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Add(XSTRING* line)
{
  if(!line) return NULL;

  XSTRING   ID;
  XSTRING   name;
  XCHAR     typechar;
  XSTRING   valuestr;
  bool      isnew = false;


  int indexstatus = line->Find(XTRACE_IDMSGSTATUSCLEAR, false);
  if(indexstatus != XSTRING_NOTFOUND) 
    {
      statusmsgs.DeleteContents();
      statusmsgs.DeleteAll();

      return NULL;
    }


  indexstatus = line->Find(XTRACE_IDMSGSTATUS, false);
  if(indexstatus == XSTRING_NOTFOUND) 
    {
      return NULL;
    }


  XSTRING string;

  line->Copy(indexstatus, string);

  ID.AdjustSize(_MAXSTR);
  name.AdjustSize(_MAXSTR);
  valuestr.AdjustSize(_MAXSTR);

  string.UnFormat(_L("%s,%s,%c,%s"), ID.Get(), name.Get(), &typechar, valuestr.Get());

  ID.AdjustSize();
  name.AdjustSize();
  valuestr.AdjustSize();


  XTRACE_STATUS_MSG* status_msg = StatusMsg_Get(name.Get());
  if(!status_msg)
    {
      status_msg = GEN_NEW XTRACE_STATUS_MSG();
      if(!status_msg) return NULL;

      isnew = true;  
    }

  switch(typechar)
    {      
      case XTRACE_IDMSGSTATUS_BOOLEAN : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_BOOLEAN);
                                          if(!valuestr.Compare(_L("true"), true))  status_msg->Value_SetBoolean(true); else status_msg->Value_SetBoolean(false);
                                        }
                                        break;

      case XTRACE_IDMSGSTATUS_INTEGER : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_INTEGER);
                                          status_msg->Value_SetInteger(valuestr.ConvertToInt());
                                        } 
                                        break;
    
      case XTRACE_IDMSGSTATUS_STRING  : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_STRING);
                                          status_msg->Value_GetString()->Set(valuestr);
                                        } 
                                        break;

      case XTRACE_IDMSGSTATUS_DWORD   : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_XDWORD);
                                          status_msg->Value_SetDword(valuestr.ConvertToDWord());
                                        }
                                        break;
    
      case XTRACE_IDMSGSTATUS_FLOAT   : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_FLOAT);
                                          status_msg->Value_SetFloat(valuestr.ConvertToFloat());
                                        }
                                        break;

      case XTRACE_IDMSGSTATUS_COLOR   : { status_msg->SetType(XTRACE_TYPE_STATUS_MSG_RGBCOLOR);

                                          int    icolor[3];
                                          XBYTE  color[3]; 
                                          valuestr.UnFormat(_L("%02X,%02X,%02X"), &icolor[0], &icolor[1], &icolor[2]);

                                          for(int c=0; c<3; c++)
                                            {  
                                              color[c] = icolor[c];
                                            }

                                          status_msg->Value_SetColor(color);
                                        }
                                        break;    
    }

  if(isnew)
    {
      status_msg->GetName()->Set(name);      
      statusmsgs.Add(status_msg);      
    }  

  return status_msg;
}

    
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Get(XCHAR* name)
* @brief      Status msg get
* @ingroup    XUTILS
* 
* @param[in]  name : Name to use.
* 
* @return     XTRACE_STATUS_MSG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Get(XCHAR* name)
{
  if(statusmsgs.IsEmpty()) return NULL;

  for(XDWORD c=0; c<statusmsgs.GetSize(); c++)
    {
      XTRACE_STATUS_MSG* status_msg = statusmsgs.Get(c);
      if(status_msg)
        {
          if(!status_msg->GetName()->Compare(name, true)) return status_msg;
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Get(int index)
* @brief      Status msg get
* @ingroup    XUTILS
* 
* @param[in]  index : Index value.
* 
* @return     XTRACE_STATUS_MSG* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE_STATUS_MSG* XTRACE_STATUS_MSGS::StatusMsg_Get(int index)
{
  if(statusmsgs.IsEmpty()) return NULL;

  XTRACE_STATUS_MSG* status_msg = statusmsgs.Get(index);
  return status_msg;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XTRACE_STATUS_MSG*>* XTRACE_STATUS_MSGS::StatusMsg_GetAll()
* @brief      Status msg get all
* @ingroup    XUTILS
* 
* @return     XVECTOR<XTRACE_STATUS_MSG*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XTRACE_STATUS_MSG*>* XTRACE_STATUS_MSGS::StatusMsg_GetAll()
{
  return &statusmsgs;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE_STATUS_MSGS::StatusMsg_DeleteAll()
* @brief      Status msg GEN_DELETE all
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE_STATUS_MSGS::StatusMsg_DeleteAll()
{
  if(statusmsgs.IsEmpty()) return false;

  statusmsgs.DeleteContents();
  statusmsgs.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE_STATUS_MSGS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE_STATUS_MSGS::Clean()
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE::XTRACE()
* @brief      Constructor of class
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE::XTRACE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XTRACE::~XTRACE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XTRACE::~XTRACE()
{
  Tests_DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetTarget(int index, XTRACE_TYPE type, XCHAR* aim)
* @brief      Set target
* @ingroup    XUTILS
* 
* @param[in]  index : index to set target
* @param[in]  type : type of the target
* @param[in]  aim : aim of the target
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetTarget(int index, XTRACE_TYPE type, XCHAR* aim)
{
  if(index >= XTRACE_MAXNTARGETS) return false;

  if(!aim) return false;

  targets[index].SetType(type);

  if(type == XTRACE_TYPE_NET)
    {
      #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

      XSTRING string;
      XSTRING url;
      int     port;

      string = aim;
      url.AdjustSize(_MAXSTR);
      string.UnFormat(_L("%s:%d"), url.Get(), &port);
      url.AdjustSize();

      targets[index].SetAim(url.Get());
      targets[index].SetPort(port);
      targets[index].IPTarget();

      if(targets[index].GetNETHandle()) CloseHandleNet(&targets[index]);
      GetHandleNet(&targets[index]);

      if(!localIP)    ObtainLocalIP();

      #ifndef XTRACE_NOINTERNET
      if(!publicIP)   ObtainPublicIP();
      #endif

      #endif
    }
   else
    {
      targets[index].SetAim(aim);
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::AddTarget(XTRACE_TYPE type, XCHAR* aim)
* @brief      Add target
* @ingroup    XUTILS
* 
* @param[in]  type : type of the target
* @param[in]  aim : aim of the target
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::AddTarget(XTRACE_TYPE type, XCHAR* aim)
{
  for(int c=0; c<XTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XTRACE_TYPE_NONE)  return SetTarget(c, type, aim);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::DeactivateAllTargets()
* @brief      Deactivate all targets
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::DeactivateAllTargets()
{
  for(int c=0; c<XTRACE_MAXNTARGETS; c++)
    {
      targets[c].Clean();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::GetSizeLimit()
* @brief      Get size limit
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::GetSizeLimit()
{
  return sizelimit;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetSizeLimit(XDWORD sizelimit)
* @brief      Set size limit
* @ingroup    XUTILS
* 
* @param[in]  sizelimit : GEN_NEW size limit
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetSizeLimit(XDWORD sizelimit)
{
  if(!sizelimit) return false;

  this->sizelimit = sizelimit;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XTRACE::Application_GetName()
* @brief      Application get name
* @ingroup    XUTILS
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XTRACE::Application_GetName()
{
  return applicationname;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetName(XCHAR* applicationname)
* @brief      Application set name
* @ingroup    XUTILS
* 
* @param[in]  applicationname : GEN_NEW application name
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetName(XCHAR* applicationname)
{
  XDWORD size = 0;
  if(applicationname) size=XSTRING::GetSize(applicationname);

  memset(this->applicationname, 0, _MAXSTR * sizeof(XCHAR));
  if(applicationname) memcpy(this->applicationname, applicationname, (size + 1) * sizeof(XCHAR));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetName(XCHAR* applicationname, XDWORD size)
* @brief      Application set name
* @ingroup    XUTILS
* 
* @param[in]  applicationname : GEN_NEW application name
* @param[in]  size : size of application name
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetName(XCHAR* applicationname, XDWORD size)
{
  memset(this->applicationname, 0, _MAXSTR*sizeof(XCHAR));
  if(applicationname) memcpy(this->applicationname, applicationname, size*sizeof(XCHAR));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetName(XSTRING& applicationname)
* @brief      Application set name
* @ingroup    XUTILS
* 
* @param[in]  applicationname : GEN_NEW application name
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetName(XSTRING& applicationname)
{
  Application_SetName(applicationname.Get(), applicationname.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_GetVersion(int& applicationversion, int& applicationsubversion, int& applicationsubversionerr)
* @brief      Application get version
* @ingroup    XUTILS
* 
* @param[in]  applicationversion : version number
* @param[in]  applicationsubversion : subversion number
* @param[in]  applicationsubversionerr : subversion error number
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_GetVersion(int& applicationversion, int& applicationsubversion, int& applicationsubversionerr)
{
  applicationversion        = this->applicationversion;
  applicationsubversion     = this->applicationsubversion;
  applicationsubversionerr  = this->applicationsubversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetVersion(int applicationversion, int applicationsubversion, int applicationsubversionerr)
* @brief      Application set version
* @ingroup    XUTILS
* 
* @param[in]  applicationversion : GEN_NEW version number
* @param[in]  applicationsubversion : GEN_NEW subversion number
* @param[in]  applicationsubversionerr : GEN_NEW subversion error number
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetVersion(int applicationversion, int applicationsubversion, int applicationsubversionerr)
{
  this->applicationversion        = applicationversion;
  this->applicationsubversion     = applicationsubversion;
  this->applicationsubversionerr  = applicationsubversionerr;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XCHAR* XTRACE::Application_GetID()
* @brief      Application get ID
* @ingroup    XUTILS
* 
* @return     XCHAR* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XCHAR* XTRACE::Application_GetID()
{
  return applicationID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetID(XCHAR* applicationID, XDWORD size)
* @brief      Application set ID
* @ingroup    XUTILS
* 
* @param[in]  applicationID : GEN_NEW string application ID
* @param[in]  size : size application ID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetID(XCHAR* applicationID, XDWORD size)
{
  memset(this->applicationID, 0, _MAXSTR*sizeof(XCHAR)),
  memcpy(this->applicationID, applicationID, (size+1)*sizeof(XCHAR));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Application_SetID(XSTRING& applicationID)
* @brief      Application set ID
* @ingroup    XUTILS
* 
* @param[in]  applicationID : GEN_NEW string application ID
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Application_SetID(XSTRING& applicationID)
{
  Application_SetID(applicationID.Get(), applicationID.GetSize());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ClearScreen(XBYTE level)
* @brief      Clear screen
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ClearScreen(XBYTE level)
{
  XSTRING string;

  string.Format(_L("%s"), XTRACE_IDMSGCLEARSCREEN);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::PrintHeader(XBYTE level, XCHAR* header)
* @brief      Print header
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  header : Header pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::PrintHeader(XBYTE level, XCHAR* header)
{
  XSTRING line;

  line.Empty();

  line  = _L("---");

  if(header)
    {
      line += _L(" ");
      line += header;
      line += _L(" ");
    }

  while(line.GetSize()<120)
    {
      line += _L("-");
    }

  Print(level, line.Get());

  openheader = header?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Print(XBYTE level, XCHAR* mask, ...)
* @brief      Print
* @ingroup    XUTILS
* 
* @param[in]  level : level of line
* @param[in]  mask : mask of format of print
* @param[in]  ... : variables to print
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Print(XBYTE level, XCHAR* mask, ...)
{
  XSTRING string;
  XSTRING outstring;

  string.Add(applicationname);
  if(string.GetSize())
    {
      string.AdjustSize(XTRACE_MAXAPPLICATIONNAMESIZE, false, _L(" "));
      outstring += string;
      outstring += _L(" ");
    }

  string.Format(_L("%d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);
  if(string.GetSize())
    {
      string.AdjustSize(XTRACE_MAXAPPLICATIONVERSIONSIZE, false, _L(" "));      
      outstring += string;
      outstring += _L(" ");
    }

  string = applicationID;
  if(string.GetSize())
    {
      string.AdjustSize(XTRACE_MAXAPPLICATIONIDSIZE, false, _L(" "));
      outstring += string;
      outstring += _L(" ");
    }

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  sequence++;
  
  if(level)
    {
      if((level & XTRACE_LEVEL_WITHCOLOR) == XTRACE_LEVEL_WITHCOLOR)
        {

        }
        else
        {
          if((level & XTRACE_LEVEL_WITHTAB) == XTRACE_LEVEL_WITHTAB)
            {
              XSTRING tabstr;
              GenerateTab(level&0x0F, tabstr);

              string.Insert(tabstr, 0);

            }
            else
            {
              if((level & XTRACE_LEVEL_WITHCODE) == XTRACE_LEVEL_WITHCODE)
                {
                  XSTRING codestr;

                  codestr.Format(_L("%02d: "), (level&0x0F));

                  string.Insert(codestr, 0);
                }
            }
        }
    }
  
  outstring += string;

  for(int c=0; c<XTRACE_MAXNTARGETS ; c++)
    {
      switch(targets[c].GetType())
        {
          case XTRACE_TYPE_SPECIAL   : PrintSpecial(&targets[c], level, outstring.Get());  break;
          case XTRACE_TYPE_FILE      : PrintFile(&targets[c], level, outstring.Get());     break;
          case XTRACE_TYPE_NET       : PrintNet(&targets[c], level, outstring.Get());      break;
                          default     : break;
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintDataBlock(XBYTE level, XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block
* @ingroup    XUTILS
* 
* @param[in]  level : level of line
* @param[in]  data : buffer of data to print
* @param[in]  size : size of buffer
* @param[in]  marginsize : size of margin
* @param[in]  sizeline : size of line
* @param[in]  showoffset : true if had offset
* @param[in]  showtext : true if show assci of buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintDataBlock(XBYTE level, XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  XSTRING    margin;
  XSTRING    strdata;
  XDWORD     _size     = 0;
  XDWORD     _sizeline = sizeline;
  int        index     = 0;

  for(XDWORD c=0; c<marginsize; c++)
    {
      margin += _L(" ");
    }

  while(_size<size)
    {
      XSTRING string;

      string   = margin;
      if(_sizeline > (size - _size))
         _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(_L("%04X"), index);
          string += strdata;
          string += _L(" ");
        }

      for(XDWORD c=0; c<_sizeline; c++)
        {
          strdata.Format(_L("%02X "), data[index]);
          string += strdata;
          _size++;
          index++;
        }

      //if(_sizeline != sizeline)
        {
          for(XDWORD c=_sizeline; c<sizeline; c++)
            {
              string += _L(" ");
            }
        }

      if(showtext)
        {
          index -= _sizeline;
          string += _L(" ");

          for(XDWORD c=0; c<_sizeline; c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==_C('%')) || (character==_C('\\'))) character = _C('.');

              strdata.Format(_L("%c"), character);
              string += strdata;
              index++;
            }
        }

      Print(level, string.Get());

      if(_size>XTRACE_MAXSIZEDATABLOCK) break;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintDataBlock(XBYTE level, XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block
* @ingroup    XUTILS
* 
* @param[in]  level : level of line
* @param[in]  data : buffer of data to print
* @param[in]  marginsize : size of margin
* @param[in]  sizeline : size of line
* @param[in]  showoffset : true if had offset
* @param[in]  showtext : true if show assci of buffer
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintDataBlock(XBYTE level, XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  return PrintDataBlock(level, data.Get(), data.GetSize(), marginsize, sizeline, showoffset, showtext);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ClearMsgsStatus(XBYTE level)
* @brief      Clear msgs status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ClearMsgsStatus(XBYTE level)
{
  XSTRING string;

  string.Format(_L("%s"), XTRACE_IDMSGSTATUSCLEAR);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, bool value)
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, bool value)
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%s"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_BOOLEAN, (value?_L("true"):_L("false")));

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, int value)
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, int value)
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%d"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_INTEGER, value);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XCHAR* value)
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XCHAR* value)
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%s"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_STRING, value);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XDWORD value)
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XDWORD value)
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%d"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_DWORD, value);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, float value)
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, float value)
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%f"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_FLOAT, value);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XBYTE value[3])
* @brief      Print msg status
* @ingroup    XUTILS
* 
* @param[in]  level : Level value.
* @param[in]  name : Name to use.
* @param[in]  value : Value value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgStatus(XBYTE level, XCHAR* name, XBYTE value[3])
{
  XSTRING string;

  string.Format(_L("%s,%s,%c,%02X,%02X,%02X"), XTRACE_IDMSGSTATUS, name, XTRACE_IDMSGSTATUS_COLOR, value[0], value[1], value[2]);

  return Print(level, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::PrintMsgTests(XDWORD ID, int error)
* @brief      print msg tests
* @ingroup    XUTILS
* 
* @param[in]  ID : 
* @param[in]  error : 
* 
* @return     bool : true if is succesful. 
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::PrintMsgTests(XDWORD ID, int error) 
{
  XSTRING string;

  string.Format(_L("%s,%d,%d"), XTRACE_IDMSGTESTS, ID, error);

  return Print(0, string.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Tests_Load(XCHAR* path)
* @brief      Load tests catalog (JSON: id + description) into memory
* @ingroup    XUTILS
* 
* @param[in]  path : Path to the JSON catalog file.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Tests_Load(XCHAR* path)
{
  if(!path) return false;

  XSTRING pathstr = path;

  return Tests_Load(pathstr);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Tests_Load(XSTRING& path)
* @brief      Load tests catalog (JSON: id + description) into memory
* @ingroup    XUTILS
* 
* @param[in]  path : Path to the JSON catalog file.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Tests_Load(XSTRING& path)
{
  if(path.IsEmpty()) return false;

  #if defined(XFILE_ACTIVE) && defined(XFILE_JSON_ACTIVE)

  XPATH     xpath;
  XFILEJSON xfilejson;

  xpath = path;

  if(!xfilejson.Open(xpath))       return false;
  if(!xfilejson.ReadAllFile())     { xfilejson.Close(); return false; }
  if(!xfilejson.DecodeAllLines())  { xfilejson.Close(); return false; }

  Tests_DeleteAll();

  XFILEJSONARRAY* tests = (XFILEJSONARRAY*)xfilejson.GetObj(_L("tests"));
  if(!tests)
    {
      xfilejson.Close();
      return false;
    }

  for(XDWORD c=0; c<tests->GetValues()->GetSize(); c++)
    {
      XFILEJSONVALUE* entry = tests->GetValues()->Get(c);
      if(!entry) continue;
      if(entry->GetType() != XFILEJSONVALUETYPE_OBJECT) continue;

      XFILEJSONOBJECT* object = entry->GetValueObject();
      if(!object) continue;

      XFILEJSONVALUE* idvalue   = xfilejson.GetValue(_L("id")         , object);
      XFILEJSONVALUE* descvalue = xfilejson.GetValue(_L("description"), object);
      if(!idvalue || !descvalue) continue;

      XDWORD id = 0;

      switch(idvalue->GetType())
        {
          case XFILEJSONVALUETYPE_STRING : id = idvalue->GetValueString().ConvertToDWord(); break;
          default                        : id = (XDWORD)idvalue->GetValueInteger();         break;
        }

      XSTRING description = descvalue->GetValueString();

      if(!id) continue;
      if(Tests_Exists(id)) continue;

      XSTRING* descriptioncopy = GEN_NEW XSTRING();
      if(!descriptioncopy) continue;

      (*descriptioncopy) = description;
      if(!testsdescriptions.Add(id, descriptioncopy))
        {
          GEN_DELETE descriptioncopy;
        }
    }

  xfilejson.Close();

  return (testsdescriptions.GetSize() > 0);

  #else

  return false;

  #endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Tests_Exists(XDWORD ID)
* @brief      Check whether a test ID exists in the loaded catalog
* @ingroup    XUTILS
* 
* @param[in]  ID : Test identifier.
* 
* @return     bool : true if the ID is present; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Tests_Exists(XDWORD ID)
{
  if(!ID) return false;

  return (testsdescriptions.Find(ID) != NOTFOUND);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Tests_GetDescription(XDWORD ID, XSTRING& description)
* @brief      Get description for a test ID from the loaded catalog
* @ingroup    XUTILS
* 
* @param[in]  ID : Test identifier.
* @param[out] description : Description text when found.
* 
* @return     bool : true if the ID exists; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Tests_GetDescription(XDWORD ID, XSTRING& description)
{
  description.Empty();

  if(!Tests_Exists(ID)) return false;

  XSTRING* stored = testsdescriptions.Get(ID);
  if(!stored) return false;

  description = (*stored);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::Tests_GetSize()
* @brief      Get number of loaded test catalog entries
* @ingroup    XUTILS
* 
* @return     XDWORD : Number of entries.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::Tests_GetSize()
{
  return testsdescriptions.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::Tests_DeleteAll()
* @brief      Clear loaded tests catalog from memory
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::Tests_DeleteAll()
{
  testsdescriptions.DeleteElementContents();
  testsdescriptions.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print special
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::PrintSpecial(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print file
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::PrintFile(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
* @brief      Print net
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  target : target to print the debug
* @param[in]  level : level of debug
* @param[in]  string : string with the line
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::PrintNet(XTRACE_TARGET* target, XBYTE level, XCHAR* string)
{

}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::GetHandleNet(XTRACE_TARGET* target)
* @brief      Get handle net
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::GetHandleNet(XTRACE_TARGET* target)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::CloseHandleNet(XTRACE_TARGET* target)
* @brief      Close handle net
* @note       VIRTUAL
* @ingroup    XUTILS
* 
* @param[in]  target : target of debug
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::CloseHandleNet(XTRACE_TARGET* target)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::GetTraceFromXBuffer(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data)
* @brief      Get trace from X buffer
* @ingroup    XUTILS
* 
* @param[in]  xbufferpacket : Xbufferpacket value.
* @param[in]  publicIP : Public IP value.
* @param[in]  localIP : Local IP value.
* @param[in]  level : Level value.
* @param[in]  sequence : Sequence value.
* @param[in]  xtime : Xtime pointer to use.
* @param[in]  data : Data buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::GetTraceFromXBuffer(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data)
{
  if(!xtime)                   return 1;
  if(!xbufferpacket.GetSize()) return 1;

  XDWORD IDpacket = XTRACE_IDPACKET;
  
  XDWORD    sizepacketread  = 0;
  XDWORD    dword;
  XDWORD    sizeinbuffer    = xbufferpacket.GetSize();
  XBUFFER   xbuffer(false);
  XDWORD    sizeheader      = (sizeof(XDWORD)    +    // ID.
                               sizeof(XDWORD)    +    // SizePacket.
                               sizeof(XDWORD)    +    // public IP address origin.
                               sizeof(XDWORD)    +    // local  IP address origin.
                               sizeof(XBYTE)     +    // Level.
                               sizeof(XDWORD)    +    // Sequence.
                              (sizeof(XDWORD)*7) +    // Date Time.
                               sizeof(XWORD));        // CRC16

  if(!sizeinbuffer)             return 1;
  if(sizeinbuffer < sizeheader) return 1;


  while(sizeinbuffer-4)
    {
      xbufferpacket.Get(IDpacket, 0);
      if(IDpacket == XTRACE_IDPACKET) break;
     
      xbufferpacket.Extract(NULL, 0, 1);
      sizeinbuffer--;
    }

  if(IDpacket != XTRACE_IDPACKET) return 1;

  if(!sizeinbuffer)               return 1;
  if(sizeinbuffer < sizeheader)   return 1;

  if(!xbufferpacket.Get(sizepacketread, sizeof(IDpacket)))
    {
      return 1;
    }

  if(!sizepacketread)
    {
      return 2;
    }

  if(sizepacketread > sizeinbuffer)
    {
      return 1;
    }

 
  XDWORD sizeassing    = sizepacketread;
  XDWORD sizeassingmin = 5*1024;

  if(sizeassing < sizeassingmin) sizeassing = sizeassingmin;
  xbuffer.Resize(sizeassing);

  xbufferpacket.Extract(xbuffer.Get(), 0, sizepacketread);

  XWORD crc16;
  XTRACE_CRC16(xbuffer.Get(), (xbuffer.GetSize()-sizeof(XWORD)), crc16)

  XWORD crc16read;

  xbuffer.Get(crc16read, xbuffer.GetSize()-sizeof(XWORD));
  if(crc16 != crc16read)
    {
      return 2;
    }

  xbuffer.SetPosition(0);

  xbuffer.Get(IDpacket);
  xbuffer.Get(sizepacketread);
  xbuffer.Get(publicIP);
  xbuffer.Get(localIP);
  xbuffer.Get(level);
  xbuffer.Get(sequence);

  xbuffer.Get(dword); xtime->SetYear(dword);
  xbuffer.Get(dword); xtime->SetMonth(dword);
  xbuffer.Get(dword); xtime->SetDay(dword);
  xbuffer.Get(dword); xtime->SetHours(dword);
  xbuffer.Get(dword); xtime->SetMinutes(dword);
  xbuffer.Get(dword); xtime->SetSeconds(dword);
  xbuffer.Get(dword); xtime->SetMilliSeconds(dword);

  XDWORD sizestr = sizepacketread - sizeheader;        // CRC16  
  data.Empty();
  data.Add(&xbuffer.Get()[xbuffer.GetPosition()], sizestr);


  return 0;
}


#ifdef DIO_ACTIVE
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::GetTraceFromDIOStream(DIOSTREAM* DIOstream, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data)
* @brief      Get trace from dio stream
* @ingroup    XUTILS
* 
* @param[in]  DIOstream : DI Ostream pointer to use.
* @param[in]  publicIP : Public IP value.
* @param[in]  localIP : Local IP value.
* @param[in]  level : Level value.
* @param[in]  sequence : Sequence value.
* @param[in]  xtime : Xtime pointer to use.
* @param[in]  data : Data buffer to use.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::GetTraceFromDIOStream(DIOSTREAM* DIOstream, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XBUFFER& data)
{
  if(!DIOstream) return false;
  if(!xtime)    return false;

  return GetTraceFromXBuffer((*DIOstream->GetInXBuffer()), publicIP, localIP, level, sequence, xtime, data);
}
#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetTraceDataToText(XBUFFER& data, XSTRING& string)
* @brief      Set trace data to text
* @ingroup    XUTILS
* 
* @param[in]  data : Data buffer to use.
* @param[in]  string : String to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetTraceDataToText(XBUFFER& data, XSTRING& string)
{
  string.Empty();

  for(XDWORD c=0; c<data.GetSize()/2; c++)
    {
      XWORD  word = 0;

      data.Get(word);
      string.Add((XCHAR)word);
    }  

  return (string.IsEmpty()?false:true);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetTraceToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XDWORD& sizepacket, XBUFFER& xbufferpacket)
* @brief      Set trace to X buffer
* @ingroup    XUTILS
* 
* @param[in]  publicIP : Public IP value.
* @param[in]  localIP : Local IP value.
* @param[in]  level : Level value.
* @param[in]  sequence : Sequence value.
* @param[in]  xtime : Xtime pointer to use.
* @param[in]  sizepacket : Sizepacket value.
* @param[in]  xbufferpacket : Xbufferpacket value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetTraceToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XDWORD& sizepacket, XBUFFER& xbufferpacket)
{
  if(!xtime)  return false;
    
  xbufferpacket.Empty();

  XDWORD IDpacket = XTRACE_IDPACKET;
 
  sizepacket += sizeof(XDWORD) +                        // ID.
                sizeof(XDWORD) +                        // SizePacket.
                sizeof(XDWORD) +                        // public IP address origin.
                sizeof(XDWORD) +                        // local  IP address origin.
                sizeof(XBYTE)  +                        // Level.
                sizeof(XDWORD) +                        // Sequence.
                (sizeof(XDWORD)*7) +                    // Date Time.                
                sizeof(XWORD);                          // CRC16

  xbufferpacket.Resize(sizepacket);

  xbufferpacket.Set(IDpacket);  
  xbufferpacket.Set(sizepacket);
  xbufferpacket.Set(publicIP);
  xbufferpacket.Set(localIP);
  xbufferpacket.Set(level);
  xbufferpacket.Set(sequence);

  xbufferpacket.Set((XDWORD)xtime->GetYear());
  xbufferpacket.Set((XDWORD)xtime->GetMonth());
  xbufferpacket.Set((XDWORD)xtime->GetDay());
  xbufferpacket.Set((XDWORD)xtime->GetHours());
  xbufferpacket.Set((XDWORD)xtime->GetMinutes());
  xbufferpacket.Set((XDWORD)xtime->GetSeconds());
  xbufferpacket.Set((XDWORD)xtime->GetMilliSeconds());

  return true;  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetTraceTextToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket)
* @brief      Set trace text to X buffer
* @ingroup    XUTILS
* 
* @param[in]  publicIP : Public IP (in DWORD format)
* @param[in]  localIP : Local IP (in DWORD format)
* @param[in]  level : Level of debug
* @param[in]  sequence : sequence of debug
* @param[in]  xtime : date/time of line
* @param[in]  string : string with of line
* @param[in]  xbufferpacket : buffer with debug pack
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetTraceTextToXBuffer(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket)
{     
  if(!string) return false;
  
  XSTRING  line;
  XBUFFER  bufferline;

  line = string;  
  line.ConvertToExchangeXBuffer(bufferline);
  
  XDWORD sizepacket = bufferline.GetSize();    // Line (WORD) each character.
  
  if(!SetTraceToXBuffer(publicIP, localIP, level, sequence, xtime, sizepacket, xbufferpacket)) return false;
  
  xbufferpacket.Set(bufferline.Get(), bufferline.GetSize());
  
  XWORD crc16;
  XTRACE_CRC16(xbufferpacket.Get(), (xbufferpacket.GetSize()-sizeof(XWORD)), crc16)

  xbufferpacket.Set((XWORD)crc16);
    
  return true; 
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::SetLocalIPFilter(XBYTE localIPfilter)
* @brief      Set local IP filter
* @ingroup    XUTILS
* 
* @param[in]  localIPfilter : Local I Pfilter value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::SetLocalIPFilter(XBYTE localIPfilter)
{
  this->localIPfilter = localIPfilter;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ObtainLocalIP()
* @brief      Obtain local IP
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ObtainLocalIP()
{
  bool status = false;

  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

  DIOSTREAMIPLOCALENUMDEVICES* enumdevices = (DIOSTREAMIPLOCALENUMDEVICES*)GEN_DIOFACTORY.CreateStreamEnumDevices(DIOSTREAMENUMTYPE_IP_LOCAL);
  if(enumdevices)
    {
      if(!localIPfilter)
        {
          DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetFirstActiveDevice();
          if(device)
            {
              XSTRING string;

              device->GetIP()->GetXString(string);
              SetLocalIPString(string.Get());

              status = true;
            }
        }
       else
        {
          enumdevices->Search();

          for(XDWORD c=0; c<enumdevices->GetDevices()->GetSize(); c++)
            {
              DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)enumdevices->GetDevices()->Get(c);
              if(device)
                {
                  //device->DebugPrintInfo();

                  if((!device->GetIP()->IsEmpty()) &&
                     ((device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_ETHERNET) || (device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_WIFI) || (device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_PPP)) &&
                      (device->GetIP()->Get()[0] == localIPfilter))
                    {
                      XSTRING string;

                      device->GetIP()->GetXString(string);
                      SetLocalIPString(string.Get());
                      status = true;
                    }
                }
            }
        }

      GEN_DIOFACTORY.DeleteStreamEnumDevices(enumdevices);
    }

  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetLocalIPString(XCHAR* localIPstring)
* @brief      Set local IP string
* @ingroup    XUTILS
* 
* @param[in]  localIPstring : GEN_NEW local ip in string format
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetLocalIPString(XCHAR* localIPstring)
{
  XSTRING _localIPstring;
  int data[4];

  _localIPstring = localIPstring;
  _localIPstring.UnFormat(_L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

  localIP  |= (data[0] << 24);
  localIP  |= (data[1] << 16);
  localIP  |= (data[2] << 8);
  localIP  |= (data[3]);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::GetLocalIP()
* @brief      Get local IP
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::GetLocalIP()
{
  return localIP;
}


#ifndef XTRACE_NOINTERNET
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ObtainPublicIP()
* @brief      Obtain public IP
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ObtainPublicIP()
{
  bool  status = false;

  #if(defined(DIO_ACTIVE) && defined(DIO_STREAMTCPIP_ACTIVE))
 
  XSTRING string;
  status = GEN_DIOPUBLICINTERNETIP.Get(string);
  if(status)
    {
      SetPublicIPString(string.Get());      
    }

  #endif

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD XTRACE::GetPublicIP()
* @brief      Get public IP
* @ingroup    XUTILS
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD XTRACE::GetPublicIP()
{
  return publicIP;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::SetPublicIPString(XCHAR* publicIPstring)
* @brief      Set public IP string
* @ingroup    XUTILS
* 
* @param[in]  publicIPstring : Public I Pstring pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::SetPublicIPString(XCHAR* publicIPstring)
{
  XSTRING _publicIPstring;
  int data[4];

  _publicIPstring = publicIPstring;

  _publicIPstring.UnFormat(_L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

  publicIP  |= (data[0] << 24);
  publicIP  |= (data[1] << 16);
  publicIP  |= (data[2] << 8);
  publicIP  |= (data[3]);

  return true;
}

#endif


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ObtainResourcesIP()
* @brief      Obtain resources IP
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ObtainResourcesIP()
{
  if(!ObtainLocalIP())  
    {
      return false;
    }

  #ifndef XTRACE_NOINTERNET
  if(!ObtainPublicIP()) 
    {
      return false;
    }
  #endif

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ReConnectedAllNetTargets()
* @brief      Re connected all net targets
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ReConnectedAllNetTargets()
{
  for(int c=0; c<XTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XTRACE_TYPE_NET)
        {
          #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

          if(targets[c].GetNETHandle()) CloseHandleNet(&targets[c]);

          GetHandleNet(&targets[c]);

          #endif
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ResolveAllNetTargets(bool& changed)
* @brief      Resolve all net targets
* @ingroup    XUTILS
* 
* @param[in]  changed : true if the target net changed IP
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ResolveAllNetTargets(bool& changed)
{
  changed = false;

  for(int c=0; c<XTRACE_MAXNTARGETS; c++)
    {
      if(targets[c].GetType() == XTRACE_TYPE_NET)
        {
          #if(defined(DIO_ACTIVE) && defined(DIO_STREAMUDP_ACTIVE))

          XSTRING IP;

          IP = targets[c].GetIP();

          targets[c].IPTarget();

          if(IP.Compare(targets[c].GetIP())) changed = true;

          #endif
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::ResolveAllResources()
* @brief      Resolve all resources
* @ingroup    XUTILS
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::ResolveAllResources()
{
  bool changed;

  ResolveAllNetTargets(changed);
  if(changed)
    {
      ObtainResourcesIP();
      ReConnectedAllNetTargets();
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XTRACE::GenerateTab(int ntab, XSTRING& tab)
* @brief      Generate tab
* @ingroup    XUTILS
* 
* @param[in]  ntab : number of tab
* @param[in]  tab : string with the tab generated
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XTRACE::GenerateTab(int ntab, XSTRING& tab)
{
  tab.Empty();

  for(int c=0;c<ntab;c++)
    {
      tab += _L(" ");
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XTRACE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    XUTILS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XTRACE::Clean()
{
  instance                  = NULL;

  sizelimit                 = XTRACE_SIZELIMITDEFAULT;

  for(int c=0; c<_MAXSTR; c++)
    {
      applicationname[c] = 0;
      applicationID[c]   = 0;
    }

  applicationversion        = 0;
  applicationsubversion     = 0;
  applicationsubversionerr  = 0;

  #ifndef XTRACE_NOINTERNET
  publicIP                  = 0;
  #endif
  localIPfilter             = 0;
  localIP                   = 0;

  openheader                = false;
  sequence                  = 0;
}





