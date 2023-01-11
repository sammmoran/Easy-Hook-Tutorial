// Easy-Hook-Tutorial.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>
#include <easyhook.h>
#include <tchar.h>

#if _WIN64
#pragma comment(lib, "EasyHook64.lib")
#else
#pragma comment(lib, "EasyHook32.lib")
#endif

using namespace std;


// Hook function declaration
BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration);


// Hook function implementation
BOOL WINAPI myBeepHook(DWORD dwFreq, DWORD dwDuration) {

	cout << "Your beeps have been acquired!\n";
	return Beep(dwFreq + 800, dwDuration);

}

int _tmain(int argc, _TCHAR* argv[]) {

	// Need a reference to keep track of our hook
	HOOK_TRACE_INFO hHook = { NULL };
	cout << "\n";
	cout << GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep");

	// Install hook directly into the DLL beep function in kernel32.dll
	NTSTATUS result = LhInstallHook(
		GetProcAddress(GetModuleHandle(TEXT("kernel32")), "Beep"),
		myBeepHook,
		NULL,
		&hHook
	);

	
	// If our install failed
	if (FAILED(result)) {
		
		// Error handling
		wstring s(RtlGetLastErrorString());
	
		wcout << "Failed to install hook: ";
		wcout << s;
		cout << "\n\nPress any key to exit: ";
		cin.get();
		return -1;

	}

	// If our install succeeded
	cout << "Beep after hook installed but not enabled.\n";
	Beep(500, 500);

	cout << "Activating hook for current thread only.\n";

	// If the threadId in the ACL is set to 0, then internally EasyHook uses GetCurrentThread()
	// We are setting the ACL entries that the hook will pay attention to. This will allow the hook
	// to discern between threads it is interested in monitoring and others it's not.
	ULONG ACLEntries[1] = { 0 };
	LhSetInclusiveACL(ACLEntries, 1, &hHook);

	// At this point, hook should now be enabled
	// Run the Beep function and let's see how the hook behaves
	cout << "Beep after hook enabled.\n";
	Beep(500, 500);

	// Now, uninstall hook
	cout << "Uninstall hook\n";
	LhUninstallHook(&hHook);

	// Now, beep after uninstalling hook
	cout << "Beep after hook uninstalled\n";
	Beep(500, 500);

	cout << "\n\nRestore ALL entry points of pending removals issued by LhUninstallHook()\n";
	LhWaitForPendingRemovals();

	cout << "Press any key to exit.";
	cin.get();
	return 0;


}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
