# escape=`

# Based on:
# https://docs.microsoft.com/en-us/visualstudio/install/advanced-build-tools-container?view=vs-2019
# https://devblogs.microsoft.com/setup/no-container-image-for-build-tools-for-visual-studio-2017/

ARG FROM_IMAGE=mcr.microsoft.com/windows/servercore:ltsc2019
FROM ${FROM_IMAGE}

# VS2017=15; VS2019=16; VS2022=17
ARG CHANNEL=16
# Download Build Tools for Visual Studio 2022.
ADD https://aka.ms/vs/${CHANNEL}/release/channel C:\TEMP\channel.cham
ADD https://dist.nuget.org/win-x86-commandline/latest/nuget.exe C:\Tools\nuget.exe
ADD https://aka.ms/vs/16/release/vs_buildtools.exe C:\TEMP\vs_buildtools.exe

# Install Build Tools for Visual Studio 2019.
# See https://docs.microsoft.com/en-us/visualstudio/install/workload-component-id-vs-build-tools?view=vs-2019
# NB: Microsoft.VisualStudio.Component.Windows10SDK is automatically included, but not enough?!
RUN C:\TEMP\vs_buildtools.exe --quiet --wait --norestart --nocache `
  --channelUri C:\TEMP\channel.cham `
  --installChannelUri C:\TEMP\channel.cham `
  --add Microsoft.VisualStudio.Workload.VCTools `
  --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
  --add Microsoft.VisualStudio.Component.Windows10SDK.19041
  # --add Microsoft.VisualStudio.Component.VC.ATL
  #  `
  # --add Microsoft.VisualStudio.Component.VC.CMake.Project `
  # --add Microsoft.VisualStudio.Component.VC.ATLMFC

# Optional packages that could be included for backward compatibility with VS2015 and VS2017.
# NB: Windows 8.1 SDK is _not_ included in VS and would have to be installed separately.
# --add Microsoft.VisualStudio.Component.VC.140 `
# --add Microsoft.VisualStudio.Component.VC.v141.x86.x64

# %comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsamd64_x86.bat"
# %comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars32.bat"
# %comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvars64.bat"

# "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsMSBuildCmd.bat"

RUN setx /m PATH "%PATH%;C:\Tools"
