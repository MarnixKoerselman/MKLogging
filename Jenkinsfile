pipeline {
  agent {
    dockerfile {
      additionalBuildArgs '--tag mklogging-builder'
      customWorkspace 'workspace\\mklogging'
      dir 'build-container'
      label 'windows && docker'
    }
  }
  stages {
    stage('Build') {
      steps {
        // as Jenkins runs these steps like 'docker exec ... container ...', the container's entrypoint is circumvented which forces us to call VsMSBuildCmd explicitly
        // cmake is mot msbuild, use VsDevCmd.bat instead of VsMSBuildCmd.bat
        bat """
          @REM call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
          call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvarsamd64_x86.bat"
          cmake --version
          cmake --preset x86-debug
          cmake --build _build_\\x86-debug
          cmake --preset x86-release
          cmake --build _build_\\x86-release
          cmake --preset x64-debug
          cmake --build _build_\\x64-debug
          cmake --preset x64-release
          cmake --build _build_\\x64-release
        """
        // bat """
        //   call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsMSBuildCmd.bat"
        //   MSBuild MkLogging.sln -nologo -target:Restore,Build -property:RestorePackagesConfig=true;Platform=x86;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x86-Debug.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x86;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x86-Release.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x64-Debug.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x64-Release.log;Encoding=UTF-8
        // """
      }
      post {
        always {
          recordIssues(tools: [msBuild()])
        }
      }
    }
    stage('Test') {
      steps {
        dir('_build_\\x86-debug\\MKLoggingTest') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('_build_\\x86-release\\MKLoggingTest') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('_build_\\x64-debug\\MKLoggingTest') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('_build_\\x64-release\\MKLoggingTest') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        junit '**/gtest-results.xml'
        recordIssues(tools: [junitParser(pattern: '**/gtest-results.xml')])
      }
    }
  }
}
