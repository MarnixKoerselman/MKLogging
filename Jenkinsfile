pipeline {
  agent none
  stages {
    stage('Build and Test') {
      matrix {
        agent {
          dockerfile {
            customWorkspace 'MKlogging'
            dir 'build-container'
            filename 'Dockerfile'
            label 'windows && docker'
          }
        }
        axes {
          axis {
            name 'BUILD_CONFIGURATION'
            values 'Debug', 'Release'
          }
          axis {
            name 'BUILD_PLATFORM'
            values 'x86', 'x64'
          }
        }
        stages {
          stage('Build') {
            steps {
              bat "echo ${BUILD_PLATFORM}-${BUILD_CONFIGURATION}"
              // bat """
              // call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\Common7\Tools\VsMSBuildCmd.bat"
              // MSBuild MkLogging.sln -nologo -target:Restore,Build -property:RestorePackagesConfig=true;Platform=${BUILD_PLATFORM};Configuration=${BUILD_CONFIGURATION} -maxCpuCount:1 -fileLogger -fileLoggerParameters:LogFile=_testreports_\msbuild=${BUILD_PLATFORM}-${BUILD_CONFIGURATION}.log
              // """
            }
          }
        }
      }
    }
  }
}
