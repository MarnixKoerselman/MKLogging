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
                      build\\windows-x86-vs2022\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x86-vs2022\\test-cpp17-debug.xml"
                      build\\windows-x86-vs2022\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x86-vs2022\\test-cpp20-debug.xml"

                      cmake --build --preset windows-x86-vs2022-release --parallel
                      build\\windows-x86-vs2022\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x86-vs2022\\test-cpp17-release.xml"
                      build\\windows-x86-vs2022\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x86-vs2022\\test-cpp20-release.xml"

                      cmake --preset windows-x64-vs2022

                      cmake --build --preset windows-x64-vs2022-debug --parallel
                      build\\windows-x64-vs2022\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x64-vs2022\\test-cpp17-debug.xml"
                      build\\windows-x64-vs2022\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x64-vs2022\\test-cpp20-debug.xml"

                      cmake --build --preset windows-x64-vs2022-release --parallel
                      build\\windows-x64-vs2022\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x64-vs2022\\test-cpp17-release.xml"
                      build\\windows-x64-vs2022\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x64-vs2022\\test-cpp20-release.xml"
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
                      build\\win-x86-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x86-debug\\test-cpp17.xml"
                      build\\win-x86-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x86-debug\\test-cpp20.xml"

                      cmake --preset win-x86-release
                      cmake --build build\\win-x86-release --parallel
                      build\\win-x86-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x86-release\\test-cpp17.xml"
                      build\\win-x86-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x86-release\\test-cpp20.xml"

                      cmake --preset win-x64-debug
                      cmake --build build\\win-x64-debug --parallel
                      build\\win-x64-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x64-debug\\test-cpp17.xml"
                      build\\win-x64-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x64-debug\\test-cpp20.xml"

                      cmake --preset win-x64-release
                      cmake --build build\\win-x64-release --parallel
                      build\\win-x64-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x64-release\\test-cpp17.xml"
                      build\\win-x64-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x64-release\\test-cpp20.xml"
                    """
                  }
                  post {
                    always {
                      recordIssues(tools: [cmake(id: 'VS2022-Ninja')])
                      junit 'build/**/*.xml'
                      recordIssues(tools: [junitParser(id: 'VS2022-junit', pattern: 'build/**/*.xml')])
                    }
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
                      build\\windows-x86-vs2026\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x86-vs2026\\test-cpp17-debug.xml" || exit /b 0
                      build\\windows-x86-vs2026\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x86-vs2026\\test-cpp20-debug.xml" || exit /b 0

                      cmake --build --preset windows-x86-vs2026-release --parallel
                      build\\windows-x86-vs2026\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x86-vs2026\\test-cpp17-release.xml" || exit /b 0
                      build\\windows-x86-vs2026\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x86-vs2026\\test-cpp20-release.xml" || exit /b 0

                      cmake --preset windows-x64-vs2026

                      cmake --build --preset windows-x64-vs2026-debug --parallel
                      build\\windows-x64-vs2026\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x64-vs2026\\test-cpp17-debug.xml" || exit /b 0
                      build\\windows-x64-vs2026\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x64-vs2026\\test-cpp20-debug.xml" || exit /b 0

                      cmake --build --preset windows-x64-vs2026-release --parallel
                      build\\windows-x64-vs2026\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\windows-x64-vs2026\\test-cpp17-release.xml" || exit /b 0
                      build\\windows-x64-vs2026\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\windows-x64-vs2026\\test-cpp20-release.xml" || exit /b 0
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
                      build\\win-x86-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x86-debug\\test-cpp17.xml" || exit /b 0
                      build\\win-x86-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x86-debug\\test-cpp20.xml" || exit /b 0

                      cmake --preset win-x86-release
                      cmake --build build\\win-x86-release --parallel
                      build\\win-x86-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x86-release\\test-cpp17.xml" || exit /b 0
                      build\\win-x86-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x86-release\\test-cpp20.xml" || exit /b 0

                      cmake --preset win-x64-debug
                      cmake --build build\\win-x64-debug --parallel
                      build\\win-x64-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x64-debug\\test-cpp17.xml" || exit /b 0
                      build\\win-x64-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x64-debug\\test-cpp20.xml" || exit /b 0

                      cmake --preset win-x64-release
                      cmake --build build\\win-x64-release --parallel
                      build\\win-x64-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\win-x64-release\\test-cpp17.xml" || exit /b 0
                      build\\win-x64-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\win-x64-release\\test-cpp20.xml" || exit /b 0
                    """
                  }
                  post {
                    always {
                      recordIssues(tools: [cmake(id: 'VS2026-Ninja')])
                      junit 'build/**/*.xml'
                      recordIssues(tools: [junitParser(id: 'VS2026-junit', pattern: 'build/**/*.xml')])
                    }
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
              customWorkspace 'workspace/mklogging'
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
                  build/linux-debug/bin/TestMKLogging-c++17 --gtest_output="xml:build/linux-debug/test-cpp17.xml"
                  build/linux-debug/bin/TestMKLogging-c++20 --gtest_output="xml:build/linux-debug/test-cpp20.xml"
                  build/linux-release/bin/TestMKLogging-c++17 --gtest_output="xml:build/linux-release/test-cpp17.xml"
                  build/linux-release/bin/TestMKLogging-c++20 --gtest_output="xml:build/linux-release/test-cpp20.xml"
                  build/linux-debug-coverage/bin/TestMKLogging-c++17 --gtest_output="xml:build/linux-debug-coverage/test-cpp17.xml"
                  build/linux-debug-coverage/bin/TestMKLogging-c++20 --gtest_output="xml:build/linux-debug-coverage/test-cpp20.xml"
                  cd ${WORKSPACE}/build/linux-debug-coverage
                  gcov CMakeFiles/MKLogging.dir/src/*.gcda
                  lcov --capture --directory . --output-file coverage.info --ignore-errors mismatch,mismatch --ignore-errors negative,negative --rc geninfo_unexecuted_blocks=1
                  lcov --remove coverage.info '/usr/*' '*/build/*' --output-file coverage.info
                  lcov --extract coverage.info '*/include/*' '*/src/*' '*/test-c++*' --output-file coverage.info
                  genhtml coverage.info --output-directory coverage-report --ignore-errors source
                '''
                junit 'build/**/*.xml'
                recordIssues(tools: [junitParser(id: 'linux-junit', pattern: 'build/**/*.xml')])
                publishHTML([
                  allowMissing: false,
                  alwaysLinkToLastBuild: true,
                  keepAll: true,
                  reportDir: 'build/linux-debug-coverage/coverage-report',
                  reportFiles: 'index.html',
                  reportName: 'Code Coverage Report (linux)'
                ])
              }
            }
          }
        }
      }
    }
  }
}
