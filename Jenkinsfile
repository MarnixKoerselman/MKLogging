pipeline {
  agent {
    dockerfile {
      additionalBuildArgs '--tag mklogging-builder'
      customWorkspace 'workspace\\mklogging'
      dir 'jenkins-windows-build-container'
      label 'windows && ltsc2019 && docker'
    }
  }
  stages {
    stage('Build') {
      steps {
        // as Jenkins runs these steps like 'docker exec ... container ...', the container's entrypoint is circumvented which forces us to call VsMSBuildCmd explicitly
        // cmake is mot msbuild, use VsDevCmd.bat instead of VsMSBuildCmd.bat
        bat """
          call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
          cmake --version
          cmake --preset windows-x86-vs2022
          cmake --build --preset windows-x86-debug --parallel
          cmake --build --preset windows-x86-release --parallel
          cmake --preset windows-x64-vs2022
          cmake --build --preset windows-x64-debug --parallel
          cmake --build --preset windows-x64-release --parallel
        """

      }
      post {
        always {
          recordIssues(tools: [msBuild()])
        }
      }
    }
    stage('Test') {
      steps {
        dir('build\\windows-x86-vs2022\\bin\\Debug') {
          bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\windows-x86-vs2022\\bin\\Release') {
          bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\windows-x64-vs2022\\bin\\Debug') {
          bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        dir('build\\windows-x64-vs2022\\bin\\Release') {
          bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
        }
        junit '**/gtest-results.xml'
        recordIssues(tools: [junitParser(pattern: '**/gtest-results.xml')])
      }
    }
  }
}
