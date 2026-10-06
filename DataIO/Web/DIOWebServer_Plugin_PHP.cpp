/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       DIOWebServer_Plugin_PHP.cpp
* 
* @class      DIOWEBSERVER_PLUGIN_PHP
* @brief      Data Input/Output Web Server PHP Plugin class
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

#include "DIOWebServer_Plugin_PHP.h"

#include "XFileTXT.h"
#include "XSystem.h"
#include "XTrace.h"

#include "DIOWebServer_QueryStrings.h"
#include "DIOWebServer.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_PLUGIN_PHP::DIOWEBSERVER_PLUGIN_PHP()
* @brief      Constructor of class
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_PLUGIN_PHP::DIOWEBSERVER_PLUGIN_PHP()
{
  Clean();

  GetName()->Set(_L("PHP CGI Pluging"));

  // Only for extension.
  //SetTypeTAG(DIOWEBSERVER_PLUGIN_TYPE_TAGLABEL_PHP);
  //GetLabelTAG()->Set(DIOWEBSERVER_PLUGIN_PHP_TAGLABEL);

  PageExtension_Add(_L(".php"));
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         DIOWEBSERVER_PLUGIN_PHP::~DIOWEBSERVER_PLUGIN_PHP()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
DIOWEBSERVER_PLUGIN_PHP::~DIOWEBSERVER_PLUGIN_PHP()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool DIOWEBSERVER_PLUGIN_PHP::Config(XPATH* pathinterpreter)
* @brief      Config
* @ingroup    DATAIO
* 
* @param[in]  pathinterpreter : Pathinterpreter pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool DIOWEBSERVER_PLUGIN_PHP::Config(XPATH* pathinterpreter)
{
  XSTRING nameexec;

  nameexec = _L("php-cgi");
  #ifdef WINDOWS
  nameexec += _L(".exe");
  #endif

  return DIOWEBSERVER_PLUGIN_CGI::Config(pathinterpreter, nameexec.Get());
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void DIOWEBSERVER_PLUGIN_PHP::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    DATAIO
* 
* --------------------------------------------------------------------------------------------------------------------*/
void DIOWEBSERVER_PLUGIN_PHP::Clean()
{

}




