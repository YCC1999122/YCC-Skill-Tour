# FPGA Verilog 学习项目

## 项目简介

本项目是一套完整的 Verilog HDL 学习教程，从基础语法到高级设计，涵盖 FPGA 开发的核心知识点。每个 .v 文件都配有极其详细的逐行中文注释，适合零基础入门到进阶学习。

## 目录结构

```
verilog-learning-project/
├── 01_basics/                    # 第1章：基础语法
│   ├── 01_module_example.v       # 模块结构（module/endmodule、端口声明、input/output/inout）
│   ├── 02_data_types.v           # 数据类型（wire/reg/parameter、位宽、进制表示）
│   ├── 03_operators.v            # 运算符（算术/逻辑/关系/位运算/移位/拼接/缩减）
│   └── 04_assign_always.v        # 连续赋值与过程块（assign、always、阻塞/非阻塞赋值）
├── 02_combination_logic/         # 第2章：组合逻辑电路
│   ├── mux4to1.v                 # 4选1多路选择器（case语句、三元运算符、锁存器避免）
│   ├── decoder3to8.v             # 3-8译码器（译码原理、casez/casex、7段数码管译码）
│   ├── encoder8to3.v             # 8-3编码器（普通编码器、优先编码器）
│   └── adder.v                   # 加法器（半加器、全加器、行波进位、超前进位）
├── 03_sequential_logic/          # 第3章：时序逻辑电路
│   ├── register.v                # 寄存器（D触发器、同步/异步复位、寄存器堆、移位寄存器、环形/扭环）
│   ├── counter.v                 # 计数器（二进制计数器、BCD计数器、模N计数器、加减可逆）
│   └── clock_divider.v           # 时钟分频器（偶数分频、奇数分频、任意分频、50%占空比）
├── 04_state_machine/             # 第4章：状态机设计
│   ├── fsm_mealy.v               # Mealy型状态机（一段式/两段式/三段式写法、状态编码方式）
│   ├── fsm_moore.v               # Moore型状态机（与Mealy对比、1101序列检测器）
│   └── fsm_traffic_light.v       # 交通灯状态机（综合实例、多状态定时切换）
├── 05_memory_fifo/               # 第5章：存储器与FIFO
│   ├── single_port_ram.v         # 单端口RAM（同步/异步、$readmemh初始化、分布式RAM vs BRAM）
│   ├── simple_dual_port_ram.v    # 简单双端口RAM（读写独立端口、双时钟、generate块）
│   └── sync_fifo.v               # 同步FIFO（读写指针、空满标志、数据计数、FWFT模式）
└── README_Verilog.md             # 本文件
```

## 学习路径建议

### 第一阶段：基础语法（01_basics）
- 模块结构
- 数据类型
- 运算符
- 连续赋值与过程块（assign/always）
- 阻塞/非阻塞赋值

### 第二阶段：组合逻辑（02_combination_logic）
- 多路选择器
- 译码器/编码器
- 加法器
- 锁存器避免

### 第三阶段：时序逻辑（03_sequential_logic）
- D触发器
- 寄存器/移位寄存器
- 计数器
- 时钟分频器

### 第四阶段：状态机（04_state_machine）
- Mealy/Moore型状态机
- 三段式写法
- 状态编码（二进制/独热码/格雷码）
- 序列检测器
- 交通灯

### 第五阶段：存储器与FIFO（05_memory_fifo）
- 单端口/双端口RAM
- 同步FIFO
- 空满标志
- 读写指针

## 开发环境

### 1. Intel Quartus II (原Altera)
- **适用芯片**：Cyclone IV/V、MAX 10、Arria 等 Intel FPGA
- **下载地址**：https://www.intel.com/content/www/us/en/software/programmable/quartus-prime/download.html
- **版本选择**：Lite版免费，适合学习
- **推荐开发板**：EP4CE6 (Cyclone IV E)

### 2. Xilinx Vivado
- **适用芯片**：Artix-7、Kintex-7、Zynq 等 Xilinx FPGA
- **下载地址**：https://www.xilinx.com/support/download.html
- **版本选择**：Vivado ML Standard (免费WebPack许可证)
- **推荐开发板**：XC7A35T (Artix-7)

### 3. 仿真工具 ModelSim / QuestaSim
- **用途**：RTL级仿真、功能验证
- **版本**：ModelSim Altera Starter Edition（免费）
- **替代方案**：Icarus Verilog + GTKWave（完全免费开源）

### 4. 开源工具链
- **Icarus Verilog**：Verilog仿真器
- **GTKWave**：波形查看器
- **Yosys**：综合工具
- **nextpnr**：布局布线工具

## FPGA开发流程

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│  设计输入    │───▶│  功能仿真    │───▶│   综合      │───▶│  布局布线    │───▶│  下载配置    │
│ (RTL Coding)│    │(Simulation) │    │(Synthesis)  │    │(Place&Route)│    │(Download)   │
└─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘    └─────────────┘
       │                   │                   │                   │
       │                   ▼                   ▼                   ▼
       │              仿真波形            时序报告/资源        时序分析/功耗
       │              通过/失败           使用报告            报告
       ▼
    修改代码 ←───────────────────────────────────────────────────┘
```

### 详细步骤说明：

1. **设计输入（Design Entry）**
   - 使用 Verilog/VHDL 编写 RTL 代码
   - 绘制原理图（可视化方式，不推荐复杂设计）
   - 调用 IP 核（PLL、RAM、FIFO 等）

2. **功能仿真（Functional Simulation）**
   - 编写 Testbench 测试文件
   - 验证逻辑功能是否正确
   - 不考虑延迟，只验证逻辑行为
   - 工具：ModelSim、Vivado Simulator、Icarus Verilog

3. **综合（Synthesis）**
   - 将 RTL 代码转换为门级网表
   - 映射到 FPGA 内部的 LUT、FF、BRAM 等资源
   - 生成 .edf 或 .vqm 网表文件
   - 检查资源使用情况

4. **布局布线（Place & Route）**
   - 将网表中的逻辑单元映射到 FPGA 芯片的具体位置
   - 完成各单元之间的连线
   - 进行时序分析（建立时间/保持时间检查）
   - 生成比特流文件（.bit 或 .sof）

5. **下载配置（Configuration）**
   - 通过 JTAG/AS/PS 等方式将比特流下载到 FPGA
   - 上板验证功能
   - 使用 SignalTap II / ILA 进行在线逻辑分析

## 常见开发板推荐

### 入门级（100-300元）
| 开发板 | 芯片 | 资源 | 推荐指数 |
|--------|------|------|----------|
| 小梅哥AC608 | EP4CE6F17C8 | 6K LE、270Kb RAM | ★★★★★ |
| 正点原子新起点 | EP4CE10F17C8 | 10K LE、414Kb RAM | ★★★★☆ |
| 黑金AX301 | XC6SLX9 | 9K Slice | ★★★☆☆ |

### 进阶级（300-800元）
| 开发板 | 芯片 | 资源 | 推荐指数 |
|--------|------|------|----------|
| 正点原子达芬奇 | XC7A35T | 33K LE、1.8Mb BRAM | ★★★★★ |
| 米联客MZ7X | XC7Z010 (Zynq) | 28K LE + ARM Cortex-A9 | ★★★★☆ |
| 野火征途Pro | EP4CE22F17C8 | 22K LE | ★★★★☆ |

### 高级（800元以上）
| 开发板 | 芯片 | 特点 |
|--------|------|------|
| ZedBoard | XC7Z020 | Zynq 7020，ARM+FPGA |
| DE10-Nano | 5CSEMA4U23C6 | Cyclone V SoC |
|  arty A7 | XC7A35T/XC7A100T | Xilinx官方板 |

## 代码规范

### 命名规范
- **模块名**：小写+下划线，如 `uart_rx`、`fifo_sync`
- **信号名**：小写+下划线，如 `clk`、`rst_n`、`data_in`
- **参数名**：大写+下划线，如 `DATA_WIDTH`、`ADDR_DEPTH`
- **时钟信号**：以 `clk` 开头，如 `clk`、`clk_50m`、`clk_100m`
- **复位信号**：以 `rst` 开头，低有效加 `_n`，如 `rst_n`、`sys_rst_n`
- **使能信号**：以 `en` 结尾，如 `en`、`rd_en`、`wr_en`
- **有效信号**：以 `valid` 结尾，如 `valid`、`data_valid`
- **就绪信号**：以 `ready` 结尾，如 `ready`、`m_axis_tready`

### 代码风格
- 使用缩进（2个或4个空格，保持一致）
- 每行不超过80字符
- 模块端口按类型分组（input / output / inout）
- always块使用 `@(posedge clk or negedge rst_n)` 格式
- 组合逻辑使用 `always @(*)` 而非敏感信号列表
- 状态机推荐三段式写法

### 文件头注释模板
```verilog
// ============================================================
// 模块名：xxx
// 功能描述：xxx
// 输入端口：
//   clk      - 系统时钟
//   rst_n    - 复位信号，低有效
//   xxx      - xxx
// 输出端口：
//   xxx      - xxx
// 作者：Verilog学习项目
// 日期：2024-xx-xx
// ============================================================
```

## 注意事项

### 1. 组合逻辑与时序逻辑
- 组合逻辑：输出只与当前输入有关，无记忆功能
- 时序逻辑：输出与当前输入和历史状态都有关
- 组合逻辑使用 `assign` 或 `always @(*)`
- 时序逻辑使用 `always @(posedge clk)`

### 2. 避免产生锁存器（Latch）
- 组合逻辑中 if-else 必须有 else 分支
- case 语句必须有 default 分支
- 所有输出信号在每个分支中都必须赋值
- 使用 `always @(*)` 可以减少遗漏

### 3. 复位设计
- 推荐使用异步复位、同步释放的方式
- 复位信号低有效（rst_n）是行业惯例
- 避免使用过多的异步复位，影响时序收敛

### 4. 跨时钟域（CDC）
- 单比特信号：两级寄存器同步
- 多比特信号：使用握手协议或异步FIFO
- 绝对不能直接跨时钟域采样多比特数据

### 5. 时序约束
- 必须添加主时钟约束
- 约束输入输出延迟
- 约束异步时钟组
- 检查建立时间（Setup）和保持时间（Hold）违例

### 6. 资源优化
- 合理使用 RAM/ROM 资源
- 避免重复逻辑，使用参数化设计
- 流水线设计提高频率
- 资源共享降低面积

## 仿真指南

后续会添加 testbench 目录，提供各模块的仿真测试文件和详细的仿真操作指南。

## 参考资料

1. 《Verilog HDL 数字设计与综合》- Samir Palnitkar
2. 《Verilog 数字系统设计教程》- 夏宇闻
3. 《FPGA 原理及应用》- 王诚等
4. Intel Quartus II Handbook
5. Xilinx Vivado Design Suite User Guide

---

**祝学习愉快！如有问题请参考各文件中的详细注释。**
