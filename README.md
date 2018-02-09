# VirtualFileSystem

Implementation of virtual file system in c++, way of use:

 ./prog <vfs name> command (...)
 
 list of commands: 
 - create <size in bytes>
 - map
 - list
 - push <source file name> <destination file name>
 - pull <source file name> <destination file name>
 - remove <file name>
 - delete
