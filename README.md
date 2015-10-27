## UDisks2 Qt binders
A basic set of bindings to org.freedesktop.UDisks2.*.  Designed for people who hate object sprawl.  

Part of the playground series of repositories.  The playground series are helper classes/apps I wrote, mainly uploaded for posterity.

## Features
Supports enough to aid "play disc" functionality (detect and mount) in media players.

- Property inspection
- Block attach/remove events
- Drive attach/remove events
- Filesystem present/unpresent events
- Mount/Unmount operations (block devices only)

## Files
- `Udisk2.*` UDisk2 classes
- `MainWindow.*` Example window
- `UDisks2.pro` Example project

## Prequisites
- Qt5 SDK
- UDisks2
- C++11 compiler

## Compile
Designed with Qt5 in mind.  Compile with qmake or Qt Creator, and a C++11 compiler.  Requires `CONFIG += C++11` in your qt project file.
