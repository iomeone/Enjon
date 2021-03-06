@echo off
pushd bin

set libs=Opengl32.lib SDL2.lib SDL2main.lib glew32.lib glew32s.lib freetype.lib Bullet3Collision.lib Bullet3Dynamics.lib
set libs2=Bullet3Common.lib LinearMath.lib BulletDynamics.lib BulletCollision.lib
set src1=..\src\*.cpp ..\src\Graphics\*.cpp ..\src\IO\*.cpp ..\src\Math\*.cpp ..\src\Physics\*.cpp ..\src\Utils\*.cpp 
set src2=..\src\Entity\*.cpp ..\src\Entity\Components\*.cpp ..\src\IMGui\*.cpp
set src3=..\IsoARPG\src\main.cpp ..\IsoARPG\src\Game.cpp
set inc=/I ..\include /I ..\deps\include /I ..\IsoARPG\include

cl /w -Zi /MP /Femain.exe ^
%src1% %src2% %src3% ^
%inc% ^
/EHsc /link /SUBSYSTEM:CONSOLE /NODEFAULTLIB:msvcrt.lib /LIBPATH:"..\deps\lib\" %libs% %libs2%

popd
