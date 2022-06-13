# Basic Info
This is a simple command-line dll injector, written in C++. It utilizes the basic WINAPI functions that every dll injector uses, such as OpenProcess, 
VirtualAllocEx, WriteProcessMemory, and CreateRemoteThread.

Also by the way, there is a chance your antivirus might not like this.

# How It Works
Dll Injectors work like this: you specify the path to your dll, the assembly to inject, let's call it dllpath. You also specify the process in which to inject in. Let's call this targetproc. The program gets the process ID (or PID) of targetproc, then it gets the HANDLE to targetproc using the OpenProcess function. Using that handle, The program allocates a given amount of memory, which in our case is MAX_PATH, because we will put our dllpath in there, in targetproc's address space using VirtualAllocEx function. VirtualAllocEx returns a hex memory address where the allocation happened. Using that, the program writes dllpath (the path of the dll you specified) INTO targetproc's memory. Here comes the interesting part: the program utilizes the CreateRemoteThread function, to create a new thread IN targetproc, and tells it to call the LoadLibraryA function, passing it the memory address where lies our dll's path in targetproc's address space. Then the LoadLibraryA function LOADS the dll specified in the path in that memory address returned by VirtualAllocEx, therefore executing our (potentially malicious) code.

That's it. If it seems kinda complicated, that's because 1) I am bad at explaining or 2) It won't when you start to understand this stuff better.


# Usage

```
C:\prancing_pony\gangsterprankster>dll-injector.exe -i 12345 -dll "C:\jenny\example-dll.dll"
```

-i: define the process ID of the target process
-dll: define the path to your dll which is to be injected
-n (not recommended): define the name with which to look for process ID

The -n cannot be trusted and it hardly ever works. You could try it if it works on your machine, but for some reason on my PC the szExeFile doesn't have any processes, so it might even work on your machine. But if you don't want to mess around, use some software to find out the process id of your process and use -i to pass that in. Using that will work. Also, if the path of your dll contains spaces, remember to wrap them with double-quotes, like this "C:\Users\Jackob Manningham\dll.dll". Otherwise it will treat C:\Users\Jackob AND Manningham\dll.dll as different arguments, which will not pass the argument check. If you want to ask a series of questions, then my discord is kutch#1290. Feel free to contact on any manner, I don't bite, unless you want me to.

```
IMPORTANT: Remember to run the program as an ADMINISTRATOR, otherwise you might run to some issues!
```
