@echo off

set CodeDir=..\code
set OutputDir=..\build_win32

set CompileFile=win32_graphics

set CompilerFlags=-Od -nologo -Zi
set LinkerFlags=-opt:ref user32.lib gdi32.lib

IF NOT EXIST %OutputDir% mkdir %OutputDir%

pushd %OutputDir%

del *.pdb > NULL 2> NULL

call cl.exe %CompilerFlags% %CodeDir%\%CompileFile%.cpp -Fm%CompileFile%.map /link %LinkerFlags%

start %CompileFile%.exe

popd