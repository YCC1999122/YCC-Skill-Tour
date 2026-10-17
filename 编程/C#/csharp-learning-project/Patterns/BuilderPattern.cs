// ============================================================
// BuilderPattern.cs - 建造者模式
// 将一个复杂对象的构建与它的表示分离
// 使得同样的构建过程可以创建不同的表示
// 适用场景：创建复杂对象，对象有很多可选参数，需要分步骤构建
// ============================================================

namespace CSharpLearning.Patterns;

/// <summary>
/// 产品：电脑
/// 这是要构建的复杂对象，有很多部件
/// </summary>
public class Computer
{
    /// <summary>
    /// CPU
    /// </summary>
    public string CPU { get; set; } = string.Empty;

    /// <summary>
    /// 内存
    /// </summary>
    public string RAM { get; set; } = string.Empty;

    /// <summary>
    /// 硬盘
    /// </summary>
    public string HardDrive { get; set; } = string.Empty;

    /// <summary>
    /// 显卡
    /// </summary>
    public string GraphicsCard { get; set; } = string.Empty;

    /// <summary>
    /// 显示器
    /// </summary>
    public string Monitor { get; set; } = string.Empty;

    /// <summary>
    /// 键盘
    /// </summary>
    public string Keyboard { get; set; } = string.Empty;

    /// <summary>
    /// 鼠标
    /// </summary>
    public string Mouse { get; set; } = string.Empty;

    /// <summary>
    /// 是否包含操作系统
    /// </summary>
    public bool HasOS { get; set; }

    /// <summary>
    /// 是否包含办公软件
    /// </summary>
    public bool HasOffice { get; set; }

    /// <summary>
    /// 显示电脑配置
    /// </summary>
    public void ShowConfig()
    {
        Console.WriteLine("    电脑配置：");
        Console.WriteLine($"    CPU：{CPU}");
        Console.WriteLine($"    内存：{RAM}");
        Console.WriteLine($"    硬盘：{HardDrive}");
        Console.WriteLine($"    显卡：{GraphicsCard}");
        Console.WriteLine($"    显示器：{Monitor}");
        Console.WriteLine($"    键盘：{Keyboard}");
        Console.WriteLine($"    鼠标：{Mouse}");
        Console.WriteLine($"    操作系统：{(HasOS ? "已安装" : "无")}");
        Console.WriteLine($"    办公软件：{(HasOffice ? "已安装" : "无")}");
        Console.WriteLine($"    总价：{CalculatePrice():C}");
    }

    /// <summary>
    /// 计算总价（简化版）
    /// </summary>
    private decimal CalculatePrice()
    {
        decimal price = 0;
        if (CPU.Contains("i9")) price += 4000;
        else if (CPU.Contains("i7")) price += 2500;
        else if (CPU.Contains("i5")) price += 1500;

        if (RAM.Contains("32")) price += 1200;
        else if (RAM.Contains("16")) price += 600;
        else if (RAM.Contains("8")) price += 300;

        if (HardDrive.Contains("1TB")) price += 800;
        else if (HardDrive.Contains("512")) price += 400;

        if (GraphicsCard.Contains("3090")) price += 15000;
        else if (GraphicsCard.Contains("3060")) price += 3000;

        if (HasOS) price += 1000;
        if (HasOffice) price += 800;

        return price;
    }
}

/// <summary>
/// 抽象建造者接口
/// 定义创建产品各个部件的抽象步骤
/// </summary>
public interface IComputerBuilder
{
    /// <summary>
    /// 构建 CPU
    /// </summary>
    IComputerBuilder BuildCPU();

    /// <summary>
    /// 构建内存
    /// </summary>
    IComputerBuilder BuildRAM();

    /// <summary>
    /// 构建硬盘
    /// </summary>
    IComputerBuilder BuildHardDrive();

    /// <summary>
    /// 构建显卡
    /// </summary>
    IComputerBuilder BuildGraphicsCard();

    /// <summary>
    /// 构建显示器
    /// </summary>
    IComputerBuilder BuildMonitor();

    /// <summary>
    /// 构建外设（键盘鼠标）
    /// </summary>
    IComputerBuilder BuildPeripherals();

    /// <summary>
    /// 安装软件
    /// </summary>
    IComputerBuilder InstallSoftware();

    /// <summary>
    /// 获取构建完成的电脑
    /// </summary>
    Computer GetComputer();
}

/// <summary>
/// 具体建造者：游戏本建造者
/// 构建高性能的游戏电脑
/// </summary>
public class GamingComputerBuilder : IComputerBuilder
{
    private Computer _computer = new Computer();

    public IComputerBuilder BuildCPU()
    {
        _computer.CPU = "Intel Core i9-13900K";
        return this;  // 返回 this，支持链式调用
    }

    public IComputerBuilder BuildRAM()
    {
        _computer.RAM = "32GB DDR5";
        return this;
    }

    public IComputerBuilder BuildHardDrive()
    {
        _computer.HardDrive = "1TB NVMe SSD";
        return this;
    }

    public IComputerBuilder BuildGraphicsCard()
    {
        _computer.GraphicsCard = "NVIDIA RTX 4090";
        return this;
    }

    public IComputerBuilder BuildMonitor()
    {
        _computer.Monitor = "27英寸 2K 240Hz 电竞屏";
        return this;
    }

    public IComputerBuilder BuildPeripherals()
    {
        _computer.Keyboard = "机械键盘（红轴）";
        _computer.Mouse = "游戏鼠标";
        return this;
    }

    public IComputerBuilder InstallSoftware()
    {
        _computer.HasOS = true;
        _computer.HasOffice = false;  // 游戏本不需要办公软件
        return this;
    }

    public Computer GetComputer()
    {
        return _computer;
    }
}

/// <summary>
/// 具体建造者：办公本建造者
/// 构建适合办公的电脑
/// </summary>
public class OfficeComputerBuilder : IComputerBuilder
{
    private Computer _computer = new Computer();

    public IComputerBuilder BuildCPU()
    {
        _computer.CPU = "Intel Core i5-13400";
        return this;
    }

    public IComputerBuilder BuildRAM()
    {
        _computer.RAM = "16GB DDR4";
        return this;
    }

    public IComputerBuilder BuildHardDrive()
    {
        _computer.HardDrive = "512GB SSD";
        return this;
    }

    public IComputerBuilder BuildGraphicsCard()
    {
        _computer.GraphicsCard = "集成显卡";
        return this;
    }

    public IComputerBuilder BuildMonitor()
    {
        _computer.Monitor = "24英寸 1080P 办公屏";
        return this;
    }

    public IComputerBuilder BuildPeripherals()
    {
        _computer.Keyboard = "普通薄膜键盘";
        _computer.Mouse = "普通鼠标";
        return this;
    }

    public IComputerBuilder InstallSoftware()
    {
        _computer.HasOS = true;
        _computer.HasOffice = true;  // 办公本需要办公软件
        return this;
    }

    public Computer GetComputer()
    {
        return _computer;
    }
}

/// <summary>
/// 指挥者（Director）
/// 负责安排建造的步骤顺序
/// 客户端只需要和指挥者打交道，不需要知道具体建造过程
/// </summary>
public class ComputerDirector
{
    /// <summary>
    /// 构造完整的电脑
    /// 按固定的步骤顺序构建
    /// </summary>
    public Computer Construct(IComputerBuilder builder)
    {
        return builder
            .BuildCPU()
            .BuildRAM()
            .BuildHardDrive()
            .BuildGraphicsCard()
            .BuildMonitor()
            .BuildPeripherals()
            .InstallSoftware()
            .GetComputer();
    }

    /// <summary>
    /// 构造只有主机的电脑（不带外设）
    /// </summary>
    public Computer ConstructHostOnly(IComputerBuilder builder)
    {
        return builder
            .BuildCPU()
            .BuildRAM()
            .BuildHardDrive()
            .BuildGraphicsCard()
            .InstallSoftware()
            .GetComputer();
    }
}

// ============================================================
// 建造者模式的另一种常见形式：Fluent Builder
// 使用 Fluent 接口风格，更灵活，不需要 Director
// 常用于配置对象、DTO 构建等场景
// ============================================================

/// <summary>
/// Fluent 风格的建造者：邮件建造者
/// 支持链式调用，灵活配置邮件的各个部分
/// </summary>
public class EmailBuilder
{
    private Email _email = new Email();

    /// <summary>
    /// 设置收件人
    /// </summary>
    public EmailBuilder To(string to)
    {
        _email.To = to;
        return this;
    }

    /// <summary>
    /// 设置发件人
    /// </summary>
    public EmailBuilder From(string from)
    {
        _email.From = from;
        return this;
    }

    /// <summary>
    /// 设置主题
    /// </summary>
    public EmailBuilder Subject(string subject)
    {
        _email.Subject = subject;
        return this;
    }

    /// <summary>
    /// 设置正文
    /// </summary>
    public EmailBuilder Body(string body)
    {
        _email.Body = body;
        return this;
    }

    /// <summary>
    /// 添加附件
    /// </summary>
    public EmailBuilder AddAttachment(string attachment)
    {
        _email.Attachments.Add(attachment);
        return this;
    }

    /// <summary>
    /// 设置是否为HTML邮件
    /// </summary>
    public EmailBuilder IsHtml(bool isHtml)
    {
        _email.IsHtml = isHtml;
        return this;
    }

    /// <summary>
    /// 构建完成，返回邮件对象
    /// </summary>
    public Email Build()
    {
        // 可以在这里做一些验证
        if (string.IsNullOrWhiteSpace(_email.To))
            throw new InvalidOperationException("收件人不能为空");
        if (string.IsNullOrWhiteSpace(_email.Subject))
            throw new InvalidOperationException("邮件主题不能为空");

        return _email;
    }
}

/// <summary>
/// 产品：邮件
/// </summary>
public class Email
{
    public string To { get; set; } = string.Empty;
    public string From { get; set; } = string.Empty;
    public string Subject { get; set; } = string.Empty;
    public string Body { get; set; } = string.Empty;
    public List<string> Attachments { get; set; } = new List<string>();
    public bool IsHtml { get; set; }

    public void ShowInfo()
    {
        Console.WriteLine($"    发件人：{From}");
        Console.WriteLine($"    收件人：{To}");
        Console.WriteLine($"    主题：{Subject}");
        Console.WriteLine($"    正文：{Body}");
        Console.WriteLine($"    附件数：{Attachments.Count}");
        Console.WriteLine($"    格式：{(IsHtml ? "HTML" : "纯文本")}");
    }
}
