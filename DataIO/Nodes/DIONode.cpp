/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIONode.cpp
* 
* @class      DIONODE
* @brief      Data Input/Output NODE (Base class for a control of nodes) class
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

#include "DIONode.h"

#include "DIONodeItem.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODE::DIONODE()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODE::DIONODE()

{
  Clean();
  
  UUID.GenerateRandom();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIONODE::~DIONODE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIONODE::~DIONODE()
{
  items.DeleteContents();
  items.DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XUUID& DIONODE::GetID()
* @brief      Get ID
* @ingroup    DATAIO
* 
* @return     XUUID& : Reference to the requested object.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XUUID& DIONODE::GetID()
{
  return UUID;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODE::SetID(XUUID& UUID)
* @brief      Set ID
* @ingroup    DATAIO
* 
* @param[in]  UUID : UUID value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODE::SetID(XUUID& UUID)
{
  this->UUID.CopyFrom(UUID);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIONODEITEM*>* DIONODE::GetItems()
* @brief      Get items
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIONODEITEM*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIONODEITEM*>* DIONODE::GetItems()
{
  return &items;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::IsLocal()
* @brief      Is local
* @ingroup    DATAIO
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::IsLocal()
{
  return islocal;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODE::SetIsLocal(bool islocal)
* @brief      Set is local
* @ingroup    DATAIO
* 
* @param[in]  islocal : Islocal value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODE::SetIsLocal(bool islocal)
{
  this->islocal = islocal;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIONODE::GetGroup()
* @brief      Get group
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIONODE::GetGroup()
{
  return &group;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::GetSubGroups(XVECTOR<XSTRING*>& listsubgroups)
* @brief      Get sub groups
* @ingroup    DATAIO
* 
* @param[in]  listsubgroups : Listsubgroups pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::GetSubGroups(XVECTOR<XSTRING*>& listsubgroups)
{  
  XPATH pathgroup;  

  pathgroup = group.Get();  
  
  pathgroup.Slash_Delete();

  do{ XSTRING* _subgroup = GEN_NEW XSTRING();
      if(_subgroup)
        {
          pathgroup.GetNamefileExt(*_subgroup);        
          pathgroup.GetPath(pathgroup);

          if(!_subgroup->Compare(pathgroup, true))
            {
              listsubgroups.Insert(0, _subgroup); 
              break;
            }

          pathgroup.Slash_Delete();

          if(_subgroup->IsEmpty())
            {
              GEN_DELETE _subgroup;
              break;
            }
           else 
            {
              listsubgroups.Insert(0, _subgroup);
            }       
        } 
       else 
        {
          break;
        }

   } while(1);

  
  return listsubgroups.IsEmpty()?false:true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::GetSubGroup(XDWORD level, XSTRING& subgroup)
* @brief      Get sub group
* @ingroup    DATAIO
* 
* @param[in]  level : Level value.
* @param[in]  subgroup : Subgroup value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::GetSubGroup(XDWORD level, XSTRING& subgroup)
{
  subgroup.Empty();

  XVECTOR<XSTRING*> listsubgroups;
  bool              status;  

  if(!GetSubGroups(listsubgroups))
    {
      return false;
    }

  if(level < listsubgroups.GetSize())
    {
      subgroup = listsubgroups.Get(level)->Get();
      status = true;
    }

  listsubgroups.DeleteContents();
  listsubgroups.DeleteAll();

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::Update()
* @brief      Update
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::Update()
{
  for(XDWORD c=0; c<items.GetSize(); c++)
    {
      DIONODEITEM* item = items.Get(c);
      if(item)
        {
          item->Update();
        }
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::CreateJSONSerialization()
* @brief      Create JSON serialization
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::CreateJSONSerialization()
{
  XFILEJSON             xfileJSON;
  XSERIALIZATIONMETHOD* serializationmethod;
  XSTRING               string;

  serializationmethod = XSERIALIZABLE::CreateInstance(xfileJSON);
  if(!serializationmethod) return false;
  
  DoSerialize(serializationmethod);

  XPATH path;
  GEN_XPATHSMANAGER.GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, path);
  path += _L("test.json");

  if(xfileJSON.Create(path))
    {
      xfileJSON.WriteAndEncodeAllLines(true);
      xfileJSON.Close();
    }

  xfileJSON.EncodeAllLines();
  xfileJSON.GetAllInOneLine(string, XFILETXTTYPELF_DEFAULT);
  xfileJSON.ShowTraceJSON(XTRACE_COLOR_PURPLE);

  GEN_DELETE serializationmethod;
  
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::Serialize()
* @brief      Serialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::Serialize()
{  
  XSTRING ID;
 
  UUID.GetToString(ID); 

  Primitive_Add<XSTRING*>(&ID, _L("ID"));
  Primitive_Add<XSTRING*>(&group, _L("group"));
  XVectorClass_Add<DIONODEITEM>(&items, _L("items"), _L("item"));

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIONODE::Deserialize()
* @brief      Deserialize
* @ingroup    DATAIO
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIONODE::Deserialize()
{
  XSTRING ID;

  Primitive_Extract<XSTRING>(ID            , _L("ID"));
  Primitive_Extract<XSTRING>(group         , _L("group"));
  
  XVectorClass_Extract<DIONODEITEM>(&items , _L("items"), _L("item"));

  UUID.SetFromString(ID);

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIONODE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIONODE::Clean()
{
  islocal = false;
  
}



