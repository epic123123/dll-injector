# Basic Info
This is a simple command-line dll injector, written in C++. It utilizes the basic WINAPI functions that every dll injector uses, such as OpenProcess, 
VirtualAllocEx, WriteProcessMemory, and CreateRemoteThread.

# How It Works
Dll Injectors work like this: you specify the path to your dll, the assembly to inject, let's call it dllpath. You also specify the process in which to inject in. Let's call this targetproc. The program gets the process ID (or PID) of targetproc, then it gets the HANDLE to targetproc using the OpenProcess function. Using that handle, The program allocates a given amount of memory, which in our case is MAX_PATH, because we will put our dllpath in there, in targetproc's address space using VirtualAllocEx function. VirtualAllocEx returns a hex memory address where the allocation happened. Using that, the program writes dllpath (the path of the dll you specified) INTO targetproc's memory. Here comes the interesting part: the program utilizes the CreateRemoteThread function, to create a new thread IN targetproc, and tells it to call the LoadLibraryA function, passing it the memory address where lies our dll's path in targetproc's address space. Then the LoadLibraryA function LOADS the dll specified in the path in that memory address returned by VirtualAllocEx, therefore executing our (potentially malicious) code.

That's it. If it seems kinda complicated, that's because 1) I am bad at explaining or 2) It won't when you start to understand this stuff better.
