pipeline {
  agent none
  stages {
    stage('root') {
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
                    bat '''
                      ctest --test-dir build\\windows-x86-vs2022 --build-config Debug --output-on-failure --verbose --output-junit build\\windows-x86-vs2022\\test-results-debug.xml
                      ctest --test-dir build\\windows-x86-vs2022 --build-config Release --output-on-failure --verbose --output-junit build\\windows-x86-vs2022\\test-results-release.xml
                      ctest --test-dir build\\windows-x64-vs2022 --build-config Debug --output-on-failure --verbose --output-junit build\\windows-x64-vs2022\\test-results-debug.xml
                      ctest --test-dir build\\windows-x64-vs2022 --build-config Release --output-on-failure --verbose --output-junit build\\windows-x64-vs2022\\test-results-release.xml
                      ctest --test-dir build\\win-x86-debug --output-on-failure --verbose --output-junit build\\win-x86-debug\\test-results.xml
                      ctest --test-dir build\\win-x86-release --output-on-failure --verbose --output-junit build\\win-x86-release\\test-results.xml
                      ctest --test-dir build\\win-x64-debug --output-on-failure --verbose --output-junit build\\win-x64-debug\\test-results.xml
                      ctest --test-dir build\\win-x64-release --output-on-failure --verbose --output-junit build\\win-x64-release\\test-results.xml
                    '''
                    junit '**/test-results*.xml'
                    recordIssues(tools: [junitParser(id: 'VS2022-junit', pattern: '**/test-results*.xml')])
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
                    bat '''
                      ctest --test-dir build\\windows-x86-vs2026 --build-config Debug --output-on-failure --verbose --output-junit build\\windows-x86-vs2026\\test-results-debug.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\windows-x86-vs2026 --build-config Release --output-on-failure --verbose --output-junit build\\windows-x86-vs2026\\test-results-release.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\windows-x64-vs2026 --build-config Debug --output-on-failure --verbose --output-junit build\\windows-x64-vs2026\\test-results-debug.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\windows-x64-vs2026 --build-config Release --output-on-failure --verbose --output-junit build\\windows-x64-vs2026\\test-results-release.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\win-x86-debug --output-on-failure --verbose --output-junit build\\win-x86-debug\\test-results.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\win-x86-release --output-on-failure --verbose --output-junit build\\win-x86-release\\test-results.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\win-x64-debug --output-on-failure --verbose --output-junit build\\win-x64-debug\\test-results.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                      ctest --test-dir build\\win-x64-release --output-on-failure --verbose --output-junit build\\win-x64-release\\test-results.xml
                      if %ErrorLevel% equ 1 (exit /b 0)
                    '''
                    junit '**/test-results*.xml'
                    recordIssues(tools: [junitParser(id: 'VS2026-junit', pattern: '**/test-results*.xml')])
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
              label 'linux && docker-engine'
            }
          }
          stages {
            stage('Build with Ninja') {
              steps {
                sh '''
                  cmake --version
                  cmake --preset linux-debug
                  cmake --build build/linux-debug --parallel
                  cmake --preset linux-release
                  cmake --build build/linux-release --parallel
                  cmake --preset linux-relwithdebinfo
                  cmake --build build/linux-relwithdebinfo --parallel
                  cmake --preset linux-x86-debug
                  cmake --build build/linux-x86-debug --parallel
                  cmake --preset linux-x86-release
                  cmake --build build/linux-x86-release --parallel
                  cmake --preset linux-debug-coverage
                  cmake --build build/linux-debug-coverage --parallel
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
                  ctest --test-dir build/linux-debug --output-on-failure --verbose --output-junit build/linux-debug/test-results.xml
                  ctest --test-dir build/linux-release --output-on-failure --verbose --output-junit build/linux-release/test-results.xml
                  ctest --test-dir build/linux-debug-coverage --output-on-failure --verbose --output-junit build/linux-debug-coverage/test-results.xml
                  cd ${WORKSPACE}/build/linux-debug-coverage
                  gcov CMakeFiles/MKLogging.dir/src/*.gcda
                  lcov --capture --directory . --output-file coverage.info
                  genhtml coverage.info --output-directory coverage-report
                '''
                junit '**/test-results.xml'
                recordIssues(tools: [junitParser(id: 'linux-junit', pattern: '**/test-results.xml')])
                publishHTML([
                  allowMissing: false,
                  alwaysLinkToLastBuild: true,
                  keepAll: true,
                  reportDir: 'build/linux-debug-coverage/coverage-report',
                  reportFiles: 'index.html',
                  reportName: 'Code Coverage Report'
                ])
              }
            }
          }
        }
      }
    }
  }
}
