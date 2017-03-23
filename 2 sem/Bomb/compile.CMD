@nasm -fwin32 %1.asm
@link /subsystem:console /nodefaultlib /entry:main %1.obj ..\useful_funcs.obj ..\kernel32.lib
@%1