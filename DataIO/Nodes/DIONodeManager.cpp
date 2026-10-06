/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONodeManager.cpp
* 
* @class      DIONODEMANAGER
* @brief      Data Input/Output Node manager (for managing base nodes) class
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

#include "DIONodeManager.h"

#include "XFactory.h"

#include "DIONode.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEMANAGER::DIONODEMANAGER()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEMANAGER::DIONODEMANAGER()
{
  Clean();  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODEMANAGER::~DIONODEMANAGER()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODEMANAGER::~DIONODEMANAGER()
{
  End();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEMANAGER::Ini()
* @brief      Initialize the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEMANAGER::Ini()
{
  xmutexnodes=GEN_XFACTORY.Create_Mutex();
  if(!xmutexnodes) return false;
  
  xthreadnodes = CREATEXTHREAD(XTHREADGROUPID_SCHEDULER, _L("DIONODEMANAGER::Ini"), ThreadNodes, (void*)this);
  if(!xthreadnodes) return false;

  return xthreadnodes->Ini();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODEMANAGER::End()
* @brief      End the object
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODEMANAGER::End()
{
  if(xthreadnodes)
    {
      xthreadnodes->End();
      DELETEXTHREAD(XTHREADGROUPID_SCHEDULER, xthreadnodes);

      xthreadnodes = NULL;
    }
  
  if(xmutexnodes)
    {
      GEN_XFACTORY.Delete_Mutex(xmutexnodes);
      xmutexnodes = NULL;
    }

  nodes.DeleteContents();
  nodes.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIONODE*>* DIONODEMANAGER::GetNodes()
* @brief      Get nodes
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIONODE*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIONODE*>* DIONODEMANAGER::GetNodes()
{
  return &nodes;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEMANAGER::ThreadNodes(void* data)
* @brief      Thread nodes
* @ingroup    DATAIO
* 
* @param[in]  data : Data buffer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEMANAGER::ThreadNodes(void* data)
{
  DIONODEMANAGER* nodemanager = (DIONODEMANAGER*)data;
  if(!nodemanager) return;

  for(XDWORD c=0; c< nodemanager->GetNodes()->GetSize(); c++)
    {
      DIONODE* node = (DIONODE*)nodemanager->GetNodes()->Get(c);
      if(node)
        {
          if(node->IsLocal())
            {
              node->Update();
            }
        }
    }  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODEMANAGER::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODEMANAGER::Clean()
{
  xmutexnodes   = NULL;
  xthreadnodes  = NULL;
}



