// AC_Trainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <windows.h>
#include "proc.h"

static void debug_message(std::string message, int velocity, std::string key)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    std::cout << "[" << key << "] ";
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);

    std::string first_message = message;
    int x = 0;
    while (first_message[x] != '\0')
    {
        std::cout << first_message[x];
        Sleep(velocity);
        x++;
    };
}

int main()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get ProcId of the target process
    DWORD procId = GetProcId(L"ac_client.exe");

    // Getmodulebaseaddress
    uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

    // Get Handle to Process
    HANDLE hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Resolve base address of the pointer chain
    uintptr_t dynamicPtrBaseAddr = moduleBase + 0x10F4F4;
    std::cout << "Allocated at Address -> " << "0x" << std::hex << dynamicPtrBaseAddr << " <-" << std::endl;

    // Resolve our ammo pointer chain
    std::vector<unsigned int> ammoOffsets = {0x374, 0x14, 0x0};
    std::vector<unsigned int> healthOffsets = { 0xF8 };
    uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, ammoOffsets);
    uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseAddr, healthOffsets);

    // Read Ammo value
    int ammoValue = 0;
    int healthValue = 0;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

    // Write to it
    int newAmmo = 99999;
    int newHealth = 99999;

    ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

    debug_message("Infinite Ammo\n", 20, "VK_F2");
    debug_message("Infinite Health\n", 20, "VK_F1");
    debug_message("Quit", 20, "VK_END");

    while (true)
    {

        if (GetAsyncKeyState(VK_F2))
            WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);

        if (GetAsyncKeyState(VK_F1))
            WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

        if (GetAsyncKeyState(VK_END))
            break;
    }

    return 0;

}
