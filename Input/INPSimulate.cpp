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

INPUTSIMULATE_KDB_PC  inputsimulare_KDB_PC[]						= { { 0x01	, NL("Left mouse")						     , ALTERNATIVE_KEY_NONE			  },					
																														{ 0x02	, NL("Right mouse")					     , ALTERNATIVE_KEY_NONE			  },					                                                           
			                                                      { 0x03	, NL("Control-break")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x04	, NL("Middle mouse")					     , ALTERNATIVE_KEY_NONE			  },					 
			                                                      { 0x05	, NL("X1 mouse")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x06	, NL("X2 mouse")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x08	, NL("BACKSPACE")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x09	, NL("TAB")							         , ALTERNATIVE_KEY_NONE			  },																															
			                                                      { 0x0C	, NL("CLEAR")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x0D	, NL("ENTER")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x10	, NL("SHIFT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x11	, NL("CTRL")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x12	, NL("ALT")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x13	, NL("PAUSE")						         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x14	, NL("CAPS LOCK")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, NL("IME Kana mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, NL("IME Hanguel mode")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x15	, NL("IME Hangul mode")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x16	, NL("IME On")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x17	, NL("IME Junja mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x18	, NL("IME final mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x19	, NL("IME Hanja mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x19	, NL("IME Kanji mode")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1A	, NL("IME Off")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1B	, NL("ESC")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1C	, NL("IME convert")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1D	, NL("IME nonconvert")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1E	, NL("IME accept")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x1F	, NL("IME mode change request")	 , ALTERNATIVE_KEY_NONE			 	},					
			                                                      { 0x20	, NL("SPACEBAR")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x21	, NL("PAGE UP")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x22	, NL("PAGE DOWN")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x23	, NL("END")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x24	, NL("HOME")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x25	, NL("LEFT ARROW")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x26	, NL("UP ARROW")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x27	, NL("RIGHT ARROW")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x28	, NL("DOWN ARROW")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x29	, NL("SELECT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2A	, NL("PRINT")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2B	, NL("EXECUTE")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2C	, NL("PRINT SCREEN")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2D	, NL("INS")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2E	, NL("DEL")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x2F	, NL("HELP")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x30	, NL("0")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x31	, NL("1")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x32	, NL("2")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x33	, NL("3")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x34	, NL("4")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x35	, NL("5")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x36	, NL("6")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x37	, NL("7")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x38	, NL("8")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x39	, NL("9")								         , ALTERNATIVE_KEY_NONE			  },																																	                                                      				
			                                                      { 0x41	, NL("A")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x42	, NL("B")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x43	, NL("C")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x44	, NL("D")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x45	, NL("E")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x46	, NL("F")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x47	, NL("G")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x48	, NL("H")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x49	, NL("I")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4A	, NL("J")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4B	, NL("K")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4C	, NL("L")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4D	, NL("M")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4E	, NL("N")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x4F	, NL("O")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x50	, NL("P")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x51	, NL("Q")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x52	, NL("R")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x53	, NL("S")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x54	, NL("T")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x55	, NL("U")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x56	, NL("V")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x57	, NL("W")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x58	, NL("X")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x59	, NL("Y")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5A	, NL("Z")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5B	, NL("Left Windows")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5C	, NL("Right Windows")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x5D	, NL("Applications")					     , ALTERNATIVE_KEY_NONE			  },																																		                                                                          
			                                                      { 0x5F	, NL("Computer Sleep")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x61	, NL("Numericpad 1")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x60	, NL("Numericpad 0")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x62	, NL("Numericpad 2")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x63	, NL("Numericpad 3")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x64	, NL("Numericpad 4")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x65	, NL("Numericpad 5")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x66	, NL("Numericpad 6")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x67	, NL("Numericpad 7")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x68	, NL("Numericpad 8")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x69	, NL("Numericpad 9")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6A	, NL("Multiply")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6B	, NL("Add")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6C	, NL("Separator")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6D	, NL("Subtract")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6E	, NL("Decimal")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x6F	, NL("Divide")							       , ALTERNATIVE_KEY_NONE			  },	
																														{ 0x70	, NL("F1")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x71	, NL("F2")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x72	, NL("F3")								         , ALTERNATIVE_KEY_NONE			  },								                                                      
			                                                      { 0x73	, NL("F4")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x74	, NL("F5")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x75	, NL("F6")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x76	, NL("F7")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x77	, NL("F8")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x78	, NL("F9")								         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x79	, NL("F10")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7A	, NL("F11")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7B	, NL("F12")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7C	, NL("F13")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7D	, NL("F14")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7E	, NL("F15")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x7F	, NL("F16")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x80	, NL("F17")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x81	, NL("F18")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x82	, NL("F19")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x83	, NL("F20")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x84	, NL("F21")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x85	, NL("F22")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x86	, NL("F23")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x87	, NL("F24")							         , ALTERNATIVE_KEY_NONE			  },																												                                                                          
			                                                      { 0x90	, NL("NUM LOCK")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0x91	, NL("SCROLL LOCK")					     , ALTERNATIVE_KEY_NONE			  },					                                                                          
			                                                      { 0xA0	, NL("Left SHIFT")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA1	, NL("Right SHIFT")				 	     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA2	, NL("Left CONTROL")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA3	, NL("Right CONTROL")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA4	, NL("Left ALT")						       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA5	, NL("Right ALT")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA6	, NL("Browser Back")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA7	, NL("Browser Forward")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA8	, NL("Browser Refresh")				   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xA9	, NL("Browser Stop")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAA	, NL("Browser Search")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAB	, NL("Browser Favorites")				 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAC	, NL("Browser Home")		      		 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAD	, NL("Volume Mute")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAE	, NL("Volume Down")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xAF	, NL("Volume Up")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB0	, NL("Next Track")					       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB1	, NL("Previous Track")					   , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB2	, NL("Stop Media")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB3	, NL("Play/Pause Media")					 , ALTERNATIVE_KEY_NONE			  },
			                                                      { 0xB4	, NL("Start Mail")						     , ALTERNATIVE_KEY_NONE			  },					
 			                                                      { 0xB5	, NL("Select Media")					     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB6	, NL("Start Application 1")			 , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xB7	, NL("Start Application 2")			 , ALTERNATIVE_KEY_NONE			  },					                                                                          			                                                     			                                                       		
			                                                      { 0xE5	, NL("IME PROCESS")						   , ALTERNATIVE_KEY_NONE			  },					                                                                   
			                                                      { 0xF6	, NL("Attn")							         , ALTERNATIVE_KEY_NONE			  },						
			                                                      { 0xF7	, NL("CrSel")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xF8	, NL("ExSel")							       , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xF9	, NL("Erase EOF")						     , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFA	, NL("Play")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFB	, NL("Zoom")							         , ALTERNATIVE_KEY_NONE			  },	  				
			                                                      { 0xFD	, NL("PA1")							         , ALTERNATIVE_KEY_NONE			  },					
			                                                      { 0xFE	, NL("Clear")							       , ALTERNATIVE_KEY_NONE			  }
                                                          };	


INPUTSIMULATE_KDB_PC  inputsimulare_KDB_INTEL_Spanish[]		= { { 0xDC  , NL("\xBA")								  		 , ALTERNATIVE_KEY_NONE       },
																														{ 0xDC  , NL("\xAA")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xDC  , NL("\\")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x31  , NL("!")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x31  , NL("|")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x32  , NL("\"")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x32  , NL("@")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x33  , NL("\x95")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x33  , NL("#")											   , ALTERNATIVE_KEY_ALTGR      },
																														{ 0x34  , NL("$")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x35  , NL("%")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x36  , NL("&")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x37  , NL("/")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x38  , NL("(")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x39  , NL(")")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0x30  , NL("=")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDB  , NL("'")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDB  , NL("?")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDD  , NL("\xA1")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDD  , NL("\xBF")										   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBA  , NL("`")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBA  , NL("^")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBA  , NL("[")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xBB  , NL("+")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBB  , NL("*")											   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBB  , NL("]")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xC0  , NL("\xF1")											 , ALTERNATIVE_KEY_NONE	      },
																														{ 0xC0  , NL("\xD1")										   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xDE  , NL("\xB4")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xDE  , NL("\xA8")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xDE  , NL("{")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xBF  , NL("\xE7")										   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBF  , NL("\xC7")										   , ALTERNATIVE_KEY_SHIFT      },
																														{ 0xBF  , NL("}")											   , ALTERNATIVE_KEY_ALTGR      },

																														{ 0xE2  , NL("<")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xE2  , NL(">")											   , ALTERNATIVE_KEY_SHIFT      },
																														
																														{ 0xBC  , NL(",")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBC  , NL(";")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBE  , NL(".")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBE  , NL(":")											   , ALTERNATIVE_KEY_SHIFT      },

																														{ 0xBD  , NL("-")											   , ALTERNATIVE_KEY_NONE	      },
																														{ 0xBD  , NL("_")											   , ALTERNATIVE_KEY_SHIFT      },																														
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
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(NL("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(NL("SHIFT"));      break;
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
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(NL("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(NL("SHIFT"));      break;
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
          case ALTERNATIVE_KEY_ALTGR  : Key_PressByLiteral(NL("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_PressByLiteral(NL("SHIFT"));      break;
        }

      bool status = Key_Click(code, pressuretime);

      switch(altkey)
        {
          case ALTERNATIVE_KEY_NONE   : break;
          case ALTERNATIVE_KEY_ALTGR  : Key_UnPressByLiteral(NL("Right ALT"));  break;
          case ALTERNATIVE_KEY_SHIFT  : Key_UnPressByLiteral(NL("SHIFT"));      break;
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
                                  if(!Key_ClickByLiteral(NL("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(NL("CAPS LOCK"), pressuretimeinterval)) status = false;
                                }
                            }
                            break;

          case _C(' ')   : if(!Key_ClickByLiteral(NL("SPACEBAR"), pressuretimeinterval)) status = false;
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
                                  if(!Key_ClickByLiteral(NL("CAPS LOCK"), pressuretimeinterval)) status = false;
                                  changecapslock = true;
                                }

                              if(!Key_ClickByLiteral(literal.Get(), pressuretimeinterval)) status = false;

                              if(changecapslock)
                                {
                                  if(!Key_ClickByLiteral(NL("CAPS LOCK"), pressuretimeinterval)) status = false;
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




