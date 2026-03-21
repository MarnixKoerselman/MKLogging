# escape=`

# Based on:
# https://docs.microsoft.com/en-us/visualstudio/install/advanced-build-tools-container?view=vs-2019
# https://devblogs.microsoft.com/setup/no-container-image-for-build-tools-for-visual-studio-2017/

ARG FROM_IMAGE=mcr.microsoft.com/windows/servercore:ltsc2022
FROM ${FROM_IMAGE}

# VS2017=15; VS2019=16; VS2022=17; VS2026=18=stable
# https://developercommunity.visualstudio.com/t/MissingVersion-SpecificDownloadLinkforVisualStudio2026BuildTools/11002499
# Download Build Tools for Visual Studio.
ARG VS_VERSION=stable
# [ 15/release, 16/release/, 17/release, stable ]
ADD https://aka.ms/vs/${VS_VERSION}/vs_buildtools.exe C:\TEMP\vs_buildtools.exe

# Install Build Tools for Visual Studio.
# See https://docs.microsoft.com/en-us/visualstudio/install/workload-component-id-vs-build-tools
RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache --includeRecommended `
  --add Microsoft.VisualStudio.Workload.VCTools

# MinGit is git for applications. It is necessary to install git in the Agent, because Jenkins won't add it to the path if Jenkins installs it - which means that only Jenkins knows where to find it, and applications (such as vcpkg) do not.
ADD https://github.com/git-for-windows/git/releases/download/v2.53.0.windows.2/MinGit-2.53.0.2-64-bit.zip C:\TEMP\MinGit.zip
RUN PowerShell -Command "Expand-Archive -Path C:\TEMP\MinGit.zip -DestinationPath C:\MinGit; [Environment]::SetEnvironmentVariable('PATH', $env:PATH + ';C:\MinGit\cmd', 'Machine')"

# "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"
# "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsMSBuildCmd.bat"
# "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\vsdevcmd\core\msbuild.bat"

# ADD https://dist.nuget.org/win-x86-commandline/latest/nuget.exe C:\Tools\nuget.exe
# RUN setx /m PATH "%PATH%;C:\Tools"
