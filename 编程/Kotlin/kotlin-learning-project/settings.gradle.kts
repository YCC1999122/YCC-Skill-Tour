// ============================================================
// Gradle设置脚本 - Kotlin DSL
// 这个文件用于配置Gradle项目本身，在build.gradle.kts之前执行
// ============================================================

// 项目名称：根项目的名称
rootProject.name = "kotlin-learning-project"

// 插件管理配置：指定从哪里解析Gradle插件
pluginManagement {
    // 插件仓库
    repositories {
        // Gradle插件门户：官方插件仓库
        gradlePluginPortal()
        // Maven中央仓库
        mavenCentral()
    }
}

// 依赖解析管理：统一配置所有项目的依赖仓库
dependencyResolutionManagement {
    // 仓库模式：优先使用这里配置的仓库
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    // 全局仓库配置
    repositories {
        mavenCentral()
    }
}
