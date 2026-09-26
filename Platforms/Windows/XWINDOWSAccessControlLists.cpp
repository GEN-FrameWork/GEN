/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       XWINDOWSAccessControlLists.cpp
* 
* @class      XWINDOWSACCESSCONTROLLISTS
* @brief      WINDOWS eXtended Utils Access Control Lists class
* @ingroup    PLATFORM_WINDOWS
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

#pragma comment(lib, "advapi32.lib")

#include "XWINDOWSAccessControlLists.h"

#include <windows.h>
#include <stdio.h>
#include <aclapi.h>
#include <tchar.h>
#include <accctrl.h>



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"




/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/



/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSACCESSCONTROLLISTS::XWINDOWSACCESSCONTROLLISTS()
* @brief      Constructor of class
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSACCESSCONTROLLISTS::XWINDOWSACCESSCONTROLLISTS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XWINDOWSACCESSCONTROLLISTS::~XWINDOWSACCESSCONTROLLISTS()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
XWINDOWSACCESSCONTROLLISTS::~XWINDOWSACCESSCONTROLLISTS()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForAdmin(XCHAR* filename)
* @brief      Set file only permission for admin
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  filename : File name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForAdmin(XCHAR* filename)
{
  #define EXPLICIT_ACCESS_MAXNUM_ADMIN  2

  PSID                      everyoneSID   = NULL;
  PSID                      adminSID      = NULL;
  PACL                      ACL           = NULL;
  EXPLICIT_ACCESS           ea[EXPLICIT_ACCESS_MAXNUM_ADMIN];
  SID_IDENTIFIER_AUTHORITY  SIDauthworld  = SECURITY_WORLD_SID_AUTHORITY;
  SID_IDENTIFIER_AUTHORITY  SIDAuthNT     = SECURITY_NT_AUTHORITY;

    // Create a well-known SID for the Everyone group.
  AllocateAndInitializeSid(&SIDauthworld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyoneSID);


  AllocateAndInitializeSid(&SIDAuthNT, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminSID);


  // Initialize an EXPLICIT_ACCESS structure for an ACE.
  ZeroMemory(&ea, EXPLICIT_ACCESS_MAXNUM_ADMIN * sizeof(EXPLICIT_ACCESS));

  // Set read access for Everyone.
  ea[0].grfAccessPermissions    = GENERIC_READ;
  ea[0].grfAccessMode           = SET_ACCESS;
  ea[0].grfInheritance          = NO_INHERITANCE;
  ea[0].Trustee.TrusteeForm     = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType     = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName       = (LPTSTR) everyoneSID;

  // Set full control for Administrators.
  ea[1].grfAccessPermissions    = GENERIC_ALL;
  ea[1].grfAccessMode           = SET_ACCESS;
  ea[1].grfInheritance          = NO_INHERITANCE;
  ea[1].Trustee.TrusteeForm     = TRUSTEE_IS_SID;
  ea[1].Trustee.TrusteeType     = TRUSTEE_IS_GROUP;
  ea[1].Trustee.ptstrName       = (LPTSTR) adminSID;

  // Create a GEN_NEW ACL that contains the GEN_NEW ACEs.
  SetEntriesInAcl(EXPLICIT_ACCESS_MAXNUM_ADMIN, ea, NULL, &ACL);

  // Initialize a security descriptor.
  PSECURITY_DESCRIPTOR SD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

  InitializeSecurityDescriptor(SD, SECURITY_DESCRIPTOR_REVISION);

  // Add the ACL to the security descriptor.
  SetSecurityDescriptorDacl(SD,
                            TRUE,      // bDaclPresent flag
                            ACL,
                            FALSE);    // not a default DACL


  // Change the security attributes
  BOOL status = SetFileSecurity(filename, DACL_SECURITY_INFORMATION, SD);

  if(everyoneSID) FreeSid(everyoneSID);
  if(adminSID)    FreeSid(adminSID);
  if(ACL)         LocalFree(ACL);
  if(SD)          LocalFree(SD);

  return status?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForSystemUser(XCHAR* filename)
* @brief      Set file only permission for system user
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  filename : File name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForSystemUser(XCHAR* filename)
{
  #define EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER  1

  PSID                      SID           = NULL;
  PACL                      ACL           = NULL;
  EXPLICIT_ACCESS           ea[EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER];
  SID_IDENTIFIER_AUTHORITY  SIDsystem     = SECURITY_NT_AUTHORITY;

  
  AllocateAndInitializeSid(&SIDsystem, 1, SECURITY_LOCAL_SYSTEM_RID, 0, 0, 0, 0, 0, 0, 0, &SID);
    
  // Initialize an EXPLICIT_ACCESS structure for an ACE.
  ZeroMemory(&ea, EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER * sizeof(EXPLICIT_ACCESS));

  // Set full control for Administrators.
  ea[0].grfAccessPermissions    = GENERIC_ALL;
  ea[0].grfAccessMode           = SET_ACCESS;
  ea[0].grfInheritance          = NO_INHERITANCE;
  ea[0].Trustee.TrusteeForm     = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType     = TRUSTEE_IS_GROUP;
  ea[0].Trustee.ptstrName       = (LPTSTR)SID;

  // Create a GEN_NEW ACL that contains the GEN_NEW ACEs.
  SetEntriesInAcl(EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER, ea, NULL, &ACL);

  // Initialize a security descriptor.
  PSECURITY_DESCRIPTOR SD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

  InitializeSecurityDescriptor(SD, SECURITY_DESCRIPTOR_REVISION);

  // Add the ACL to the security descriptor.
  SetSecurityDescriptorDacl(SD,
                            TRUE,      // bDaclPresent flag
                            ACL,
                            FALSE);    // not a default DACL


  // Change the security attributes
  BOOL status = SetFileSecurity(filename, DACL_SECURITY_INFORMATION, SD);

  if(SID)         FreeSid(SID);
  if(ACL)         LocalFree(ACL);
  if(SD)          LocalFree(SD);

  return status?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForAdminUser(XCHAR* filename)
* @brief      set file only permission for admin user
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  filename : File name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSACCESSCONTROLLISTS::SetFileOnlyPermissionForAdminUser(XCHAR* filename)
{
  #define EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER  1

  PSID                      SID           = NULL;
  PACL                      ACL           = NULL;
  EXPLICIT_ACCESS           ea[EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER];
  SID_IDENTIFIER_AUTHORITY  SIDsystem     = SECURITY_NT_AUTHORITY;

  AllocateAndInitializeSid(&SIDsystem,
                           2,                         // 2 sub-authorities
                           SECURITY_BUILTIN_DOMAIN_RID,
                           DOMAIN_ALIAS_RID_ADMINS,
                           0, 0, 0, 0, 0, 0,
                           &SID);

  // Initialize an EXPLICIT_ACCESS structure for an ACE.
  ZeroMemory(&ea, EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER * sizeof(EXPLICIT_ACCESS));

  // Set full control for Administrators.
  ea[0].grfAccessPermissions    = GENERIC_ALL;
  ea[0].grfAccessMode           = SET_ACCESS;
  ea[0].grfInheritance          = NO_INHERITANCE;
  ea[0].Trustee.TrusteeForm     = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType     = TRUSTEE_IS_GROUP;
  ea[0].Trustee.ptstrName       = (LPTSTR)SID;

  // Create a GEN_NEW ACL that contains the GEN_NEW ACEs.
  SetEntriesInAcl(EXPLICIT_ACCESS_MAXNUM_SYSTEMUSER, ea, NULL, &ACL);

  // Initialize a security descriptor.
  PSECURITY_DESCRIPTOR SD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

  InitializeSecurityDescriptor(SD, SECURITY_DESCRIPTOR_REVISION);

  // Add the ACL to the security descriptor.
  SetSecurityDescriptorDacl(SD,
                            TRUE,      // bDaclPresent flag
                            ACL,
                            FALSE);    // not a default DACL


  // Change the security attributes
  BOOL status = SetFileSecurity(filename, DACL_SECURITY_INFORMATION, SD);

  if(SID)         FreeSid(SID);
  if(ACL)         LocalFree(ACL);
  if(SD)          LocalFree(SD);

  return status?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool XWINDOWSACCESSCONTROLLISTS::SetFilePermissionForEveryone(XCHAR* filename)
* @brief      Set file permission for everyone
* @ingroup    PLATFORM_WINDOWS
* 
* @param[in]  filename : File name to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool XWINDOWSACCESSCONTROLLISTS::SetFilePermissionForEveryone(XCHAR* filename)
{
  PSID                     everyoneSID = NULL;
  PACL                     ACL         = NULL;
  EXPLICIT_ACCESS          ea[1];
  SID_IDENTIFIER_AUTHORITY SIDauthworld = SECURITY_WORLD_SID_AUTHORITY;

  // Create a well-known SID for the Everyone group.
  AllocateAndInitializeSid(&SIDauthworld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &everyoneSID);

  // Initialize an EXPLICIT_ACCESS structure for an ACE.
  ZeroMemory(&ea, 1 * sizeof(EXPLICIT_ACCESS));

  ea[0].grfAccessPermissions      = GENERIC_ALL;
  ea[0].grfAccessMode             = SET_ACCESS;
  ea[0].grfInheritance            = SUB_CONTAINERS_AND_OBJECTS_INHERIT; //NO_INHERITANCE;
  ea[0].Trustee.TrusteeForm       = TRUSTEE_IS_SID;
  ea[0].Trustee.TrusteeType       = TRUSTEE_IS_WELL_KNOWN_GROUP;
  ea[0].Trustee.ptstrName         = (LPTSTR) everyoneSID;

  // Create a GEN_NEW ACL that contains the GEN_NEW ACEs.
  SetEntriesInAcl(1, ea, NULL, &ACL);

  // Initialize a security descriptor.
  PSECURITY_DESCRIPTOR SD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

  InitializeSecurityDescriptor(SD, SECURITY_DESCRIPTOR_REVISION);

  // Add the ACL to the security descriptor.
  SetSecurityDescriptorDacl(SD,
                            TRUE,      // bDaclPresent flag
                            ACL,
                            FALSE);    // not a default DACL

  //Change the security attributes
  BOOL status = SetFileSecurity(filename, DACL_SECURITY_INFORMATION, SD);

  if(everyoneSID)  FreeSid(everyoneSID);
  if(ACL)          LocalFree(ACL);
  if(SD)           LocalFree(SD);

  return status?true:false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void XWINDOWSACCESSCONTROLLISTS::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    PLATFORM_WINDOWS
* 
* --------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSACCESSCONTROLLISTS::Clean()
{

}



