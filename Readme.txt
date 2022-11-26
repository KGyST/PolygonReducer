This project uses
-Windows SDK 7.1
-Archicad Api Devkit 19
-Boost

TODO
-Boost.Test
-S::Array<Type>::begin()
-S_Stuff as submodule

Developer Command Prompt
D:\Git\support\boost_1_78_0>

bootstrap.bat vc10
b2.exe address-model=64 architecture=x86 runtime-link=static --with-filesystem --with-system 
