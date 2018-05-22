pipeline {
  agent any
  stages {
    stage('initial_setup') {
      steps {
          sh '''cd android
          fastlane build_start_notification
          fastlane clean_old_artifacts
          fastlane save_git_log'''
      }
    }
    stage('viroreact_aar') {
      steps {
        sh '''cd android
        fastlane virorenderer_viroreact_aar'''
      }
    }
    stage('virocore_aar') {
      steps {
        sh '''cd android
        fastlane virorenderer_virocore_aar'''
      }
     }
    stage('virokit_framework (ios)') {
      steps {
        sh '''cd ios
        fastlane virorender_viroreact_virokit'''
      }
    }
    stage('releasetest') {
      steps {
        sh '''cd android
fastlane renderer_releasetest'''
      }
    }
    stage('memoryleaktest') {
      steps {
        sh '''cd android
fastlane renderer_memoryleaktest'''
      }
    }
    stage('start react-viro') {
      steps {
        build(job: 'react-viro/master', propagate: true, wait: true)
      }
    }
  }
  environment {
    LC_ALL = 'en_US.UTF-8'
    LANG = 'en_US.UTF-8'
  }
}
