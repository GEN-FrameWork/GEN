/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       INPSimulate.cpp
* 
* @class      INPSIMULATE
* @brief      Input Simulate
* @ingroup    INPUT
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

#include "INPSimulate.h"

#include "XString.h"



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

INPUTSIMULATE_KDB_PC  inputsimulare_KDB_PC[]						= { { 0x01	, _L("Left mouse")						     , ALTERNATIVE_KEY_NONE			  },					
																														{ 0x02	, _L("Right mouse")					     , ALTERNATIVE_KEY_NONE			  },					                                                           
			                                                      { 0x03	, _L("Control-break")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x04	, _L("Middle mouse")					     , ALTERNATIVE_KEY_NONE			  },					 
			                                                      { 0x05	, _L("X1 mouse")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x06	, _L("X2 mouse")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x08	, _L("BACKSPACE")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x09	, _L("TAB")							         , ALTERNATIVE_KEY_NONE			  },																															
			                                                      { 0x0C	, _L("CLEAR")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x0D	, _L("ENTER")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x10	, _L("SHIFT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x11	, _L("CTRL")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x12	, _L("ALT")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x13	, _L("PAUSE")						         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x14	, _L("CAPS LOCK")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, _L("IME Kana mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, _L("IME Hanguel mode")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, _L("IME Hangul mode")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x16	, _L("IME On")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x17	, _L("IME Junja mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x18	, _L("IME final mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x19	, _L("IME Hanja mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x19	, _L("IME Kanji mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1A	, _L("IME Off")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1B	, _L("ESC")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1C	, _L("IME convert")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1D	, _L("IME nonconvert")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1E	, _L("IME accept")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1F	, _L("IME mode change request")	 , ALTERNATIVE_KEY_NONE			 	},					
			                                                      { 0x20	, _L("SPACEBAR")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x21	, _L("PAGE UP")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x22	, _L("PAGE DOWN")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x23	, _L("END")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x24	, _L("HOME")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x25	, _L("LEFT ARROW")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x26	, _L("UP ARROW")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x27	, _L("RIGHT ARROW")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x28	, _L("DOWN ARROW")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x29	, _L("SELECT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2A	, _L("PRINT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2B	, _L("EXECUTE")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2C	, _L("PRINT SCREEN")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2D	, _L("INS")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2E	, _L("DEL")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2F	, _L("HELP")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x30	, _L("0")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x31	, _L("1")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x32	, _L("2")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x33	, _L("3")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x34	, _L("4")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x35	, _L("5")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x36	, _L("6")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x37	, _L("7")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x38	, _L("8")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x39	, _L("9")								         , ALTERNATIVE_KEY_NONE			  },																																	                                                      				
			                                                      { 0x41	, _L("A")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x42	, _L("B")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x43	, _L("C")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x44	, _L("D")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x45	, _L("E")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x46	, _L("F")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x47	, _L("G")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x48	, _L("H")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x49	, _L("I")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4A	, _L("J")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4B	, _L("K")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4C	, _L("L")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4D	, _L("M")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4E	, _L("N")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4F	, _L("O")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x50	, _L("P")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x51	, _L("Q")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x52	, _L("R")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x53	, _L("S")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x54	, _L("T")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x55	, _L("U")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x56	, _L("V")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x57	, _L("W")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x58	, _L("X")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x59	, _L("Y")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5A	, _L("Z")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5B	, _L("Left Windows")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5C	, _L("Right Windows")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5D	, _L("Applications")					     , ALTERNATIVE_KEY_NONE			  },																																		                                                                          
			                                                      { 0x5F	, _L("Computer Sleep")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x61	, _L("Numericpad 1")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x60	, _L("Numericpad 0")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x62	, _L("Numericpad 2")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x63	, _L("Numericpad 3")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x64	, _L("Numericpad 4")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x65	, _L("Numericpad 5")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x66	, _L("Numericpad 6")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x67	, _L("Numericpad 7")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x68	, _L("Numericpad 8")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x69	, _L("Numericpad 9")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6A	, _L("Multiply")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6B	, _L("Add")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6C	, _L("Separator")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6D	, _L("Subtract")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6E	, _L("Decimal")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6F	, _L("Divide")							       , ALTERNATIVE_KEY_NONE			  },	
																														{ 0x70	, _L("F1")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x71	, _L("F2")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x72	, _L("F3")								         , ALTERNATIVE_KEY_NONE			  },								                                                      
			                                                      { 0x73	, _L("F4")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x74	, _L("F5")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x75	, _L("F6")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x76	, _L("F7")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x77	, _L("F8")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x78	, _L("F9")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x79	, _L("F10")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7A	, _L("F11")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7B	, _L("F12")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7C	, _L("F13")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7D	, _L("F14")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7E	, _L("F15")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7F	, _L("F16")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x80	, _L("F17")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x81	, _L("F18")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x82	, _L("F19")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x83	, _L("F20")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x84	, _L("F21")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x85	, _L("F22")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x86	, _L("F23")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x87	, _L("F24")							         , ALTERNATIVE_KEY_NONE			  },																												                                                                          
			                                                      { 0x90	, _L("NUM LOCK")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x91	, _L("SCROLL LOCK")					     , ALTERNATIVE_KEY_NONE			  },					                                                                          
			                                                      { 0xA0	, _L("Left SHIFT")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA1	, _L("Right SHIFT")				 	     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA2	, _L("Left CONTROL")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA3	, _L("Right CONTROL")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA4	, _L("Left ALT")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA5	, _L("Right ALT")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA6	, _L("Browser Back")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA7	, _L("Browser Forward")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA8	, _L("Browser Refresh")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA9	, _L("Browser Stop")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAA	, _L("Browser Search")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAB	, _L("Browser Favorites")				 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAC	, _L("Browser Home")		      		 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAD	, _L("Volume Mute")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAE	, _L("Volume Down")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAF	, _L("Volume Up")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB0	, _L("Next Track")					       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB1	, _L("Previous Track")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB2	, _L("Stop Media")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB3	, _L("Play/Pause Media")					 , ALTERNATIVE_KEY_NONE			  },
			                                                      { 0xB4	, _L("Start Mail")						     , ALTERNATIVE_KEY_NONE			  },					
 			                                                      { 0xB5	, _L("Select Media")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB6	, _L("Start Application 1")			 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB7	, _L("Start Application 2")			 , ALTERNATIVE_KEY_NONE			  },					                                                                          			                                                     			                                                       		
			                                                      { 0xE5	, _L("IME PROCESS")						   , ALTERNATIVE_KEY_NONE			  },					                                                                   
			                                                      { 0xF6	, _L("Attn")							         , ALTERNATIVE_KEY_NONE			  },						
			                                                      { 0xF7	, _L("CrSel")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xF8	, _L("ExSel")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xF9	, _L("Erase EOF")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFA	, _L("Play")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFB	, _L("Zoom")							         , ALTERNATIVE_KEY_NONE			  },	  				
			                                                      { 0xFD	, _L("PA1")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFE	, _L("Clear")							       , ALTERNATIVE_KEY_NONE			  }
                                                          };	


INPUTSIMULATE_KDB_PC  inputsimulare_KDB_INTEL_Spanish[]		= { { 0xDC  , _L("\xBA")								  		 , ALTERNATIVE_KEY_NONE       },
																														{ 0xDC  , _L("\xAA")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xDC  , _L("\\")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x31  , _L("!")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x31  , _L("|")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x32  , _L("\"")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x32  , _L("@")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x33  , _L("\x95")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x33  , _L("#")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x34  , _L("$")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x35  , _L("%")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x36  , _L("&")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x37  , _L("/")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x38  , _L("(")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x39  , _L(")")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x30  , _L("=")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDB  , _L("'")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDB  , _L("?")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDD  , _L("\xA1")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDD  , _L("\xBF")										   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBA  , _L("`")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBA  , _L("^")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBA  , _L("[")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xBB  , _L("+")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBB  , _L("*")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBB  , _L("]")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xC0  , _L("\xF1")											 , ALTERNATIVE_KEY_NONE	      },
																														{ 0xC0  , _L("\xD1")										   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDE  , _L("\xB4")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDE  , _L("\xA8")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xDE  , _L("{")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xBF  , _L("\xE7")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBF  , _L("\xC7")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBF  , _L("}")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xE2  , _L("<")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xE2  , _L(">")											   , ALTERNATIVE_KEY_SHIFT      },
																														
																														{ 0xBC  , _L(",")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBC  , _L(";")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBE  , _L(".")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBE  , _L(":")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBD  , _L("-")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBD  , _L("_")											   , ALTERNATIVE_KEY_SHIFT      },																														
																													};



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPSIMULATE::INPSIMULATE()
* @brief      Constructor of class
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPSIMULATE::INPSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         INPSIMULATE::~INPSIMULATE()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
INPSIMULATE::~INPSIMULATE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_Press(XBYTE code)
* @brief      Key press
* @ingroup    INPUT
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_Press(XBYTE code)
{
	return false;
}	


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_UnPress(XBYTE code)
* @brief      Key un press
* @ingroup    INPUT
* 
* @param[in]  code : Code value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_UnPress(XBYTE code)
{
	return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_Click(XBYTE code, int pressuretime)
* @brief      Key click
* @ingroup    INPUT
* 
* @param[in]  code : Code value.
* @param[in]  pressuretime : Pressuretime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_Click(XBYTE code, int pressuretime)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_PressByLiteral(XCHAR* literal)
* @brief      Key press by literal
* @ingroup    INPUT
* 
* @param[in]  literal : Literal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_PressByLiteral(XCHAR* literal)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {
      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(_L("SHIFT"));      break;
        }

      return Key_Press(code);
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_UnPressByLiteral(XCHAR* literal)
* @brief      Key un press by literal
* @ingroup    INPUT
* 
* @param[in]  literal : Literal pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_UnPressByLiteral(XCHAR* literal)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {
      bool status = Key_UnPress(code);

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(_L("SHIFT"));      break;
        }

      return status;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_ClickByLiteral(XCHAR* literal, int pressuretime)
* @brief      Key click by literal
* @ingroup    INPUT
* 
* @param[in]  literal : Literal pointer to use.
* @param[in]  pressuretime : Pressuretime value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_ClickByLiteral(XCHAR* literal, int pressuretime)
{
  ALTERNATIVE_KEY altkey  = ALTERNATIVE_KEY_NONE;
  XBYTE           code    = GetKDBCodeByLiteral(literal, altkey);

  if(code)
    {
      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(_L("SHIFT"));      break;
        }

      bool status = Key_Click(code, pressuretime);

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(_L("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(_L("SHIFT"));      break;
        }

      return status;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Key_ClickByText(XCHAR* text, int pressuretimeinterval)
* @brief      Key click by text
* @ingroup    INPUT
* 
* @param[in]  text : Text to use.
* @param[in]  pressuretimeinterval : Pressuretimeinterval value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Key_ClickByText(XCHAR* text, int pressuretimeinterval)
{
  XSTRING _text;
  XSTRING literal;
  bool    status = true;

  if(!text)
    {
      return false;
    }

  _text = text;

  if(_text.IsEmpty())
    {
      return true;
    }

  for(XDWORD c=0; c<_text.GetSize(); c++)
    {
      bool handled = true;

      literal.Empty();
      literal.Add(_text.Get()[c]);

      switch(_text.Get()[c])
        {
          case _C('A')   :
          case _C('B')   :
          case _C('C')   :
          case _C('D')   :
          case _C('E')   :
          case _C('F')   :
          case _C('G')   :
          case _C('H')   :
          case _C('I')   :
          case _C('J')   :
          case _C('K')   :
          case _C('L')   :
          case _C('M')   :
          case _C('N')   :
          case _C('O')   :
          case _C('P')   :
          case _C('Q')   :
          case _C('R')   :
          case _C('S')   :
          case _C('T')   :
          case _C('U')   :
          case _C('V')   :
          case _C('W')   :
          case _C('X')   :
          case _C('Y')   :
          case _C('Z')   : { bool changecapslock = false;

                              if(!IsCapsLockActive())
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                }
                            }
                            break;

          case _C(' ')   : if(!Key_ClickByLiteral(_L("SPACEBAR"), pressuretimeinterval)) status = false;
                            break;

          case _C('a')   :
          case _C('b')   :
          case _C('c')   :
          case _C('d')   :
          case _C('e')   :
          case _C('f')   :
          case _C('g')   :
          case _C('h')   :
          case _C('i')   :
          case _C('j')   :
          case _C('k')   :
          case _C('l')   :
          case _C('m')   :
          case _C('n')   :
          case _C('o')   :
          case _C('p')   :
          case _C('q')   :
          case _C('r')   :
          case _C('s')   :
          case _C('t')   :
          case _C('u')   :
          case _C('v')   :
          case _C('w')   :
          case _C('x')   :
          case _C('y')   :

          case _C('z')   : { bool changecapslock = false;

                              if(IsCapsLockActive())
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(_L("CAPS LOCK"), pressuretimeinterval)) status = false;
                                }
                            }
                            break;

          case _C('1')   :
          case _C('2')   :
          case _C('3')   :
          case _C('4')   :
          case _C('5')   :
          case _C('6')   :
          case _C('7')   :
          case _C('8')   :
          case _C('9')   :
          case _C('0')   :

          case _C('!')   :
          case _C('@')   :
          case _C('#')   :
          case _C('$')   :
          case _C('%')   :
          case _C('^')   :
          case _C('&')   :
          case _C('*')   :
          case _C('(')   :
          case _C(')')   :
          case _C('_')   :
          case _C('+')   :
          case _C('-')   :
          case _C('=')   :
          case _C('[')   :
          case _C(']')   :
          case _C('{')   :
          case _C('}')   :
          case _C('|')   :
          case _C(';')   :
          case _C(':')   :
          case _C('\'')  :
          case _C(',')   :
          case _C('.')   :
          case _C('<')   :
          case _C('?')   :
          case _C('/')   :
          case _C('\\')  :
          case _C('"')   :

          case 0xBF       :
          case 0xA1       :
          case 0xF1       :
          case 0xD1       :
          case 0xB7       :
                            if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;
                            break;

          default         : handled = false;
                            break;
        }

      if(!handled)
        {
          status = false;
        }
    }

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::IsCapsLockActive()
* @brief      Is caps lock active (platform default: unknown / off)
* @ingroup    INPUT
* 
* @return     bool : true if the condition is met; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::IsCapsLockActive()
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Mouse_SetPos(int x, int y)
* @brief      Mouse set pos
* @ingroup    INPUT
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Mouse_SetPos(int x, int y)
{ 
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool INPSIMULATE::Mouse_Click(int x, int y)
* @brief      Mouse click
* @ingroup    INPUT
* 
* @param[in]  x : X coordinate.
* @param[in]  y : Y coordinate.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool INPSIMULATE::Mouse_Click(int x, int y)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XBYTE INPSIMULATE::GetKDBCodeByLiteral(XCHAR* literal, ALTERNATIVE_KEY& altkey)
* @brief      Get KDB code by literal
* @ingroup    INPUT
* 
* @param[in]  literal : Literal pointer to use.
* @param[in]  altkey : Altkey value.
* 
* @return     XBYTE : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XBYTE INPSIMULATE::GetKDBCodeByLiteral(XCHAR* literal, ALTERNATIVE_KEY& altkey)
{
	XSTRING _literal = literal;

	if(_literal.IsEmpty())
		{
			return 0;
		}

	altkey = ALTERNATIVE_KEY_NONE;

	for(XDWORD c=0; c<(sizeof(inputsimulare_KDB_PC)/sizeof(INPUTSIMULATE_KDB_PC)); c++)
		{
			if(!_literal.Compare(inputsimulare_KDB_PC[c].literal, true))
				{
				  altkey = inputsimulare_KDB_PC[c].altkey;
					return inputsimulare_KDB_PC[c].code;
				}					
		}

	for(XDWORD c=0; c<(sizeof(inputsimulare_KDB_INTEL_Spanish)/sizeof(INPUTSIMULATE_KDB_PC)); c++)
		{
			if(!_literal.Compare(inputsimulare_KDB_INTEL_Spanish[c].literal, true))
				{
					altkey = inputsimulare_KDB_INTEL_Spanish[c].altkey;
					return inputsimulare_KDB_INTEL_Spanish[c].code;
				}					
		}

	return 0;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void INPSIMULATE::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    INPUT
* 
* --------------------------------------------------------------------------------------------------------------------*/
void INPSIMULATE::Clean()
{

}




