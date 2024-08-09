# escape=`

# Based on:
# https://docs.microsoft.com/en-us/visualstudio/install/advanced-build-tools-container?view=vs-2019
# https://devblogs.microsoft.com/setup/no-container-image-for-build-tools-for-visual-studio-2017/

ARG FROM_IMAGE=mcr.microsoft.com/windows/servercore:ltsc2019
FROM ${FROM_IMAGE}

# VS2017=15; VS2019=16; VS2022=17
# Download Build Tools for Visual Studio 2022.
ADD https://aka.ms/vs/17/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe

# Install Build Tools for Visual Studio.
# See https://docs.microsoft.com/en-us/visualstudio/install/workload-component-id-vs-build-tools
RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache --includeRecommended `
  --add Microsoft.VisualStudio.Workload.VCTools

# https://github.com/git-for-windows/git/wiki/Silent-or-Unattended-Installation
RUN PowerShell -Command "& {$ProgressPreference='SilentlyContinue' ; Invoke-WebRequest -UseBasicParsing https://github.com/git-for-windows/git/releases/download/v2.43.0.windows.1/Git-2.43.0-64-bit.exe -OutFile C:\TEMP\git-installer.exe}" `
&& C:\TEMP\git-installer.exe /VERYSILENT /NORESTART /NOCANCEL /SP- /CLOSEAPPLICATIONS /NOICONS /NORESTARTAPPLICATIONS

# "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsMSBuildCmd.bat"

# ADD https://dist.nuget.org/win-x86-commandline/latest/nuget.exe C:\Tools\nuget.exe
# RUN setx /m PATH "%PATH%;C:\Tools"
