pipeline {
  agent any
  stages {
    stage('checkout code') {
      steps {
        git(url: 'https://github.com/adamsigi/testbench', branch: 'main')
      }
    }

    stage('run') {
      steps {
        sh 'python3 hello_world.py'
      }
    }

  }
}