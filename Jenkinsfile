pipeline{
    agent any
    stages {
        stage('Build'){
            steps {
                sh 'echo "Running Cmake build script...."'
                sh 'chmod +x scripts/generate-cmake.sh'
                sh 'scripts/generate-cmake.sh'
                archiveArtifacts artifacts: 'bin/cmake-build/*', fingerprint: true
            }
        }
    }
}