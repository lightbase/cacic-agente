#include "winprocess.h"

#ifdef Q_OS_WIN
BOOL AddAceToWindowStation(HWINSTA hwinsta, PSID psid)
{
    ACCESS_ALLOWED_ACE   *pace = NULL;
    ACL_SIZE_INFORMATION aclSizeInfo;
    BOOL                 bDaclExist;
    BOOL                 bDaclPresent;
    BOOL                 bSuccess = FALSE;
    DWORD                dwNewAclSize;
    DWORD                dwSidSize = 0;
    DWORD                dwSdSizeNeeded;
    PACL                 pacl;
    PACL                 pNewAcl = NULL;
    PSECURITY_DESCRIPTOR psd = NULL;
    PSECURITY_DESCRIPTOR psdNew = NULL;
    PVOID                pTempAce;
    SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
    unsigned int         i;

    // Obtain the DACL for the window station.

    if (!GetUserObjectSecurity(
                hwinsta,
                &si,
                psd,
                dwSidSize,
                &dwSdSizeNeeded)
            )
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            psd = (PSECURITY_DESCRIPTOR)HeapAlloc(
                        GetProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        dwSdSizeNeeded);

            if (psd == NULL)
                __leave;

            psdNew = (PSECURITY_DESCRIPTOR)HeapAlloc(
                        GetProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        dwSdSizeNeeded);

            if (psdNew == NULL)
                __leave;

            dwSidSize = dwSdSizeNeeded;

            if (!GetUserObjectSecurity(
                        hwinsta,
                        &si,
                        psd,
                        dwSidSize,
                        &dwSdSizeNeeded)
                    )
                __leave;
        }
        else
            __leave;

    // Create a new DACL.

    if (!InitializeSecurityDescriptor(
                psdNew,
                SECURITY_DESCRIPTOR_REVISION)
            )
        __leave;

    // Get the DACL from the security descriptor.

    if (!GetSecurityDescriptorDacl(
                psd,
                &bDaclPresent,
                &pacl,
                &bDaclExist)
            )
        __leave;

    // Initialize the ACL.

    ZeroMemory(&aclSizeInfo, sizeof(ACL_SIZE_INFORMATION));
    aclSizeInfo.AclBytesInUse = sizeof(ACL);

    // Call only if the DACL is not NULL.

    if (pacl != NULL)
    {
        // get the file ACL size info
        if (!GetAclInformation(
                    pacl,
                    (LPVOID)&aclSizeInfo,
                    sizeof(ACL_SIZE_INFORMATION),
                    AclSizeInformation)
                )
            __leave;
    }

    // Compute the size of the new ACL.

    dwNewAclSize = aclSizeInfo.AclBytesInUse +
            (2*sizeof(ACCESS_ALLOWED_ACE)) + (2*GetLengthSid(psid)) -
            (2*sizeof(DWORD));

    // Allocate memory for the new ACL.

    pNewAcl = (PACL)HeapAlloc(
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                dwNewAclSize);

    if (pNewAcl == NULL)
        __leave;

    // Initialize the new DACL.

    if (!InitializeAcl(pNewAcl, dwNewAclSize, ACL_REVISION))
        __leave;

    // If DACL is present, copy it to a new DACL.

    if (bDaclPresent)
    {
        // Copy the ACEs to the new ACL.
        if (aclSizeInfo.AceCount)
        {
            for (i=0; i < aclSizeInfo.AceCount; i++)
            {
                // Get an ACE.
                if (!GetAce(pacl, i, &pTempAce))
                    __leave;

                // Add the ACE to the new ACL.
                if (!AddAce(
                            pNewAcl,
                            ACL_REVISION,
                            MAXDWORD,
                            pTempAce,
                            ((PACE_HEADER)pTempAce)->AceSize)
                        )
                    __leave;
            }
        }
    }

    // Add the first ACE to the window station.

    pace = (ACCESS_ALLOWED_ACE *)HeapAlloc(
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psid) -
                sizeof(DWORD));

    if (pace == NULL)
        __leave;

    pace->Header.AceType  = ACCESS_ALLOWED_ACE_TYPE;
    pace->Header.AceFlags = CONTAINER_INHERIT_ACE |
            INHERIT_ONLY_ACE | OBJECT_INHERIT_ACE;
    pace->Header.AceSize  = LOWORD(sizeof(ACCESS_ALLOWED_ACE) +
                                   GetLengthSid(psid) - sizeof(DWORD));
    pace->Mask            = GENERIC_ACCESS;

    if (!CopySid(GetLengthSid(psid), &pace->SidStart, psid))
        __leave;

    if (!AddAce(
                pNewAcl,
                ACL_REVISION,
                MAXDWORD,
                (LPVOID)pace,
                pace->Header.AceSize)
            )
        __leave;

    // Add the second ACE to the window station.

    pace->Header.AceFlags = NO_PROPAGATE_INHERIT_ACE;
    pace->Mask            = WINSTA_ALL;

    if (!AddAce(
                pNewAcl,
                ACL_REVISION,
                MAXDWORD,
                (LPVOID)pace,
                pace->Header.AceSize)
            )
        __leave;

    // Set a new DACL for the security descriptor.

    if (!SetSecurityDescriptorDacl(
                psdNew,
                TRUE,
                pNewAcl,
                FALSE)
            )
        __leave;

    // Set the new security descriptor for the window station.

    if (!SetUserObjectSecurity(hwinsta, &si, psdNew))
        __leave;

    // Indicate success.

    bSuccess = TRUE;

    // Free the allocated buffers.

    if (pace != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)pace);

    if (pNewAcl != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)pNewAcl);

    if (psd != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)psd);

    if (psdNew != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)psdNew);

    return bSuccess;

}

BOOL AddAceToDesktop(HDESK hdesk, PSID psid)
{
    ACL_SIZE_INFORMATION aclSizeInfo;
    BOOL                 bDaclExist;
    BOOL                 bDaclPresent;
    BOOL                 bSuccess = FALSE;
    DWORD                dwNewAclSize;
    DWORD                dwSidSize = 0;
    DWORD                dwSdSizeNeeded;
    PACL                 pacl;
    PACL                 pNewAcl = NULL;
    PSECURITY_DESCRIPTOR psd = NULL;
    PSECURITY_DESCRIPTOR psdNew = NULL;
    PVOID                pTempAce;
    SECURITY_INFORMATION si = DACL_SECURITY_INFORMATION;
    unsigned int         i;


    // Obtain the security descriptor for the desktop object.

    if (!GetUserObjectSecurity(
                hdesk,
                &si,
                psd,
                dwSidSize,
                &dwSdSizeNeeded))
    {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            psd = (PSECURITY_DESCRIPTOR)HeapAlloc(
                        GetProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        dwSdSizeNeeded );

            if (psd == NULL)
                __leave;

            psdNew = (PSECURITY_DESCRIPTOR)HeapAlloc(
                        GetProcessHeap(),
                        HEAP_ZERO_MEMORY,
                        dwSdSizeNeeded);

            if (psdNew == NULL)
                __leave;

            dwSidSize = dwSdSizeNeeded;

            if (!GetUserObjectSecurity(
                        hdesk,
                        &si,
                        psd,
                        dwSidSize,
                        &dwSdSizeNeeded)
                    )
                __leave;
        }
        else
            __leave;
    }

    // Create a new security descriptor.

    if (!InitializeSecurityDescriptor(
                psdNew,
                SECURITY_DESCRIPTOR_REVISION)
            )
        __leave;

    // Obtain the DACL from the security descriptor.

    if (!GetSecurityDescriptorDacl(
                psd,
                &bDaclPresent,
                &pacl,
                &bDaclExist)
            )
        __leave;

    // Initialize.

    ZeroMemory(&aclSizeInfo, sizeof(ACL_SIZE_INFORMATION));
    aclSizeInfo.AclBytesInUse = sizeof(ACL);

    // Call only if NULL DACL.

    if (pacl != NULL)
    {
        // Determine the size of the ACL information.

        if (!GetAclInformation(
                    pacl,
                    (LPVOID)&aclSizeInfo,
                    sizeof(ACL_SIZE_INFORMATION),
                    AclSizeInformation)
                )
            __leave;
    }

    // Compute the size of the new ACL.

    dwNewAclSize = aclSizeInfo.AclBytesInUse +
            sizeof(ACCESS_ALLOWED_ACE) +
            GetLengthSid(psid) - sizeof(DWORD);

    // Allocate buffer for the new ACL.

    pNewAcl = (PACL)HeapAlloc(
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                dwNewAclSize);

    if (pNewAcl == NULL)
        __leave;

    // Initialize the new ACL.

    if (!InitializeAcl(pNewAcl, dwNewAclSize, ACL_REVISION))
        __leave;

    // If DACL is present, copy it to a new DACL.

    if (bDaclPresent)
    {
        // Copy the ACEs to the new ACL.
        if (aclSizeInfo.AceCount)
        {
            for (i=0; i < aclSizeInfo.AceCount; i++)
            {
                // Get an ACE.
                if (!GetAce(pacl, i, &pTempAce))
                    __leave;

                // Add the ACE to the new ACL.
                if (!AddAce(
                            pNewAcl,
                            ACL_REVISION,
                            MAXDWORD,
                            pTempAce,
                            ((PACE_HEADER)pTempAce)->AceSize)
                        )
                    __leave;
            }
        }
    }

    // Add ACE to the DACL.

    if (!AddAccessAllowedAce(
                pNewAcl,
                ACL_REVISION,
                DESKTOP_ALL,
                psid)
            )
        __leave;

    // Set new DACL to the new security descriptor.

    if (!SetSecurityDescriptorDacl(
                psdNew,
                TRUE,
                pNewAcl,
                FALSE)
            )
        __leave;

    // Set the new security descriptor for the desktop object.

    if (!SetUserObjectSecurity(hdesk, &si, psdNew))
        __leave;

    // Indicate success.

    bSuccess = TRUE;


    // Free buffers.
    if (pNewAcl != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)pNewAcl);

    if (psd != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)psd);

    if (psdNew != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)psdNew);


    return bSuccess;
}

BOOL GetLogonSID (HANDLE hToken, PSID *ppsid)
{
    BOOL bSuccess = FALSE;
    DWORD dwIndex;
    DWORD dwLength = 0;
    PTOKEN_GROUPS ptg = NULL;

    // Verify the parameter passed in is not NULL.
    if (NULL == ppsid)
        goto Cleanup;

    // Get required buffer size and allocate the TOKEN_GROUPS buffer.

    if (!GetTokenInformation(
                hToken,         // handle to the access token
                TokenGroups,    // get information about the token's groups
                (LPVOID) ptg,   // pointer to TOKEN_GROUPS buffer
                0,              // size of buffer
                &dwLength       // receives required buffer size
                ))
    {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
            goto Cleanup;

        ptg = (PTOKEN_GROUPS)HeapAlloc(GetProcessHeap(),
                                       HEAP_ZERO_MEMORY, dwLength);

        if (ptg == NULL)
            goto Cleanup;
    }

    // Get the token group information from the access token.

    if (!GetTokenInformation(
                hToken,         // handle to the access token
                TokenGroups,    // get information about the token's groups
                (LPVOID) ptg,   // pointer to TOKEN_GROUPS buffer
                dwLength,       // size of buffer
                &dwLength       // receives required buffer size
                ))
    {
        goto Cleanup;
    }

    // Loop through the groups to find the logon SID.

    for (dwIndex = 0; dwIndex < ptg->GroupCount; dwIndex++)
        if ((ptg->Groups[dwIndex].Attributes & SE_GROUP_LOGON_ID)
                ==  SE_GROUP_LOGON_ID)
        {
            // Found the logon SID; make a copy of it.

            dwLength = GetLengthSid(ptg->Groups[dwIndex].Sid);
            *ppsid = (PSID) HeapAlloc(GetProcessHeap(),
                                      HEAP_ZERO_MEMORY, dwLength);
            if (*ppsid == NULL)
                goto Cleanup;
            if (!CopySid(dwLength, *ppsid, ptg->Groups[dwIndex].Sid))
            {
                HeapFree(GetProcessHeap(), 0, (LPVOID)*ppsid);
                goto Cleanup;
            }
            break;
        }

    bSuccess = TRUE;

Cleanup:

    // Free the buffer for the token groups.

    if (ptg != NULL)
        HeapFree(GetProcessHeap(), 0, (LPVOID)ptg);

    return bSuccess;
}


DWORD FindProcessId(const std::wstring& processName, const DWORD& dwSessionId)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if ( processesSnapshot == INVALID_HANDLE_VALUE )
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if ( !processName.compare(processInfo.szExeFile) )
    {
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);

        HANDLE hToken;
        if( OpenProcessToken(hProcess,TOKEN_READ,&hToken) ) {
            DWORD ssidToken;
            DWORD returnLength = sizeof(DWORD);

            if ( GetTokenInformation(hToken,TokenSessionId,&ssidToken,returnLength,&returnLength) ) {
                if(ssidToken == dwSessionId) {
                    CloseHandle(hProcess);
                    CloseHandle(hToken);
                    CloseHandle(processesSnapshot);
                    return processInfo.th32ProcessID;
                }
            }
        }
    }

    while ( Process32Next(processesSnapshot, &processInfo) )
    {
        if ( !processName.compare(processInfo.szExeFile) )
        {
            //            CloseHandle(processesSnapshot);
            //            return processInfo.th32ProcessID;
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);

            HANDLE hToken;
            if( OpenProcessToken(hProcess,TOKEN_READ,&hToken) ) {
                DWORD ssidToken;
                DWORD returnLength = sizeof(DWORD);

                if ( GetTokenInformation(hToken,TokenSessionId,&ssidToken,returnLength,&returnLength) ) {
                    if(ssidToken == dwSessionId) {
                        CloseHandle(hProcess);
                        CloseHandle(hToken);
                        CloseHandle(processesSnapshot);
                        return processInfo.th32ProcessID;
                    }
                }
            }
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

VOID FreeLogonSID (PSID *ppsid)
{
    HeapFree(GetProcessHeap(), 0, (LPVOID)*ppsid);
}


#endif //ifdef Q_OS_WIN
