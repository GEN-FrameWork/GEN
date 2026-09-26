/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOApplicationUpdate_XEvent.cpp
* 
* @class      DIOAPPLICATIONUPDATE_XEVENT
* @brief      Data Input/Output Application Update eXtended Event class
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

#include "DIOApplicationUpdate_XEvent.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_XEVENT::DIOAPPLICATIONUPDATE_XEVENT(XSUBJECT* subject, XDWORD family, XDWORD type) : XEVENT(subject, family, type)
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* @param[in]  subject : Subject pointer to use.
* @param[in]  family : Family value.
* @param[in]  type : Type value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_XEVENT::DIOAPPLICATIONUPDATE_XEVENT(XSUBJECT* subject, XDWORD family, XDWORD type) : XEVENT(subject, family, type)
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_XEVENT::~DIOAPPLICATIONUPDATE_XEVENT()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_XEVENT::~DIOAPPLICATIONUPDATE_XEVENT()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_VERSIONDATA* DIOAPPLICATIONUPDATE_XEVENT::GetVersionData()
* @brief      Get version data
* @ingroup    DATAIO
* 
* @return     DIOAPPLICATIONUPDATE_VERSIONDATA* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_VERSIONDATA* DIOAPPLICATIONUPDATE_XEVENT::GetVersionData()
{
  return versiondata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::SetVersionData(DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata)
* @brief      Set version data
* @ingroup    DATAIO
* 
* @param[in]  versiondata : Versiondata pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::SetVersionData(DIOAPPLICATIONUPDATE_VERSIONDATA* versiondata)
{
  this->versiondata = versiondata;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         int DIOAPPLICATIONUPDATE_XEVENT::GetVersionComparation()
* @brief      Get version comparation
* @ingroup    DATAIO
* 
* @return     int : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
int DIOAPPLICATIONUPDATE_XEVENT::GetVersionComparation()
{
  return versioncomparation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::SetVersionComparation(int versioncomparation)
* @brief      Set version comparation
* @ingroup    DATAIO
* 
* @param[in]  versioncomparation : Versioncomparation value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::SetVersionComparation(int versioncomparation)
{
  this->versioncomparation = versioncomparation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* DIOAPPLICATIONUPDATE_XEVENT::GetFilesToUpdate()
* @brief      Get files to update
* @ingroup    DATAIO
* 
* @return     XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* DIOAPPLICATIONUPDATE_XEVENT::GetFilesToUpdate()
{
  return filestoupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::SetFilesToUpdate(XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* filestoupdate)
* @brief      Set files to update
* @ingroup    DATAIO
* 
* @param[in]  filestoupdate : Filestoupdate pointer to use.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::SetFilesToUpdate(XVECTOR<DIOAPPLICATIONUPDATE_FILE*>* filestoupdate)
{
  this->filestoupdate = filestoupdate;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD DIOAPPLICATIONUPDATE_XEVENT::GetIndexOfOperation()
* @brief      Get index of operation
* @ingroup    DATAIO
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD DIOAPPLICATIONUPDATE_XEVENT::GetIndexOfOperation()
{
  return indexoperation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::SetIndexOfOperation(XDWORD indexoperation)
* @brief      Set index of operation
* @ingroup    DATAIO
* 
* @param[in]  indexoperation : Indexoperation value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::SetIndexOfOperation(XDWORD indexoperation)
{
  this->indexoperation = indexoperation;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_ERROR DIOAPPLICATIONUPDATE_XEVENT::GetError()
* @brief      Get error
* @ingroup    DATAIO
* 
* @return     DIOAPPLICATIONUPDATE_ERROR : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_ERROR DIOAPPLICATIONUPDATE_XEVENT::GetError()
{
  return error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::SetError(DIOAPPLICATIONUPDATE_ERROR error)
* @brief      Set error
* @ingroup    DATAIO
* 
* @param[in]  error : Error value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::SetError(DIOAPPLICATIONUPDATE_ERROR error)
{
  this->error = error;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOAPPLICATIONUPDATE_FILE* DIOAPPLICATIONUPDATE_XEVENT::GetActualFileToUpdate()
* @brief      Get actual file to update
* @ingroup    DATAIO
* 
* @return     DIOAPPLICATIONUPDATE_FILE* : Pointer to the requested object; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOAPPLICATIONUPDATE_FILE* DIOAPPLICATIONUPDATE_XEVENT::GetActualFileToUpdate()
{
  if(!filestoupdate) return NULL;
  return filestoupdate->Get(indexoperation);
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XSTRING* DIOAPPLICATIONUPDATE_XEVENT::GetActualUnzipFileName()
* @brief      Get actual unzip file name
* @ingroup    DATAIO
* 
* @return     XSTRING* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XSTRING* DIOAPPLICATIONUPDATE_XEVENT::GetActualUnzipFileName()
{
  return &actualunzipfilename;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOAPPLICATIONUPDATE_XEVENT::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOAPPLICATIONUPDATE_XEVENT::Clean()
{
  versioncomparation    = 0;
  versiondata           = NULL;

  filestoupdate         = NULL;

  indexoperation        = 0;
  error                 = DIOAPPLICATIONUPDATE_NOT_ERROR;
}



