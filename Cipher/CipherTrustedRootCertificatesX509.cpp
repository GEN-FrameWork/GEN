/**-------------------------------------------------------------------------------------------------------------------
* 
* @file       CipherTrustedRootCertificatesX509.cpp
* 
* @class      CIPHERTRUSTEDROOTCERTIFICATES
* @brief      Cipher Trusted Root Certificates X.509 class
* @ingroup    CIPHER
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

#include "CipherTrustedRootCertificatesX509.h"

#include "XFileTXT.h"

#include "CipherCertificateX509.h"
#include "CipherKey.h"
#include "CipherCredentialsLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(LINUX) || defined(ANDROID)
#include <dirent.h>
#include <sys/stat.h>
#endif



/*---- PRECOMPILATION INCLUDES ---------------------------------------------------------------------------------------*/

#include "GEN_Control.h"



/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

#include "cacert.h"
//XCHAR*  trustedrootcertificatesX509[] = { __L("")  };
                       


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERTRUSTPROVIDERX509::CIPHERTRUSTPROVIDERX509()
* @brief      Constructor of class
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTPROVIDERX509::CIPHERTRUSTPROVIDERX509()
{
  maximumroots           = CIPHERTRUSTPROVIDERX509_DEFAULT_MAXROOTS;
  maximumcertificatesize = CIPHERTRUSTPROVIDERX509_DEFAULT_MAXCERTIFICATESIZE;
  maximumtotalsize       = CIPHERTRUSTPROVIDERX509_DEFAULT_MAXTOTALSIZE;
  totalsize              = 0;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERTRUSTPROVIDERX509::~CIPHERTRUSTPROVIDERX509()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTPROVIDERX509::~CIPHERTRUSTPROVIDERX509()
{
  Roots_Delete();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XBUFFER*>* CIPHERTRUSTPROVIDERX509::GetRoots()
* @brief      Get the trust anchors loaded by the provider
* @ingroup    CIPHER
*
* @return     XVECTOR<XBUFFER*>* : Pointer to the loaded trust anchors.
*
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* CIPHERTRUSTPROVIDERX509::GetRoots()
{
  return &roots;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERTRUSTPROVIDERX509::Root_Add(XBUFFER& root)
* @brief      Add a copied DER trust anchor to the provider
* @ingroup    CIPHER
*
* @param[in]  root : DER certificate to copy.
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509::Root_Add(XBUFFER& root)
{
  XBUFFER* copy;

  if(root.IsEmpty() || root.GetSize() > maximumcertificatesize || roots.GetSize() >= maximumroots ||
     totalsize > (maximumtotalsize - root.GetSize())) return false;

  for(XDWORD c=0; c<roots.GetSize(); c++)
    {
      XBUFFER* existing = roots.Get(c);
      if(existing && existing->GetSize() == root.GetSize() &&
         !memcmp(existing->Get(), root.Get(), root.GetSize())) return true;
    }

  copy = GEN_NEW XBUFFER();
  if(!copy) return false;

  if(!copy->Add(root) || !roots.Add(copy))
    {
      GEN_DELETE copy;
      return false;
    }

  totalsize += root.GetSize();
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTPROVIDERX509::Root_Remove(XBUFFER& root)
* @brief      Root remove
* @ingroup    CIPHER
* 
* @param[in]  root : Root value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509::Root_Remove(XBUFFER& root)
{
  if(root.IsEmpty()) return false;
  for(XDWORD c=0; c<roots.GetSize(); c++)
    {
      XBUFFER* existing = roots.Get(c);
      if(existing && existing->GetSize() == root.GetSize() && !memcmp(existing->Get(), root.Get(), root.GetSize()))
        {
          totalsize -= existing->GetSize();
          roots.Delete(existing);
          GEN_DELETE existing;
          return true;
        }
    }
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTPROVIDERX509::SetLimits(XDWORD maximumroots, XDWORD maximumcertificatesize, XDWORD maximumtotalsize)
* @brief      Set limits
* @ingroup    CIPHER
* 
* @param[in]  maximumroots : Maximumroots value.
* @param[in]  maximumcertificatesize : Maximumcertificatesize value.
* @param[in]  maximumtotalsize : Maximumtotalsize value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509::SetLimits(XDWORD maximumroots, XDWORD maximumcertificatesize, XDWORD maximumtotalsize)
{
  if(!maximumroots || !maximumcertificatesize || !maximumtotalsize || !roots.IsEmpty()) return false;
  if(maximumcertificatesize > maximumtotalsize) return false;
  this->maximumroots           = maximumroots;
  this->maximumcertificatesize = maximumcertificatesize;
  this->maximumtotalsize       = maximumtotalsize;
  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XDWORD CIPHERTRUSTPROVIDERX509::GetTotalSize()
* @brief      Get total size
* @ingroup    CIPHER
* 
* @return     XDWORD : Requested value.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XDWORD CIPHERTRUSTPROVIDERX509::GetTotalSize()
{
  return totalsize;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERTRUSTPROVIDERX509::Roots_Delete()
* @brief      Delete all trust anchors loaded by the provider
* @ingroup    CIPHER
*
* @return     bool : true if the operation is successful; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509::Roots_Delete()
{
  roots.DeleteContents();
  roots.DeleteAll();
  totalsize = 0;

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERTRUSTPROVIDERX509GEN::CIPHERTRUSTPROVIDERX509GEN()
* @brief      Constructor of class
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTPROVIDERX509GEN::CIPHERTRUSTPROVIDERX509GEN()
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         CIPHERTRUSTPROVIDERX509GEN::~CIPHERTRUSTPROVIDERX509GEN()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
*
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTPROVIDERX509GEN::~CIPHERTRUSTPROVIDERX509GEN()
{
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool CIPHERTRUSTPROVIDERX509GEN::Load()
* @brief      Load the supported trust anchors from the embedded GEN CA bundle
* @ingroup    CIPHER
*
* @return     bool : true if at least one trust anchor is loaded; otherwise false.
*
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509GEN::Load()
{
  CIPHERTRUSTEDROOTCERTIFICATESX509 defaultroots;

  Roots_Delete();

  if(!defaultroots.Certificates_Decode()) return false;

  for(XDWORD c=0; c<defaultroots.Certificates_GetAll()->GetSize(); c++)
    {
      XBUFFER* rootDER = defaultroots.Certificates_GetAll()->Get(c);

      if(rootDER)
        {
          CIPHERCERTIFICATEX509 root;

          if(root.Decode((*rootDER)) && root.IsCertificateAuthority() && root.GetPublicCipherKey() &&
             ((root.GetPublicCipherKey()->GetType() == CIPHERKEYTYPE_RSA_PUBLIC) ||
              (root.GetPublicCipherKey()->GetType() == CIPHERKEYTYPE_ECDSA_SECP256R1_PUBLIC) ||
              (root.GetPublicCipherKey()->GetType() == CIPHERKEYTYPE_ECDSA_SECP384R1_PUBLIC) ||
              (root.GetPublicCipherKey()->GetType() == CIPHERKEYTYPE_ECDSA_SECP521R1_PUBLIC)
              #ifdef CIPHER_ASYMMETRIC_ED25519_ACTIVE
              || (root.GetPublicCipherKey()->GetType() == CIPHERKEYTYPE_ED25519_PUBLIC)
              #endif
             ))
            {
              if(!Root_Add((*rootDER)))
                {
                  Roots_Delete();
                  return false;
                }
            }
        }
    }

  return !GetRoots()->IsEmpty();
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERTRUSTPROVIDERX509_LoadFile(CIPHERTRUSTPROVIDERX509* provider, const char* path, bool remove = false)
* @brief      Load file
* @ingroup    CIPHER
* 
* @param[in]  provider : Pointer to provider.
* @param[in]  path : Pointer to path.
* @param[in]  remove : Remove value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERTRUSTPROVIDERX509_LoadFile(CIPHERTRUSTPROVIDERX509* provider, const char* path, bool remove = false)
{
  if(!provider || !path) return false;
  FILE* file = fopen(path, "rb");
  if(!file) return false;
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  XBUFFER data;
  bool status = size > 0 && size <= (16*1024*1024) && data.Resize((XDWORD)size) &&
                (fread(data.Get(), 1, (size_t)size, file) == (size_t)size);
  fclose(file);
  if(!status) return false;

  XVECTOR<XBUFFER*> certificates;
  if(!CIPHERCREDENTIALSLOADER::Certificates_Load(data, certificates)) return false;
  for(XDWORD c=0; c<certificates.GetSize(); c++)
    {
      XBUFFER* certificate = certificates.Get(c);
      if(certificate && !(remove?provider->Root_Remove((*certificate)):provider->Root_Add((*certificate)))) status = false;
    }
  CIPHERCREDENTIALSLOADER::Certificates_Delete(certificates);
  return status;
}

#if defined(LINUX) || defined(ANDROID)
/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERTRUSTPROVIDERX509_LoadDirectory(CIPHERTRUSTPROVIDERX509* provider, const char* directorypath, bool remove = false)
* @brief      Load directory
* @ingroup    CIPHER
* 
* @param[in]  provider : Pointer to provider.
* @param[in]  directorypath : Pointer to directorypath.
* @param[in]  remove : Remove value.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERTRUSTPROVIDERX509_LoadDirectory(CIPHERTRUSTPROVIDERX509* provider, const char* directorypath, bool remove = false)
{
  if(!provider || !directorypath || !directorypath[0]) return false;
  DIR* directory = opendir(directorypath);
  if(!directory) return false;

  bool loaded = false;
  struct dirent* entry;
  while((entry = readdir(directory)) != NULL)
    {
      if(entry->d_name[0] == '.') continue;
      char path[1024];
      int length = snprintf(path, sizeof(path), "%s/%s", directorypath, entry->d_name);
      if(length <= 0 || length >= (int)sizeof(path)) continue;
      struct stat information;
      if(stat(path, &information) || !S_ISREG(information.st_mode) || information.st_size <= 0 ||
         information.st_size > (16*1024*1024)) continue;
      if(CIPHERTRUSTPROVIDERX509_LoadFile(provider, path, remove)) loaded = true;
    }

  closedir(directory);
  return loaded;
}

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         static bool CIPHERTRUSTPROVIDERX509_LoadDirectoryList(CIPHERTRUSTPROVIDERX509* provider, const char* paths)
* @brief      Load directory list
* @ingroup    CIPHER
* 
* @param[in]  provider : Pointer to provider.
* @param[in]  paths : Pointer to paths.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
static bool CIPHERTRUSTPROVIDERX509_LoadDirectoryList(CIPHERTRUSTPROVIDERX509* provider, const char* paths)
{
  if(!provider || !paths || !paths[0]) return false;
  bool loaded = false;
  const char* start = paths;
  while(*start)
    {
      const char* end = start;
      while(*end && *end != ':') end++;
      if(end > start && (end-start) < 1024)
        {
          char path[1024];
          memcpy(path, start, (size_t)(end-start));
          path[end-start] = 0;
          if(CIPHERTRUSTPROVIDERX509_LoadDirectory(provider, path)) loaded = true;
        }
      start = *end?(end+1):end;
    }
  return loaded;
}
#endif

/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTPROVIDERX509LINUX::Load()
* @brief      Load
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509LINUX::Load()
{
  Roots_Delete();
#if defined(LINUX) && !defined(ANDROID)
  const char* certificatefile = getenv("SSL_CERT_FILE");
  const char* certificatedirs = getenv("SSL_CERT_DIR");
  bool environmentconfigured = (certificatefile && certificatefile[0]) || (certificatedirs && certificatedirs[0]);

  if(certificatefile && certificatefile[0]) CIPHERTRUSTPROVIDERX509_LoadFile(this, certificatefile);
  if(certificatedirs && certificatedirs[0]) CIPHERTRUSTPROVIDERX509_LoadDirectoryList(this, certificatedirs);

  if(!environmentconfigured)
    {
      static const char* files[] = { "/etc/ssl/certs/ca-certificates.crt", "/etc/pki/tls/certs/ca-bundle.crt",
                                     "/etc/ssl/ca-bundle.pem", "/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem" };
      static const char* directories[] = { "/etc/ssl/certs", "/etc/pki/tls/certs", "/etc/pki/ca-trust/source/anchors" };
      for(XDWORD c=0; c<(sizeof(files)/sizeof(files[0])); c++) CIPHERTRUSTPROVIDERX509_LoadFile(this, files[c]);
      for(XDWORD c=0; c<(sizeof(directories)/sizeof(directories[0])); c++) CIPHERTRUSTPROVIDERX509_LoadDirectory(this, directories[c]);
    }

  return !GetRoots()->IsEmpty();
#endif
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTPROVIDERX509ANDROID::Load()
* @brief      Load
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTPROVIDERX509ANDROID::Load()
{
  Roots_Delete();
#if defined(ANDROID)
  // Android 14+ selects the updatable Conscrypt APEX store when populated;
  // older releases use ANDROID_ROOT.  Do not merge both system stores.
  if(!CIPHERTRUSTPROVIDERX509_LoadDirectory(this, "/apex/com.android.conscrypt/cacerts"))
    {
      const char* androidroot = getenv("ANDROID_ROOT");
      char path[1024];
      if(!androidroot || !androidroot[0]) androidroot = "/system";
      int length = snprintf(path, sizeof(path), "%s/etc/security/cacerts", androidroot);
      if(length > 0 && length < (int)sizeof(path)) CIPHERTRUSTPROVIDERX509_LoadDirectory(this, path);
    }

  // Apply the Android user/system removal overlay before adding user and
  // device-policy managed anchors. Access is subject to the application sandbox.
  const char* androiddata = getenv("ANDROID_DATA");
  if(!androiddata || !androiddata[0]) androiddata = "/data";
  char removedpath[1024];
  int removedlength = snprintf(removedpath, sizeof(removedpath), "%s/misc/keychain/cacerts-removed", androiddata);
  if(removedlength > 0 && removedlength < (int)sizeof(removedpath))
    {
      CIPHERTRUSTPROVIDERX509_LoadDirectory(this, removedpath, true);
    }

  char managedpath[1024];
  int managedlength = snprintf(managedpath, sizeof(managedpath), "%s/misc/keychain/cacerts-added", androiddata);
  if(managedlength > 0 && managedlength < (int)sizeof(managedpath))
    {
      CIPHERTRUSTPROVIDERX509_LoadDirectory(this, managedpath);
    }

  return !GetRoots()->IsEmpty();
#else
  return false;
#endif
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERTRUSTEDROOTCERTIFICATESX509::CIPHERTRUSTEDROOTCERTIFICATESX509()
* @brief      Constructor of class
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTEDROOTCERTIFICATESX509::CIPHERTRUSTEDROOTCERTIFICATESX509()
{
  Clean();

  for(XDWORD c=0; c<(sizeof(trustedrootcertificates)/sizeof(XCHAR*)); c++)
    {
      XSTRING* line = GEN_NEW XSTRING();
      if(line)
        {
          line->Set(trustedrootcertificates[c]);
          lines.Add(line);  
        }
    }  
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         CIPHERTRUSTEDROOTCERTIFICATESX509::~CIPHERTRUSTEDROOTCERTIFICATESX509()
* @brief      Destructor of class
* @note       VIRTUAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
CIPHERTRUSTEDROOTCERTIFICATESX509::~CIPHERTRUSTEDROOTCERTIFICATESX509()
{
  Certificates_DeleteAll();
  DeleteAllLines();

  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTEDROOTCERTIFICATESX509::ReadFromFile(XPATH* pathnamefile)
* @brief      read from file
* @ingroup    CIPHER
* 
* @param[in]  pathnamefile : Pathnamefile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTEDROOTCERTIFICATESX509::ReadFromFile(XPATH* pathnamefile)
{
  XFILETXT* xfiletxt;
  bool      status = false;  

  if(!pathnamefile)
    {
      return false;
    }

  Certificates_DeleteAll();
    
  xfiletxt = GEN_NEW XFILETXT();
  if(!xfiletxt) 
    {
      return false;
    }

  status = xfiletxt->Open((*pathnamefile), true);
  if(status)   
    { 
      status = xfiletxt->ReadAllFile();
      if(status)
        {      
          DeleteAllLines(); 
    
          for(XDWORD c=0; c<xfiletxt->GetNLines(); c++)
            {
              XSTRING* line = GEN_NEW XSTRING();
              if(line)
                {
                  line->Set(xfiletxt->GetLine(c)->Get()); 
                  lines.Add(line);                  
                }
            }                                      
        }

      xfiletxt->Close();   
    }

  GEN_DELETE xfiletxt;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTEDROOTCERTIFICATESX509::GenerateEmbeddedHeadere(XPATH* originpathnamefile, XPATH* targetpathnamefile)
* @brief      generate embedded headere
* @ingroup    CIPHER
* 
* @param[in]  originpathnamefile : Originpathnamefile pointer to use.
* @param[in]  targetpathnamefile : Targetpathnamefile pointer to use.
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTEDROOTCERTIFICATESX509::GenerateEmbeddedHeadere(XPATH* originpathnamefile, XPATH* targetpathnamefile)
{
  XFILETXT* originxfiletxt;
  XFILETXT* targetxfiletxt;
  bool      status = false;  

  if(!originpathnamefile)
    {
      return false;
    }

  if(!targetpathnamefile)
    {
      return false;
    }
    
  originxfiletxt = GEN_NEW XFILETXT();
  if(!originxfiletxt) 
    {
      return false;
    }

  status = originxfiletxt->Open((*originpathnamefile), true);
  if(status)   
    { 
      status = originxfiletxt->ReadAllFile();
      if(status)
        {              
          targetxfiletxt = GEN_NEW XFILETXT();
          if(targetxfiletxt) 
            {
              status = targetxfiletxt->Create((*targetpathnamefile));
              if(status)   
                { 
                  XSTRING line;     

                  for(XDWORD c=0; c<originxfiletxt->GetNLines(); c++)
                    {
                      line.Empty();
  
                      if(!c)
                        {
                          line.AddFormat(__L("XCHAR* trustedrootcertificates[] = { __L(\"%s\") ,"), originxfiletxt->GetLine(c)->Get()); 
                        }
                       else
                        {
                          line.AddFormat(__L(" __L(\"%s\") ,"), originxfiletxt->GetLine(c)->Get());  
                        }

                      targetxfiletxt->AddLine(line);                                                                            
                    } 

                  line.Empty();
                  line.AddFormat(__L(" };"));  
                  targetxfiletxt->AddLine(line);                                                                            

                  status = targetxfiletxt->WriteAllFile();

                  targetxfiletxt->Close(); 
                }
            }                                     
        }

      originxfiletxt->Close();   
    }

  GEN_DELETE originxfiletxt;

  return status;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XSTRING*>* CIPHERTRUSTEDROOTCERTIFICATESX509::GetLines()
* @brief      get lines
* @ingroup    CIPHER
* 
* @return     XVECTOR<XSTRING*>* : Pointer to the requested string; NULL if it is not available.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>* CIPHERTRUSTEDROOTCERTIFICATESX509::GetLines()
{
  return &lines;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTEDROOTCERTIFICATESX509::DeleteAllLines()
* @brief      GEN_DELETE all lines
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTEDROOTCERTIFICATESX509::DeleteAllLines()
{
  if(lines.IsEmpty())
    {
      return false;
    }

  lines.DeleteContents();
  lines.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_Decode()
* @brief      Decode every PEM certificate into an owned DER buffer
* @ingroup    CIPHER
* 
* @return     bool : true if at least one complete certificate is decoded; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_Decode()
{
  XSTRING certificatebase64;
  bool    iscertificate = false;

  Certificates_DeleteAll();

  for(XDWORD c=0; c<lines.GetSize(); c++)
    {
      XSTRING* line = lines.Get(c);
      if(!line) continue;

      if(line->Find(__L("-----BEGIN CERTIFICATE-----"), true) != XSTRING_NOTFOUND)
        {
          if(iscertificate)
            {
              Certificates_DeleteAll();
              return false;
            }

          certificatebase64.Empty();
          iscertificate = true;
          continue;
        }

      if(line->Find(__L("-----END CERTIFICATE-----"), true) != XSTRING_NOTFOUND)
        {
          XBUFFER* certificate;

          if(!iscertificate || certificatebase64.IsEmpty())
            {
              Certificates_DeleteAll();
              return false;
            }

          certificate = GEN_NEW XBUFFER();
          if(!certificate)
            {
              Certificates_DeleteAll();
              return false;
            }

          if(!certificate->ConvertFromBase64(certificatebase64) || certificate->IsEmpty() || !certificates.Add(certificate))
            {
              GEN_DELETE certificate;
              Certificates_DeleteAll();
              return false;
            }

          certificatebase64.Empty();
          iscertificate = false;
          continue;
        }

      if(iscertificate && !line->IsEmpty()) certificatebase64.Add(line->Get());
    }

  if(iscertificate || certificates.IsEmpty())
    {
      Certificates_DeleteAll();
      return false;
    }

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         XVECTOR<XBUFFER*>* CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_GetAll()
* @brief      Get every decoded DER trust anchor
* @ingroup    CIPHER
* 
* @return     XVECTOR<XBUFFER*>* : Pointer to the owned certificate list.
* 
* --------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XBUFFER*>* CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_GetAll()
{
  return &certificates;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         bool CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_DeleteAll()
* @brief      Delete every decoded DER trust anchor
* @ingroup    CIPHER
* 
* @return     bool : true if the operation is successful; otherwise false.
* 
* --------------------------------------------------------------------------------------------------------------------*/
bool CIPHERTRUSTEDROOTCERTIFICATESX509::Certificates_DeleteAll()
{
  certificates.DeleteContents();
  certificates.DeleteAll();

  return true;
}


/**-------------------------------------------------------------------------------------------------------------------
* 
* @fn         void CIPHERTRUSTEDROOTCERTIFICATESX509::Clean()
* @brief      Clean the attributes of the class: Default initialize
* @note       INTERNAL
* @ingroup    CIPHER
* 
* --------------------------------------------------------------------------------------------------------------------*/
void CIPHERTRUSTEDROOTCERTIFICATESX509::Clean()
{

}
