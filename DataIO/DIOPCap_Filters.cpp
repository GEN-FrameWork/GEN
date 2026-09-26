/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOPCap_Filters.cpp
* 
* @class      DIOPCAP_FILTERS
* @brief      Data Input/Output PCap Library Filters class
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



#ifdef DIO_PCAP_ACTIVE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "DIOPCap_Filters.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTER_ENTRY::DIOPCAP_FILTER_ENTRY()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTER_ENTRY::DIOPCAP_FILTER_ENTRY()
{
  Clean();

  ports.SetIsMulti(false);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTER_ENTRY::~DIOPCAP_FILTER_ENTRY()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTER_ENTRY::~DIOPCAP_FILTER_ENTRY()
{
  Ports_DelAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOPCAP_FILTER_ENTRY::GetName()
* @brief      Get name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOPCAP_FILTER_ENTRY::GetName()
{
  return &name;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPCAP_FILTER_ENTRY::GetProtocolType()
* @brief      Get protocol type
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPCAP_FILTER_ENTRY::GetProtocolType()
{
  return protocoltype;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTER_ENTRY::SetProtocolType(XDWORD protocoltype)
* @brief      Set protocol type
* @ingroup    DATAIO
* 
* @param[in]  protocoltype : Protocoltype value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTER_ENTRY::SetProtocolType(XDWORD protocoltype)
{
  this->protocoltype = protocoltype;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XDWORD>* DIOPCAP_FILTER_ENTRY::Ports_GetAll()
* @brief      Ports get all
* @ingroup    DATAIO
* 
* @return     XVECTOR<XDWORD>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XDWORD>* DIOPCAP_FILTER_ENTRY::Ports_GetAll()
{
  return &ports;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTER_ENTRY::Ports_Exists(XDWORD port)
* @brief      Ports exists
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTER_ENTRY::Ports_Exists(XDWORD port)
{
  for(XDWORD c=0; c<ports.GetSize(); c++)
    {
      XDWORD _port = ports.Get(c);
      if(_port == port) 
        {
          return true;
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOPCAP_FILTER_ENTRY::Ports_Get(int index)
* @brief      Ports get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOPCAP_FILTER_ENTRY::Ports_Get(int index)
{
  if(index < 0) 
    {
      return 0;
    }

  if(index >= ports.GetSize())
    {
      return 0;
    }

  return ports.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTER_ENTRY::Ports_Add(XDWORD port)
* @brief      Ports add
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTER_ENTRY::Ports_Add(XDWORD port)
{
  if(Ports_Exists(port))
    {
      return true;
    }

  return ports.Add(port);  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTER_ENTRY::Ports_Del(XDWORD port)
* @brief      Ports del
* @ingroup    DATAIO
* 
* @param[in]  port : Port number to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTER_ENTRY::Ports_Del(XDWORD port)
{
  if(!Ports_Exists(port))
    {
      return false;
    }

  return ports.Delete(port);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTER_ENTRY::Ports_DelAll()
* @brief      Ports del all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTER_ENTRY::Ports_DelAll()
{
  if(!ports.GetSize())
    {
      return false;
    }

  return ports.DeleteAll();
} 


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPCAP_FILTER_ENTRY::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPCAP_FILTER_ENTRY::Clean()
{
  name.Empty();
  protocoltype = 0x00;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTERS::DIOPCAP_FILTERS()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTERS::DIOPCAP_FILTERS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTERS::~DIOPCAP_FILTERS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTERS::~DIOPCAP_FILTERS()
{
  Entrys_DelAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOPCAP_FILTER_ENTRY*>* DIOPCAP_FILTERS::Entrys_GetAll()
* @brief      Entrys get all
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOPCAP_FILTER_ENTRY*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOPCAP_FILTER_ENTRY*>* DIOPCAP_FILTERS::Entrys_GetAll()
{
  return &entrys;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(XCHAR* name)
* @brief      Entrys get
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* 
* @return     DIOPCAP_FILTER_ENTRY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(XCHAR* name)
{
  for(XDWORD c=0; c<entrys.GetSize(); c++)
    {
      DIOPCAP_FILTER_ENTRY* _entry = entrys.Get(c);
      if(_entry)
        {
          if(!_entry->GetName()->Compare(name, true))
            {
              return _entry;
            }
        }
    }

  return NULL;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(XSTRING& name)
* @brief      Entrys get
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* 
* @return     DIOPCAP_FILTER_ENTRY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(XSTRING& name)
{
  return Entrys_Get(name.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(int index)
* @brief      Entrys get
* @ingroup    DATAIO
* 
* @param[in]  index : Index value.
* 
* @return     DIOPCAP_FILTER_ENTRY* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOPCAP_FILTER_ENTRY* DIOPCAP_FILTERS::Entrys_Get(int index)
{
  if(index < 0) 
    {
      return NULL;
    }

  if(index >= entrys.GetSize())
    {
      return NULL;
    }

  return entrys.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTERS::Entrys_Set(XCHAR* name, XDWORD typeprotocol, int nports, ...)
* @brief      Entrys set
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  typeprotocol : Typeprotocol value.
* @param[in]  nports : Nports value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTERS::Entrys_Set(XCHAR* name, XDWORD typeprotocol, int nports, ...)
{
  DIOPCAP_FILTER_ENTRY* _entry = Entrys_Get(name);
  if(!_entry)
    {
      return false;
    }

  _entry->SetProtocolType(typeprotocol);

  _entry->Ports_DelAll();

  va_list arg;

  va_start(arg, nports);

  for(int c=0; c<nports; c++)
    {
      int port = va_arg(arg, int);

      _entry->Ports_Add((XDWORD)port);  
    }

  va_end(arg);
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTERS::Entrys_Add(XCHAR* name, XDWORD typeprotocol, int nports, ...)
* @brief      Entrys add
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* @param[in]  typeprotocol : Typeprotocol value.
* @param[in]  nports : Nports value.
* @param[in]  ... : Variable argument list.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTERS::Entrys_Add(XCHAR* name, XDWORD typeprotocol, int nports, ...)
{
  DIOPCAP_FILTER_ENTRY* _entry = Entrys_Get(name);
  if(!_entry)
    {
      _entry = GEN_NEW DIOPCAP_FILTER_ENTRY();
      if(!_entry)
        {
          return false;  
        }
    }

  _entry->SetProtocolType(typeprotocol);

  va_list arg;

  va_start(arg, nports);

  for(int c=0; c<nports; c++)
    {
      int port = va_arg(arg, int);

      _entry->Ports_Add((XDWORD)port);  
    }

  va_end(arg);

  return entrys.Add(_entry);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTERS::Entrys_Del(XCHAR* name)
* @brief      Entrys del
* @ingroup    DATAIO
* 
* @param[in]  name : Name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTERS::Entrys_Del(XCHAR* name)
{
  DIOPCAP_FILTER_ENTRY* _entry = Entrys_Get(name);
  if(!_entry)
    {
      return false;
    }

  return entrys.Delete(_entry);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTERS::Entrys_Check(DIOPCAPFRAME* frame)
* @brief      Entrys check
* @ingroup    DATAIO
* 
* @param[in]  frame : Frame pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTERS::Entrys_Check(DIOPCAPFRAME* frame)
{
  bool valid = false;

  if(!frame)
    {
      return false;
    }
  
  if(!entrys.GetSize())
    {
      return true;
    }

  for(XDWORD c=0; c<entrys.GetSize(); c++)
    {
      DIOPCAP_FILTER_ENTRY* entry = entrys.Get(c);
      if(entry)
        {
          XDWORD protocoltype = entry->GetProtocolType();
         
          if((protocoltype & DIOPCAPPROTOCOL_TYPE_ICMP) == frame->GetProtocolType())
            {
              valid = true; 
            }

          if((protocoltype & DIOPCAPPROTOCOL_TYPE_UDP) == frame->GetProtocolType())
            {
              valid = true; 
            }

          if((protocoltype & DIOPCAPPROTOCOL_TYPE_TCP) == frame->GetProtocolType())
            {
              valid = true; 
            }
        
          if(valid)
            {
              if(!entry->Ports_GetAll()->GetSize())
                {
                  return valid;
                }   

              for(XDWORD d=0; d<entry->Ports_GetAll()->GetSize(); d++) 
                {
                  XDWORD port_target  = entry->Ports_GetAll()->Get(d);
                  XDWORD port_source  = (port_target & ~DIOPCAP_FILTERS_PORT_TARGET);
                                      
                  if(port_source == frame->GetSourcePort())
                    {
                      return valid;
                    }        

                  if(port_target == frame->GetTargetPort())
                    {
                      return valid;  
                    }          
                }
            }
        }
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOPCAP_FILTERS::Entrys_DelAll()
* @brief      Entrys del all
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOPCAP_FILTERS::Entrys_DelAll()
{
  if(entrys.IsEmpty())
    {
      return false;
    }

  entrys.DeleteContents();
  entrys.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOPCAP_FILTERS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOPCAP_FILTERS::Clean()
{

}




#endif