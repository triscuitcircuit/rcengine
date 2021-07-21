pipeline{
    agent any
    options{
            skipDefaultCheckout(true)
    }
    stages {
        stage('Build'){
            steps {
                cleanWs()
                checkout scm
                sh 'echo "Running Cmake build script...."'
                sh 'chmod +x scripts/jenkins/jenkins-cmake.sh'
                sh 'scripts/jenkins/jenkins-cmake.sh'
                archiveArtifacts artifacts: 'bin/cmake-build/*', fingerprint: true
            }
        }
    }
}