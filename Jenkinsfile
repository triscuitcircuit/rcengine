pipeline{
    agent any
    stages {
        stage('Build'){
            steps {
                sh 'echo "Running Cmake build script...."'
                sh 'chmod +x scripts/jenkins/jenkins-cmake.sh'
                sh 'scripts/jenkins/generate-cmake.sh'
                archiveArtifacts artifacts: 'bin/cmake-build/*', fingerprint: true
            }
        }
    }
}