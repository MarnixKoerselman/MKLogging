pipeline {
  agent none
  stages {
    parallel {
      stage('windows') {
        agent {
          node {
            label 'windows && ws2022 && docker-engine'
            customWorkspace 'workspace\\mklogging'
          }
        }
        stages {
          stage('VS2022') {
            agent {
              dockerfile {
                additionalBuildArgs '--tag mklogging-builder-vs2022 --build-arg VS_VERSION=17/release'
                dir 'jenkins-windows-build-container'
                reuseNode true
              }
            }
            stages {
              stage('Build with Visual Studio Generator') {
                steps {
                  bat """
                    call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                    cmake --version
                    cmake --preset windows-x86-vs2022
                    cmake --build --preset windows-x86-vs2022-debug --parallel
                    cmake --build --preset windows-x86-vs2022-release --parallel
                    cmake --preset windows-x64-vs2022
                    cmake --build --preset windows-x64-vs2022-debug --parallel
                    cmake --build --preset windows-x64-vs2022-release --parallel
                  """
                }
                post {
                  always {
                    recordIssues(tools: [msBuild(id: 'VS2022')])
                  }
                }
              }
              stage('Build with Ninja') {
                steps {
                  bat """
                    call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                    cmake --version
                    cmake --preset win-x86-debug
                    cmake --build build\\win-x86-debug --parallel
                    cmake --preset win-x86-release
                    cmake --build build\\win-x86-release --parallel
                    cmake --preset win-x64-debug
                    cmake --build build\\win-x64-debug --parallel
                    cmake --preset win-x64-release
                    cmake --build build\\win-x64-release --parallel
                  """
                }
                post {
                  always {
                    recordIssues(tools: [cmake(id: 'VS2022-Ninja')])
                  }
                }
              }
              stage('Test') {
                steps {
                  dir('build\\windows-x86-vs2026\\bin\\Debug') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x86-vs2026\\bin\\Release') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x64-vs2026\\bin\\Debug') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x64-vs2026\\bin\\Release') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x86-debug\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x86-release\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x64-debug\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x64-release\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  junit '**/gtest-results.xml'
                  recordIssues(tools: [junitParser(id: 'VS2022-junit', pattern: '**/gtest-results.xml')])
                }
              }
            }
          }
          stage('VS2026') {
            agent {
              dockerfile {
                additionalBuildArgs '--tag mklogging-builder-vs2026 --build-arg VS_VERSION=stable' // see Dockerfile for explanation
                dir 'jenkins-windows-build-container'
                reuseNode true
              }
            }
            stages {
              stage('Build with Visual Studio Generator') {
                steps {
                  bat 'rd /s /q build' // otherwise 'build with ninja' fails
                  bat """
                    call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                    cmake --version
                    cmake --preset windows-x86-vs2026
                    cmake --build --preset windows-x86-vs2026-debug --parallel
                    cmake --build --preset windows-x86-vs2026-release --parallel
                    cmake --preset windows-x64-vs2026
                    cmake --build --preset windows-x64-vs2026-debug --parallel
                    cmake --build --preset windows-x64-vs2026-release --parallel
                  """
                }
                post {
                  always {
                    recordIssues(tools: [msBuild(id: 'VS2026')])
                  }
                }
              }
              stage('Build with Ninja') {
                steps {
                  bat """
                    call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                    cmake --version
                    cmake --preset win-x86-debug
                    cmake --build build\\win-x86-debug --parallel
                    cmake --preset win-x86-release
                    cmake --build build\\win-x86-release --parallel
                    cmake --preset win-x64-debug
                    cmake --build build\\win-x64-debug --parallel
                    cmake --preset win-x64-release
                    cmake --build build\\win-x64-release --parallel
                  """
                }
                post {
                  always {
                    recordIssues(tools: [cmake(id: 'VS2026-Ninja')])
                  }
                }
              }
              stage('Test') {
                steps {
                  dir('build\\windows-x86-vs2026\\bin\\Debug') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x86-vs2026\\bin\\Release') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x64-vs2026\\bin\\Debug') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\windows-x64-vs2026\\bin\\Release') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x86-debug\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x86-release\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x64-debug\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  dir('build\\win-x64-release\\bin') {
                    bat 'TestMKLogging.exe --gtest_output="xml:gtest-results.xml" || exit /b 0'
                  }
                  junit '**/gtest-results.xml'
                  recordIssues(tools: [junitParser(id: 'VS2026-junit', pattern: '**/gtest-results.xml')])
                }
              }
            }
          }
        }
      }
      stage('linux') {
        agent {
          dockerfile {
            dir '.devcontainer'
            label 'ubuntu && docker-engine'
          }
        }
        stages {
          stage('Build with Ninja') {
            steps {
              sh '''
                cmake --version
                cmake --preset linux-x86_64-debug
                cmake --build build/linux-x86_64-debug --parallel
                cmake --preset linux-x86_64-release
                cmake --build build/linux-x86_64-release --parallel
              '''
            }
            post {
              always {
                recordIssues(tools: [cmake(id: 'linux-ninja')])
              }
            }
          }
          stage('Test') {
            steps {
              sh '''
                cd ${WORKSPACE}/build/linux-x86_64-debug/bin
                ./TestMKLogging --gtest_output="xml:gtest-results.xml" || true
                cd ${WORKSPACE}/build/linux-x86_64-release/bin
                ./TestMKLogging --gtest_output="xml:gtest-results.xml" || true
              '''
              junit '**/gtest-results.xml'
              recordIssues(tools: [junitParser(id: 'linux-junit', pattern: '**/gtest-results.xml')])
            }
          }
        }
      }
    }
  }
}
