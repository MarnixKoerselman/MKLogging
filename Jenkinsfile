pipeline {
  agent {
    dockerfile {
      additionalBuildArgs '--tag mklogging-builder'
      customWorkspace 'mklogging'
      dir 'build-container'
      label 'windows && docker'
    }
  }
  stages {
    stage('Build') {
      steps {
        // as Jenkins runs these steps like 'docker exec ... container ...', the container's entrypoint is circumvented which forces us to call VsMSBuildCmd explicitly
        bat """
          call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\BuildTools\\Common7\\Tools\\VsMSBuildCmd.bat"
          MSBuild MkLogging.sln -nologo -target:Restore,Build -property:RestorePackagesConfig=true;Platform=x86;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x86-Debug.log;Encoding=UTF-8
          MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x86;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x86-Release.log;Encoding=UTF-8
          MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Debug -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x64-Debug.log;Encoding=UTF-8
          MSBuild MkLogging.sln -nologo -target:Build -property:Platform=x64;Configuration=Release -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_build_\\msbuild-x64-Release.log;Encoding=UTF-8
        """
      }
      post {
        always {
          recordIssues(tools: [msBuild(pattern: '_build_\\*.log', reportEncoding: 'UTF-8')])
        }
      }
    }
    stage('Test') {
      steps {
        dir('_test_\\x86\\Debug') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml"'
        }
        dir('_test_\\x86\\Release') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml"'
        }
        dir('_test_\\x64\\Debug') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml"'
        }
        dir('_test_\\x64\\Release') {
          bat 'MKLoggingTest.exe --gtest_output="xml:gtest-results.xml"'
        }
        recordIssues(tools: [junitParser(pattern: '**/gtest-results.xml')])
      }
    }
  }
}
