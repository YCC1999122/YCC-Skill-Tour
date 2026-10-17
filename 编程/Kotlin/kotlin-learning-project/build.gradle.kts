// ============================================================
// Gradle构建脚本 - Kotlin DSL
// 这是项目的主要构建配置文件
// ============================================================

// 插件配置块：声明项目需要使用的Gradle插件
plugins {
    // Kotlin JVM插件：支持Kotlin代码编译为JVM字节码
    kotlin("jvm") version "1.9.22"

    // application插件：支持将项目作为可执行应用运行
    application
}

// 项目分组（组织标识），通常是域名反转形式
group = "com.learning"

// 项目版本号
version = "1.0.0"

// 仓库配置：指定从哪里下载依赖
repositories {
    // Maven中央仓库：最主要的开源依赖仓库
    mavenCentral()
}

// 依赖配置：声明项目所需的依赖库
dependencies {
    // Kotlin标准库（由Kotlin插件自动添加，这里显式声明以便了解）
    // implementation(kotlin("stdlib"))

    // Kotlin协程核心库：提供协程的基础API
    // "org.jetbrains.kotlinx:kotlinx-coroutines-core:1.7.3"
    implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.7.3")

    // 测试依赖：JUnit 5测试框架
    testImplementation("org.junit.jupiter:junit-jupiter-api:5.10.1")
    testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.10.1")
}

// Kotlin编译配置
kotlin {
    // JVM目标版本：编译为Java 17兼容的字节码
    jvmToolchain(17)
}

// Java编译配置（可选，与Kotlin配置保持一致）
java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

// 应用程序配置：指定主类
application {
    // 主类的完整路径（包名+类名+Kt后缀，因为顶层函数会被编译到类名+Kt的类中）
    mainClass.set("com.learning.MainKt")
}

// 测试任务配置
tasks.test {
    // 使用JUnit Platform运行测试
    useJUnitPlatform()
}

// 编译任务配置
tasks.withType<org.jetbrains.kotlin.gradle.tasks.KotlinCompile> {
    // Kotlin编译器参数
    kotlinOptions {
        // JVM目标版本
        jvmTarget = "17"
        // 自由编译器参数（例如启用实验性特性）
        // freeCompilerArgs = listOf("-Xopt-in=kotlin.RequiresOptIn")
    }
}
