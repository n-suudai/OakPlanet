@echo off

set PATH=%PATH%;C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin;

MSBuild.exe Core.sln /t:build /p:Configuration=Debug;Platform="x64"
rem MSBuild.exe Core.sln /t:build /p:Configuration=Release;Platform="x64"