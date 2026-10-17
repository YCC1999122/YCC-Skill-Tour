// go.mod 是 Go 模块的定义文件
// Go 1.11 引入了模块（Module）系统，用于管理依赖包
// 每个 Go 项目都应该有一个 go.mod 文件，位于项目根目录

// module 声明当前模块的路径（模块名）
// 这个路径是唯一的标识符，通常使用域名反转 + 项目名的格式
// 例如：github.com/username/projectname
// 这里我们使用 go-learning-project 作为本地学习项目的模块名
module go-learning-project

// go 声明当前项目使用的 Go 语言版本
// 这个版本决定了可用的语言特性和标准库版本
// Go 1.18 引入了泛型，1.21 引入了更多标准库改进
go 1.21

// require 语句列出项目直接依赖的第三方包及其版本
// 本学习项目只使用标准库，因此没有外部依赖
// 示例格式：
// require (
//     github.com/gin-gonic/gin v1.9.1
//     gorm.io/gorm v1.25.0
// )

// replace 语句用于替换依赖包的路径
// 常用于本地开发调试或修复第三方包问题
// 示例格式：
// replace github.com/xxx/yyy => ../local-path

// exclude 语句用于排除特定版本的依赖
// 示例格式：
// exclude github.com/xxx/yyy v1.0.0
