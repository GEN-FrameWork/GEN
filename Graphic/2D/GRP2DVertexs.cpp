/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       GRP2DVertexs.cpp
* 
* @class      GRP2DVERTEXS
* @brief      Graphics 2D Vertexs class
* @ingroup    GRAPHIC
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

#include "GRP2DVertexs.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/




/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVERTEX::GRP2DVERTEX()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVERTEX::GRP2DVERTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVERTEX::~GRP2DVERTEX()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVERTEX::~GRP2DVERTEX()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVERTEX::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVERTEX::Clean()
{
   x = 0;
   y = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVERTEXS::GRP2DVERTEXS()
* @brief      Constructor of class
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVERTEXS::GRP2DVERTEXS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVERTEXS::~GRP2DVERTEXS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVERTEXS::~GRP2DVERTEXS()
{
  DeleteAll();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVERTEXS::Add(double x, double y)
* @brief      Add value
* @ingroup    GRAPHIC
* 
* @param[in]  x : x pos
* @param[in]  y : y pos
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVERTEXS::Add(double x, double y)
{
  GRP2DVERTEX* vertex = GEN_NEW GRP2DVERTEX();
  if(!vertex) return false;

  vertex->x = x;
  vertex->y = y;

  return vertexs.Add(vertex);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD GRP2DVERTEXS::GetSize()
* @brief      Get size
* @ingroup    GRAPHIC
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD GRP2DVERTEXS::GetSize()
{
  return vertexs.GetSize();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         GRP2DVERTEX* GRP2DVERTEXS::Get(int index)
* @brief      Get value
* @ingroup    GRAPHIC
* 
* @param[in]  index : index of vertex
* 
* @return     GRP2DVERTEX* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
GRP2DVERTEX* GRP2DVERTEXS::Get(int index)
{
  return vertexs.Get(index);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool GRP2DVERTEXS::DeleteAll()
* @brief      Delete all
* @ingroup    GRAPHIC
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool GRP2DVERTEXS::DeleteAll()
{
  if(vertexs.IsEmpty()) return true;

  vertexs.DeleteContents();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void GRP2DVERTEXS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    GRAPHIC
* 
* --------------------------------------------------------------------------------------------------------------------*/
void GRP2DVERTEXS::Clean()
{

}





