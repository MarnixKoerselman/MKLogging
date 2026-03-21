pipeline {
  agent none
  stages {
    stage('root') {
      parallel {
        stage('VS2022 with Visual Studio Generator') {
          when {
            branch 'main'
            beforeAgent true
          }
          agent {
            dockerfile {
              additionalBuildArgs '--tag mklogging-builder-vs2022 --build-arg VS_VERSION=17/release'
              dir 'jenkins-windows-build-container'
              label 'windows && ws2022 && docker-engine'
              customWorkspace 'workspace\\mklogging'
            }
          }
          steps {
            bat """
              call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
              cmake --version

              cmake --preset vs2022-x86

              cmake --build --preset vs2022-x86-debug --parallel
              build\\vs2022-x86\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\vs2022-x86\\test-cpp17-debug.xml"
              build\\vs2022-x86\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\vs2022-x86\\test-cpp20-debug.xml"

              cmake --build --preset vs2022-x86-release --parallel
              build\\vs2022-x86\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\vs2022-x86\\test-cpp17-release.xml"
              build\\vs2022-x86\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\vs2022-x86\\test-cpp20-release.xml"
            """
          }
          post {
            always {
              junit 'build/**/*.xml'
              recordIssues(tools: [cmake(id: 'VS2022'), junitParser(id: 'VS2022-junit', pattern: 'build/**/*.xml')])
            }
          }
        }
        stage('VS2022 with Ninja') {
          when {
            branch 'main'
            beforeAgent true
          }
          agent {
            dockerfile {
              additionalBuildArgs '--tag mklogging-builder-vs2022 --build-arg VS_VERSION=17/release'
              dir 'jenkins-windows-build-container'
              label 'windows && ws2022 && docker-engine'
              customWorkspace 'workspace\\mklogging'
            }
          }
          steps {
            bat """
              call "C:\\Program Files (x86)\\Microsoft Visual Studio\\2022\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
              cmake --version

              cmake --preset ninja-msvc-x86-debug
              cmake --build build\\ninja-msvc-x86-debug --parallel
              build\\ninja-msvc-x86-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x86-debug\\test-cpp17.xml"
              build\\ninja-msvc-x86-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x86-debug\\test-cpp20.xml"

              cmake --preset ninja-msvc-x86-release
              cmake --build build\\ninja-msvc-x86-release --parallel
              build\\ninja-msvc-x86-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x86-release\\test-cpp17.xml"
              build\\ninja-msvc-x86-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x86-release\\test-cpp20.xml"

              cmake --preset ninja-msvc-x64-debug
              cmake --build build\\ninja-msvc-x64-debug --parallel
              build\\ninja-msvc-x64-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x64-debug\\test-cpp17.xml"
              build\\ninja-msvc-x64-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x64-debug\\test-cpp20.xml"

              cmake --preset ninja-msvc-x64-release
              cmake --build build\\ninja-msvc-x64-release --parallel
              build\\ninja-msvc-x64-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x64-release\\test-cpp17.xml"
              build\\ninja-msvc-x64-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x64-release\\test-cpp20.xml"
            """
          }
          post {
            always {
              junit 'build/**/*.xml'
              recordIssues(tools: [cmake(id: 'VS2022-Ninja'), junitParser(id: 'VS2022-Ninja-junit', pattern: 'build/**/*.xml')])
            }
          }
        }
        stage('VS2026 with Visual Studio Generator') {
          agent {
            dockerfile {
              additionalBuildArgs '--tag mklogging-builder-vs2026 --build-arg VS_VERSION=stable' // see Dockerfile for explanation
              dir 'jenkins-windows-build-container'
              label 'windows && ws2022 && docker-engine'
              customWorkspace 'workspace\\mklogging'
            }
          }
          when {
            branch 'main'
            beforeAgent true
          }
          steps {
            bat """
              call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
              cmake --version

              cmake --preset vs2026-x64

              cmake --build --preset vs2026-x64-debug --parallel
              build\\vs2026-x64\\bin\\Debug\\TestMKLogging-c++17.exe --gtest_output="xml:build\\vs2026-x64\\test-cpp17-debug.xml" || exit /b 0
              build\\vs2026-x64\\bin\\Debug\\TestMKLogging-c++20.exe --gtest_output="xml:build\\vs2026-x64\\test-cpp20-debug.xml" || exit /b 0

              cmake --build --preset vs2026-x64-release --parallel
              build\\vs2026-x64\\bin\\Release\\TestMKLogging-c++17.exe --gtest_output="xml:build\\vs2026-x64\\test-cpp17-release.xml" || exit /b 0
              build\\vs2026-x64\\bin\\Release\\TestMKLogging-c++20.exe --gtest_output="xml:build\\vs2026-x64\\test-cpp20-release.xml" || exit /b 0
            """
          }
          post {
            always {
              junit 'build/**/*.xml'
              recordIssues(tools: [cmake(id: 'VS2026'), junitParser(id: 'VS2026-junit', pattern: 'build/**/*.xml')])
            }
          }

        }
        stage('VS2026 with Ninja') {
          agent {
            dockerfile {
              additionalBuildArgs '--tag mklogging-builder-vs2026 --build-arg VS_VERSION=stable' // see Dockerfile for explanation
              dir 'jenkins-windows-build-container'
              label 'windows && ws2022 && docker-engine'
              customWorkspace 'workspace\\mklogging'
            }
          }
          stages {
            stage('Build x86') {
              steps {
                bat """
                  call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                  cmake --version

                  cmake --preset ninja-msvc-x86-debug
                  cmake --build build\\ninja-msvc-x86-debug --parallel

                  cmake --preset ninja-msvc-x86-release
                  cmake --build build\\ninja-msvc-x86-release --parallel
                """
              }
              post {
                always {
                  recordIssues(tools: [cmake(id: 'VS2026-Ninja-x86')])
                }
              }
            }
            stage('Build x64') {
              steps {
                bat """
                  call "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\Common7\\Tools\\VsDevCmd.bat"
                  cmake --version

                  cmake --preset ninja-msvc-x64-debug
                  cmake --build build\\ninja-msvc-x64-debug --parallel

                  cmake --preset ninja-msvc-x64-release
                  cmake --build build\\ninja-msvc-x64-release --parallel
                """
              }
              post {
                always {
                  recordIssues(tools: [cmake(id: 'VS2026-Ninja-x64')])
                }
              }
            }
            stage('Test') {
              steps {
                bat """
                  build\\ninja-msvc-x86-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x86-debug\\test-cpp17.xml" || exit /b 0
                  build\\ninja-msvc-x86-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x86-debug\\test-cpp20.xml" || exit /b 0

                  build\\ninja-msvc-x86-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x86-release\\test-cpp17.xml" || exit /b 0
                  build\\ninja-msvc-x86-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x86-release\\test-cpp20.xml" || exit /b 0

                  build\\ninja-msvc-x64-debug\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x64-debug\\test-cpp17.xml" || exit /b 0
                  build\\ninja-msvc-x64-debug\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x64-debug\\test-cpp20.xml" || exit /b 0

                  build\\ninja-msvc-x64-release\\bin\\TestMKLogging-c++17.exe --gtest_output="xml:build\\ninja-msvc-x64-release\\test-cpp17.xml" || exit /b 0
                  build\\ninja-msvc-x64-release\\bin\\TestMKLogging-c++20.exe --gtest_output="xml:build\\ninja-msvc-x64-release\\test-cpp20.xml" || exit /b 0
                """
              }
              post {
                always {
                  junit 'build/**/*.xml'
                  recordIssues(tools: [junitParser(id: 'VS2026-Ninja-junit', pattern: 'build/**/*.xml')])
                }
              }
            }
          }
        }
        stage('linux') {
          agent {
            dockerfile {
              additionalBuildArgs '--tag mklogging-builder-linux'
              dir '.devcontainer'
              label 'linux && docker-engine'
              customWorkspace 'workspace/mklogging'
            }
          }
          stages {
            stage('Build x86') {
              steps {
                sh '''
                  cmake --version
                  cmake --preset linux-x86-debug
                  cmake --build build/linux-x86-debug --parallel
                  cmake --preset linux-x86-release
                  cmake --build build/linux-x86-release --parallel
                '''
              }
              post {
                always {
                  recordIssues(tools: [cmake(id: 'linux-ninja-x86')])
                }
              }
            }
            stage('Build x64') {
              steps {
                sh '''
                  cmake --version
                  cmake --preset linux-debug
                  cmake --build build/linux-debug --parallel
                  cmake --preset linux-release
                  cmake --build build/linux-release --parallel
                  cmake --preset linux-relwithdebinfo
                  cmake --build build/linux-relwithdebinfo --parallel
                  cmake --preset linux-debug-coverage
                  cmake --build build/linux-debug-coverage --parallel
                '''
              }
              post {
                always {
                  recordIssues(tools: [cmake(id: 'linux-ninja-x64')])
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
