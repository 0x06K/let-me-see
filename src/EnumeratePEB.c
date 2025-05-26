#include <windows.h>
#include <winternl.h>
#include <stdio.h>

typedef NTSTATUS(WINAPI* PNtQueryInformationProcess)(
    HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG
);
typedef struct _RTL_USER_PROCESS_PARAMETERS_PARTIAL {
    BYTE Reserved1[16];
    PVOID Reserved2[10];
    PWSTR Environment;  // Pointer to environment block
} RTL_USER_PROCESS_PARAMETERS_PARTIAL;
void DumpEnvironmentVariables(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (!hProcess) {
        printf("OpenProcess failed: %lu\n", GetLastError());
        return;
    }

    // Get NtQueryInformationProcess
    PNtQueryInformationProcess NtQueryInfo =
        (PNtQueryInformationProcess)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationProcess");

    PROCESS_BASIC_INFORMATION pbi;
    NTSTATUS status = NtQueryInfo(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), NULL);
    if (status != 0) {
        printf("NtQueryInformationProcess failed: 0x%08X\n", status);
        CloseHandle(hProcess);
        return;
    }

    // Read PEB
    PEB peb;
    if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), NULL)) {
        printf("Failed to read PEB: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return;
    }

    // Read RTL_USER_PROCESS_PARAMETERS
    RTL_USER_PROCESS_PARAMETERS_PARTIAL procParams;
    if (!ReadProcessMemory(hProcess, peb.ProcessParameters, &procParams, sizeof(procParams), NULL)) {
        printf("Failed to read process parameters: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return;
    }

    // Read Environment block
    WCHAR env[32767]; // max size for env block
    if (!ReadProcessMemory(hProcess, procParams.Environment, &env, sizeof(env), NULL)) {
        printf("Failed to read environment block: %lu\n", GetLastError());
        CloseHandle(hProcess);
        return;
    }

    // Environment is a block of null-terminated strings, double null at the end
    WCHAR* p = env;
    while (*p) {
        wprintf(L"%s\n", p);
        p += wcslen(p) + 1;
    }

    CloseHandle(hProcess);
}

int main() {
    // Replace with Actual PID
    DWORD targetPID = 1484;
    DumpEnvironmentVariables(targetPID);
    return 0;
}
