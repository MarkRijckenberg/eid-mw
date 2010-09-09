/* ****************************************************************************

 * eID Middleware Project.
 * Copyright (C) 2008-2009 FedICT.
 *
 * This is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version
 * 3.0 as published by the Free Software Foundation.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this software; if not, see
 * http://www.gnu.org/licenses/.

**************************************************************************** */
#include "GlobMdrv.h"

#include "Log.h"
#include "util.h"
#include "smartcard.h"

/****************************************************************************************************/
//
// Function: CardCreateContainer
//

#define WHERE "CardCreateContainer()"
DWORD WINAPI   CardCreateContainer
               (
                  __in      PCARD_DATA  pCardData,
                  __in      BYTE        bContainerIndex,
                  __in      DWORD       dwFlags,
                  __in      DWORD       dwKeySpec,
                  __in      DWORD       dwKeySize,
                  __in      PBYTE       pbKeyData
               )
{
   DWORD    dwReturn = 0;
   LogTrace(LOGTYPE_INFO, WHERE, "Enter API...");

   CLEANUP(SCARD_E_UNSUPPORTED_FEATURE);

cleanup:

   LogTrace(LOGTYPE_INFO, WHERE, "Exit API...");
   return(dwReturn);
}
#undef WHERE

/****************************************************************************************************/

//
// Function: CardDeleteContainer
//
// Purpose: Delete the specified key container.
//

#define WHERE "CardDeleteContainer()"
DWORD WINAPI   CardDeleteContainer
               (
                  __in      PCARD_DATA  pCardData,
                  __in      BYTE        bContainerIndex,
                  __in      DWORD       dwReserved
               )
{
   DWORD    dwReturn = 0;
   LogTrace(LOGTYPE_INFO, WHERE, "Enter API...");

   CLEANUP(SCARD_E_UNSUPPORTED_FEATURE);

cleanup:

   LogTrace(LOGTYPE_INFO, WHERE, "Exit API...");
   return(dwReturn);
}
#undef WHERE

/****************************************************************************************************/

//
// Function: CardGetContainerInfo
//
// Purpose: Query for all public information available about
//          the named key container.  This includes the Signature
//          and Key Exchange type public keys, if they exist.
//
//          The pbSigPublicKey and pbKeyExPublicKey buffers contain the
//          Signature and Key Exchange public keys, respectively, if they
//          exist.  The format of these buffers is a Crypto
//          API PUBLICKEYBLOB - 
//
//              BLOBHEADER
//              RSAPUBKEY
//              modulus
//          
//          In the case of ECC public keys, the pbSigPublicKey will contain
//          the ECDSA key and pbKeyExPublicKey will contain the ECDH key if
//          they exist. ECC key structure -
//
//              BCRYPT_ECCKEY_BLOB
//              X coord (big endian)
//              Y coord (big endian)
//

#define WHERE "CardGetContainerInfo()"
DWORD WINAPI   CardGetContainerInfo
               (
                  __in      PCARD_DATA       pCardData,
                  __in      BYTE             bContainerIndex,
                  __in      DWORD            dwFlags,
                  __in      PCONTAINER_INFO  pContainerInfo
               )
{
   DWORD                dwReturn  = 0;
   DWORD                dwVersion = 0;
   DWORD                cbCertif = 0;
   DWORD				dwCertSpec = 0;
   PBYTE				pbCertif = NULL;
   PCARD_LIST_TYPE      pCardItem = NULL;

   LogTrace(LOGTYPE_INFO, WHERE, "Enter API...[%d]", bContainerIndex);

   /********************/
   /* Check Parameters */
   /********************/
   if ( pCardData == NULL )
   {
      LogTrace(LOGTYPE_ERROR, WHERE, "Invalid parameter [pCardData]");
      CLEANUP(SCARD_E_INVALID_PARAMETER);
   }

   if ( ( bContainerIndex != 0 ) &&
        ( bContainerIndex != 1 ) )
   {
      LogTrace(LOGTYPE_ERROR, WHERE, "Invalid parameter [bContainerIndex]");
      CLEANUP(SCARD_E_NO_KEY_CONTAINER);
   }

   if ( dwFlags != 0 )
   {
      LogTrace(LOGTYPE_ERROR, WHERE, "Invalid parameter [dwFlags]");
      CLEANUP(SCARD_E_INVALID_PARAMETER);
   }

   if ( pContainerInfo == NULL )
   {
      LogTrace(LOGTYPE_ERROR, WHERE, "Invalid parameter [pContainerInfo]");
      CLEANUP(SCARD_E_INVALID_PARAMETER);
   }

   dwVersion = (pContainerInfo->dwVersion == 0) ? 1 : pContainerInfo->dwVersion;
   if ( dwVersion != CONTAINER_INFO_CURRENT_VERSION )
   {
      LogTrace(LOGTYPE_ERROR, WHERE, "Invalid parameter [pContainerInfo->dwVersion]");
      CLEANUP(ERROR_REVISION_MISMATCH );
   }

   //pCardItem = GetCardListItem(pCardData);
   //if ( pCardItem == NULL )
   //{
   //   LogTrace(LOGTYPE_ERROR, WHERE, "Card context and handle not Found...");
   //   CLEANUP(SCARD_E_UNEXPECTED);
   //}

//////   pContainerInfo->dwVersion        = pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.dwVersion;
//   pContainerInfo->dwReserved       = pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.dwReserved;
//   pContainerInfo->cbSigPublicKey   = pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.cbSigPublicKey;
//   if ( pContainerInfo->cbSigPublicKey > 0 )
//   {
//      pContainerInfo->pbSigPublicKey = pCardData->pfnCspAlloc(pContainerInfo->cbSigPublicKey);
//      if ( pContainerInfo->pbSigPublicKey == NULL )
//      {
//         LogTrace(LOGTYPE_ERROR, WHERE, "Error allocating memory for [pContainerInfo->pbSigPublicKey]");
//         CLEANUP(SCARD_E_NO_MEMORY);
//      }
//
//      memcpy(pContainerInfo->pbSigPublicKey, pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.pbSigPublicKey, pContainerInfo->cbSigPublicKey); 
//
//      LogTrace(LOGTYPE_INFO, WHERE, "SigPublicKey [%d]", pContainerInfo->cbSigPublicKey);
//#ifdef _DEBUG
//   LogDump (pContainerInfo->cbSigPublicKey, (char *)pContainerInfo->pbSigPublicKey);
//#endif
//   }
//   else
//   {
//      pContainerInfo->cbSigPublicKey = 0;
//      pContainerInfo->pbSigPublicKey = NULL;
//   }
//
//   pContainerInfo->cbKeyExPublicKey = pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.cbKeyExPublicKey;
//   if ( pContainerInfo->cbKeyExPublicKey > 0 )
//   {
//      pContainerInfo->pbKeyExPublicKey = pCardData->pfnCspAlloc(pContainerInfo->cbKeyExPublicKey);
//      if ( pContainerInfo->pbKeyExPublicKey == NULL )
//      {
//         LogTrace(LOGTYPE_ERROR, WHERE, "Error allocating memory for [pContainerInfo->pbKeyExPublicKey]");
//         CLEANUP(SCARD_E_NO_MEMORY);
//      }
//
//      memcpy(pContainerInfo->pbKeyExPublicKey, pCardItem->ContainerInfo[bContainerIndex].ContainerInfo.pbKeyExPublicKey, pContainerInfo->cbKeyExPublicKey); 
//
//      LogTrace(LOGTYPE_INFO, WHERE, "KeyExPublicKey [%d]", pContainerInfo->cbKeyExPublicKey);
//#ifdef _DEBUG
//   LogDump (pContainerInfo->cbSigPublicKey, (char *)pContainerInfo->pbKeyExPublicKey);
//#endif
//   }
//   else
//   {
//      pContainerInfo->cbKeyExPublicKey = 0;
//      pContainerInfo->pbKeyExPublicKey = NULL;
//   }

    /*
    * Authentication Certificate
    */
	if (bContainerIndex == 0) {
		LogTrace(LOGTYPE_INFO, WHERE, "Creating Authentication Certif...");
		dwCertSpec = CERT_AUTH;
	}
	if (bContainerIndex == 1) {
		LogTrace(LOGTYPE_INFO, WHERE, "Creating Non-Repudiation Certif...");
		dwCertSpec = CERT_NONREP;
	}

	dwReturn = BeidReadCert(pCardData, dwCertSpec, &cbCertif, &pbCertif);
	if ( dwReturn != SCARD_S_SUCCESS )
	{
		if (bContainerIndex == 0)
			LogTrace(LOGTYPE_ERROR, WHERE, "BeidReadCert[CERT_AUTH] returned [%d]", dwReturn);
		if (bContainerIndex == 1)
			LogTrace(LOGTYPE_ERROR, WHERE, "BeidReadCert[CERT_NONREP] returned [%d]", dwReturn);
		CLEANUP(SCARD_E_UNEXPECTED);
	}

#ifdef _DEBUG
	LogDump (cbCertif, (char *)pbCertif);
	if (bContainerIndex == 0)
		LogDumpBin ("C:\\SmartCardMinidriverTest\\auth.crt", cbCertif, (char *)pbCertif);
	if (bContainerIndex == 1)
		LogDumpBin ("C:\\SmartCardMinidriverTest\\nonrep.crt", cbCertif, (char *)pbCertif);
#endif
   
   /* Container Info */
	pContainerInfo->dwVersion  = CONTAINER_INFO_CURRENT_VERSION;
	pContainerInfo->dwReserved = 0;
	dwReturn = BeidGetPubKey(pCardData, 
                            cbCertif, 
                            pbCertif, 
                            &(pContainerInfo->cbSigPublicKey), 
                            &(pContainerInfo->pbSigPublicKey));
	if ( dwReturn != SCARD_S_SUCCESS )
	{
		LogTrace(LOGTYPE_ERROR, WHERE, "BeidGetPubKey returned [%d]", dwReturn);
		CLEANUP(SCARD_E_UNEXPECTED);
	}
	pContainerInfo->cbKeyExPublicKey = 0;
	pContainerInfo->pbKeyExPublicKey = NULL;

#ifdef _DEBUG
	if (bContainerIndex == 0)
  		LogDumpBin("C:\\SmartCardMinidriverTest\\authpk.bin", pContainerInfo->cbSigPublicKey
                                                       , (char *)pContainerInfo->pbSigPublicKey);
	if (bContainerIndex == 1)
  		LogDumpBin("C:\\SmartCardMinidriverTest\\nonreppk.bin", pContainerInfo->cbSigPublicKey
                                                       , (char *)pContainerInfo->pbSigPublicKey);
#endif
	
cleanup:
	LogTrace(LOGTYPE_INFO, WHERE, "Exit API...");
	return(dwReturn);
}
#undef WHERE

/****************************************************************************************************/