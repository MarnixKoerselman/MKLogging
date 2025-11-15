pipeline {
  agent {
    dockerfile {
      additionalBuildArgs '--tag mklogging-builder'
      customWorkspace 'workspace\\mklogging'
      dir 'jenkins-windows-build-container'
      label 'windows && docker'
    }
  }
  stages {
    stage('Build') {
      steps {
        // as Jenkins runs these steps like 'docker exec ... container ...', the container's entrypoint is circumvented which forces us to call VsMSBuildCmd explicitly
        // cmake is mot msbuild, use VsDevCmd.bat instead of VsMSBuildCmd.bat
        bat """
          call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
          @REM call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\VC\\Auxiliary\\Build\\vcvarsamd64_x86.bat"
          cmake --version
          cmake --preset x86
          cmake --build --preset x86-debug --parallel
          cmake --build --preset x86-release --parallel
          cmake --preset x64
          cmake --build --preset x64-debug --parallel
          cmake --build --preset x64-release --parallel
        """
        // bat """
        //   call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsMSBuildCmd.bat"
        //   MSBuild MkLogging.sln -nologo -target:Restore,Build -property:RestorePackagesConfig=true;Platform=x86;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=build\\msbuild-x86-Debug.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x86;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=build\\msbuild-x86-Release.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=build\\msbuild-x64-Debug.log;Encoding=UTF-8
        //   MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=build\\msbuild-x64-Release.log;Encoding=UTF-8
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
        dir('build\\x86\\MKLoggingTest\\Debug') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\x86\\MKLoggingTest\\Release') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\x64\\MKLoggingTest\\Debug') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\x64\\MKLoggingTest\\Release') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        junit '**/gtest-results.xml'
        recordIssues(tools: [junitParser(pattern: '**/gtest-results.xml')])
      }
    }
  }
}
