// ============================================================
// 包声明
// ============================================================
package com.learning.collections

// ============================================================
// 集合与序列演示
// 包含：只读集合 vs 可变集合、集合工厂函数、集合操作、序列、
//      性能对比、区间、数组与集合的转换
// ============================================================

// ============================================================
// 1. 只读集合 vs 可变集合
// Kotlin的集合框架明确区分了只读和可变集合
// 只读：List, Set, Map（接口，不能修改）
// 可变：MutableList, MutableSet, MutableMap（可以增删改）
// ============================================================
fun collectionTypesDemo() {
    println("=== 1. 只读集合 vs 可变集合 ===")

    // --- List（只读列表） ---
    val readOnlyList: List<Int> = listOf(1, 2, 3, 4, 5)
    println("只读List：$readOnlyList")
    println("List大小：${readOnlyList.size}")
    println("第一个元素：${readOnlyList[0]}")
    // readOnlyList.add(6)  // 错误：只读List不能添加元素

    // --- MutableList（可变列表） ---
    val mutableList: MutableList<Int> = mutableListOf(1, 2, 3)
    println("可变List（原始）：$mutableList")
    mutableList.add(4)                    // 添加元素
    mutableList.add(0, 0)            // 在指定位置添加
    mutableList.removeAt(1)              // 删除指定位置元素
    mutableList[2] = 100                 // 修改元素
    println("可变List（修改后）：$mutableList")

    // --- Set（只读集合，元素不重复，无序） ---
    val readOnlySet: Set<String> = setOf("苹果", "香蕉", "橙子", "苹果")
    println("只读Set：$readOnlySet（自动去重）")
    println("Set包含'香蕉'吗：${"香蕉" in readOnlySet}")

    // --- MutableSet（可变集合） ---
    val mutableSet: MutableSet<String> = mutableSetOf("a", "b", "c")
    println("可变Set（原始）：$mutableSet")
    mutableSet.add("d")
    mutableSet.remove("a")
    println("可变Set（修改后）：$mutableSet")

    // --- Map（只读映射，键值对） ---
    val readOnlyMap: Map<String, Int> = mapOf(
        "张三" to 90,
        "李四" to 85,
        "王五" to 95
    )
    println("只读Map：$readOnlyMap")
    println("张三的分数：${readOnlyMap["张三"]}")
    println("所有键：${readOnlyMap.keys}")
    println("所有值：${readOnlyMap.values}")

    // --- MutableMap（可变映射） ---
    val mutableMap: MutableMap<String, Int> = mutableMapOf(
        "语文" to 80,
        "数学" to 90
    )
    println("可变Map（原始）：$mutableMap")
    mutableMap["英语"] = 85              // 添加/修改
    mutableMap.put("物理", 95)            // put方法
    mutableMap.remove("语文")             // 删除
    println("可变Map（修改后）：$mutableMap")

    // 将只读集合转为可变集合
    val newMutable = readOnlyList.toMutableList()
    newMutable.add(6)
    println("只读转可变：$newMutable")

    // 将可变集合转为只读（实际上是返回一个只读视图）
    val readOnlyView: List<Int> = mutableList
    // readOnlyView.add(6)  // 编译错误：只读视图不能修改
}

// ============================================================
// 2. 集合工厂函数
// ============================================================
fun factoryFunctionsDemo() {
    println("\n=== 2. 集合工厂函数 ===")

    // listOf：创建只读List
    val list1 = listOf(1, 2, 3)
    println("listOf：$list1")

    // mutableListOf：创建可变List
    val list2 = mutableListOf("a", "b", "c")
    println("mutableListOf：$list2")

    // arrayListOf：创建ArrayList
    val list3 = arrayListOf(1.1, 2.2, 3.3)
    println("arrayListOf：$list3")

    // setOf：创建只读Set
    val set1 = setOf(1, 2, 3, 1)
    println("setOf：$set1")

    // mutableSetOf：创建可变Set
    val set2 = mutableSetOf("x", "y")
    println("mutableSetOf：$set2")

    // hashSetOf：创建HashSet
    val set3 = hashSetOf(1, 2, 3)
    println("hashSetOf：$set3")

    // linkedSetOf：创建LinkedHashSet（有序）
    val set4 = linkedSetOf("first", "second", "third")
    println("linkedSetOf：$set4")

    // sortedSetOf：创建TreeSet（排序）
    val set5 = sortedSetOf(3, 1, 2, 5, 4)
    println("sortedSetOf：$set5")

    // mapOf：创建只读Map
    val map1 = mapOf("key1" to 1, "key2" to 2)
    println("mapOf：$map1")

    // mutableMapOf：创建可变Map
    val map2 = mutableMapOf("a" to 1, "b" to 2)
    println("mutableMapOf：$map2")

    // hashMapOf：创建HashMap
    val map3 = hashMapOf("x" to 10, "y" to 20)
    println("hashMapOf：$map3")

    // linkedMapOf：创建LinkedHashMap（有序）
    val map4 = linkedMapOf("first" to 1, "second" to 2)
    println("linkedMapOf：$map4")

    // sortedMapOf：创建TreeMap（按键排序）
    val map5 = sortedMapOf("c" to 3, "a" to 1, "b" to 2)
    println("sortedMapOf：$map5")

    // List构造函数：根据索引生成元素
    val generatedList = List(5) { index -> index * index }
    println("List构造函数（平方数）：$generatedList")

    // MutableList构造函数
    val generatedMutable = MutableList(5) { it * 2 }
    println("MutableList构造函数（偶数）：$generatedMutable")

    // 空集合
    val emptyList = emptyList<String>()
    val emptySet = emptySet<Int>()
    val emptyMap = emptyMap<String, Int>()
    println("空集合：emptyList=$emptyList, emptySet=$emptySet, emptyMap=$emptyMap")
}

// ============================================================
// 3. 集合操作（最常用的操作符）
// ============================================================
fun collectionOperationsDemo() {
    println("\n=== 3. 集合操作 ===")

    val numbers = listOf(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    println("原始数据：$numbers")

    // --- 过滤操作 ---
    println("\n--- 过滤操作 ---")

    // filter：保留满足条件的元素
    val evens = numbers.filter { it % 2 == 0 }
    println("filter（偶数）：$evens")

    // filterNot：保留不满足条件的元素
    val odds = numbers.filterNot { it % 2 == 0 }
    println("filterNot（奇数）：$odds")

    // filterNotNull：过滤掉null
    val nullableList = listOf(1, null, 3, null, 5)
    val nonNulls = nullableList.filterNotNull()
    println("filterNotNull：$nonNulls")

    // filterIndexed：带索引的过滤
    val indexFiltered = numbers.filterIndexed { index, value ->
        index % 2 == 0 && value > 3  // 索引为偶数且值大于3
    }
    println("filterIndexed：$indexFiltered")

    // --- 转换操作 ---
    println("\n--- 转换操作 ---")

    // map：将每个元素转换为新值
    val doubled = numbers.map { it * 2 }
    println("map（翻倍）：$doubled")

    // mapIndexed：带索引的转换
    val withIndex = numbers.mapIndexed { index, value ->
        "[$index:$value]"
    }
    println("mapIndexed：$withIndex")

    // mapNotNull：转换后过滤null
    val strNumbers = listOf("1", "2", "three", "4", "five")
    val parsedInts = strNumbers.mapNotNull { it.toIntOrNull() }
    println("mapNotNull：$parsedInts")

    // flatMap：将每个元素转换为集合，然后扁平化
    val nestedList = listOf(listOf(1, 2), listOf(3, 4), listOf(5, 6))
    val flattened = nestedList.flatMap { it }
    println("flatMap（扁平化）：$flattened")

    val words = listOf("hello", "world")
    val chars = words.flatMap { it.toList() }
    println("flatMap（字符列表）：$chars")

    // flatten：直接扁平化嵌套集合
    val flat = nestedList.flatten()
    println("flatten：$flat")

    // --- 聚合操作 ---
    println("\n--- 聚合操作 ---")

    // sum：求和
    println("sum：${numbers.sum()}")

    // average：平均值
    println("average：${numbers.average()}")

    // max/min：最大最小值
    println("max：${numbers.maxOrNull()}")
    println("min：${numbers.minOrNull()}")

    // count：计数
    println("count（大于5的数量）：${numbers.count { it > 5 }}")

    // fold：从初始值开始累积
    // 第一个参数是初始值，第二个参数是累积函数
    val sumFold = numbers.fold(0) { accumulator, element ->
        accumulator + element
    }
    println("fold（求和）：$sumFold")

    // foldRight：从右向左累积
    val concatFold = numbers.foldRight("") { element, acc ->
        acc + element
    }
    println("foldRight（拼接字符串）：$concatFold")

    // reduce：没有初始值，第一个元素作为初始值
    val productReduce = numbers.reduce { acc, element ->
        acc * element
    }
    println("reduce（累乘）：$productReduce")

    // --- 分组操作 ---
    println("\n--- 分组操作 ---")

    // groupBy：按条件分组，返回Map
    val grouped = numbers.groupBy {
        if (it % 2 == 0) "偶数" else "奇数"
    }
    println("groupBy（奇偶分组）：$grouped")

    val wordsList = listOf("apple", "banana", "cherry", "date", "blueberry")
    val byFirstLetter = wordsList.groupBy { it.first() }
    println("groupBy（首字母分组）：$byFirstLetter")

    // --- 去重操作 ---
    println("\n--- 去重操作 ---")

    val duplicates = listOf(1, 2, 2, 3, 3, 3, 4, 5, 5)
    val distinct = duplicates.distinct()
    println("distinct（去重）：$distinct")

    // distinctBy：按条件去重
    val people = listOf("张三" to 25, "李四" to 30, "王五" to 25, "赵六" to 30)
    val distinctByAge = people.distinctBy { it.second }
    println("distinctBy（按年龄去重）：$distinctByAge")

    // --- 排序操作 ---
    println("\n--- 排序操作 ---")

    val unsorted = listOf(5, 2, 8, 1, 9, 3)
    println("未排序：$unsorted")

    // sorted：升序排序
    println("sorted（升序）：${unsorted.sorted()}")

    // sortedDescending：降序排序
    println("sortedDescending（降序）：${unsorted.sortedDescending()}")

    // sortedBy：按指定条件排序
    val strList = listOf("banana", "apple", "cherry", "date")
    println("sortedBy（按长度排序）：${strList.sortedBy { it.length }}")

    // sortedByDescending：按指定条件降序
    println("sortedByDescending（按长度降序）：${strList.sortedByDescending { it.length }}")

    // --- 取元素操作 ---
    println("\n--- 取元素操作 ---")

    println("first：${numbers.first()}")           // 第一个元素
    println("last：${numbers.last()}")             // 最后一个元素
    println("firstOrNull：${emptyList<Int>().firstOrNull()}")  // 空集合返回null
    println("take(3)：${numbers.take(3)}")        // 取前3个
    println("takeLast(3)：${numbers.takeLast(3)}")  // 取后3个
    println("drop(3)：${numbers.drop(3)}")        // 去掉前3个
    println("dropLast(3)：${numbers.dropLast(3)}")  // 去掉后3个
    println("singleOrNull（只有一个元素满足）：${listOf(5).singleOrNull()}")

    // --- 检查操作 ---
    println("\n--- 检查操作 ---")

    println("any（有偶数吗）：${numbers.any { it % 2 == 0 }}")
    println("all（都大于0吗）：${numbers.all { it > 0 }}")
    println("none（都小于0吗）：${numbers.none { it < 0 }}")
    println("contains（包含5吗）：${5 in numbers}")

    // --- 切片操作 ---
    println("\n--- 切片操作 ---")
    println("slice(2..5)：${numbers.slice(2..5)}")
    println("slice(listOf(0,2,4))：${numbers.slice(listOf(0, 2, 4))}")
}

// ============================================================
// 4. 序列 Sequence（惰性求值）
// Sequence是惰性集合，只有在需要结果时才执行操作
// 适合处理大数据量或链式操作，避免创建中间集合
// ============================================================
fun sequenceDemo() {
    println("\n=== 4. 序列 Sequence ===")

    // --- 创建序列 ---

    // asSequence：将集合转为序列
    val list = listOf(1, 2, 3, 4, 5)
    val sequence1 = list.asSequence()
    println("asSequence：${sequence1.toList()}")

    // sequenceOf：直接创建序列
    val sequence2 = sequenceOf("a", "b", "c")
    println("sequenceOf：${sequence2.toList()}")

    // generateSequence：生成无限序列（惰性）
    // 第一个参数是初始值，第二个参数是生成下一个值的函数
    val naturalNumbers = generateSequence(1) { it + 1 }  // 自然数序列（无限）
    // 必须用take限制数量，否则会无限循环
    println("generateSequence（前10个自然数）：${naturalNumbers.take(10).toList()}")

    // 斐波那契序列
    val fibonacci = generateSequence(Pair(0, 1)) { Pair(it.second, it.first + it.second) }
        .map { it.first }
    println("generateSequence（前10个斐波那契数）：${fibonacci.take(10).toList()}")

    // --- 序列操作（惰性求值） ---
    println("\n--- 序列的惰性求值特性 ---")

    // 对比：集合操作是急切的（每一步都创建新集合）
    // 序列操作是惰性的（只有在终端操作时才执行，且每个元素经历所有操作后才处理下一个）

    val numbers = (1..1000000).toList()

    // 集合操作（创建中间集合，可能占用较多内存）
    // 先filter创建新集合，再map创建新集合
    val resultList = numbers
        .filter { it % 2 == 0 }
        .map { it * 2 }
        .take(10)
    println("集合操作结果：$resultList")

    // 序列操作（惰性，不需要创建中间集合）
    // 每个元素依次经过filter -> map -> take，找到10个就停止
    val resultSeq = numbers.asSequence()
        .filter { it % 2 == 0 }
        .map { it * 2 }
        .take(10)
        .toList()  // 终端操作：触发计算
    println("序列操作结果：$resultSeq")

    // --- 序列的终端操作 ---
    // 以下操作会触发序列的实际计算
    println("\n--- 序列的终端操作 ---")

    val seq = sequenceOf(1, 2, 3, 4, 5)

    println("toList：${seq.toList()}")       // 转为List
    println("toSet：${seq.toSet()}")         // 转为Set
    println("sum：${seq.sum()}")              // 求和
    println("count：${seq.count()}")          // 计数
    println("first：${seq.first()}")          // 第一个元素
    println("fold：${seq.fold(0) { acc, e -> acc + e }}")  // fold

    // --- yield和sequence函数 ---
    // 使用sequence { ... } 构建器，可以用yield产生值
    val customSequence = sequence {
        yield(1)        // 产生一个值
        yieldAll(listOf(2, 3, 4))  // 产生多个值
        yield(5)
        yieldAll(generateSequence(6) { it + 1 }.take(3))  // 产生序列
    }
    println("sequence构建器：${customSequence.toList()}")
}

// ============================================================
// 5. 集合与序列性能对比
// ============================================================
fun performanceComparisonDemo() {
    println("\n=== 5. 集合与序列性能对比 ===")

    // 生成一个大列表
    val size = 100000
    val bigList = (1..size).toList()

    // --- 集合操作性能 ---
    val listStartTime = System.nanoTime()
    val listResult = bigList
        .filter { it % 2 == 0 }      // 创建中间集合
        .map { it * 2 }              // 再创建中间集合
        .take(10)
    val listEndTime = System.nanoTime()
    println("集合操作耗时：${(listEndTime - listStartTime) / 1000} 微秒，结果：$listResult")

    // --- 序列操作性能 ---
    val seqStartTime = System.nanoTime()
    val seqResult = bigList.asSequence()
        .filter { it % 2 == 0 }      // 惰性，不创建中间集合
        .map { it * 2 }              // 惰性
        .take(10)                    // 找到10个就停止
        .toList()
    val seqEndTime = System.nanoTime()
    println("序列操作耗时：${(seqEndTime - seqStartTime) / 1000} 微秒，结果：$seqResult")

    // 注意：性能对比结果取决于数据量和操作复杂度
    // - 小数据量：集合可能更快（序列有额外开销）
    // - 大数据量+链式操作+短路操作（如take）：序列优势明显
    println("\n性能说明：")
    println("- 序列是惰性求值，适合大数据量、链式操作、有短路操作（如take/first）的场景")
    println("- 小数据量时，集合操作可能更快（序列有创建和迭代的额外开销）")
}

// ============================================================
// 6. 区间 Range 和 数列 Progression
// ============================================================
fun rangeDemo() {
    println("\n=== 6. 区间 Range 和 数列 Progression ===")

    // --- IntRange（整数区间） ---
    val intRange: IntRange = 1..10        // 闭区间 [1, 10]
    println("IntRange 1..10：${intRange.toList()}")
    println("5 in 1..10：${5 in intRange}")

    // --- 半开区间：until ---
    val halfOpen = 1 until 10              // [1, 10)
    println("1 until 10：${halfOpen.toList()}")

    // --- 倒序区间：downTo ---
    val downRange = 10 downTo 1            // 从10到1
    println("10 downTo 1：${downRange.toList()}")

    // --- 步长：step ---
    val stepRange = 1..10 step 2           // 步长为2
    println("1..10 step 2：${stepRange.toList()}")

    // 倒序+步长
    val downStep = 10 downTo 1 step 3
    println("10 downTo 1 step 3：${downStep.toList()}")

    // --- 字符区间 ---
    val charRange = 'a'..'e'
    println("字符区间 a..e：${charRange.toList()}")

    // --- LongRange ---
    val longRange = 1L..5L
    println("LongRange：${longRange.toList()}")

    // --- 区间操作 ---
    println("\n区间操作：")
    println("first：${intRange.first}")
    println("last：${intRange.last}")
    println("step：${stepRange.step}")

    // 使用区间遍历
    print("for循环遍历区间：")
    for (i in 1..5) {
        print("$i ")
    }
    println()

    // when表达式中使用区间
    val score = 85
    val grade = when (score) {
        in 90..100 -> "优秀"
        in 80 until 90 -> "良好"
        in 60 until 80 -> "及格"
        !in 0..100 -> "无效"
        else -> "不及格"
    }
    println("分数$score 等级：$grade")
}

// ============================================================
// 7. 数组与集合的转换
// ============================================================
fun arrayCollectionConversionDemo() {
    println("\n=== 7. 数组与集合的转换 ===")

    // 数组转集合
    val intArray = intArrayOf(1, 2, 3, 4, 5)
    val listFromArray = intArray.toList()           // 转只读List
    val mutableListFromArray = intArray.toMutableList()  // 转可变List
    val setFromArray = intArray.toSet()             // 转Set
    println("intArrayOf -> List：$listFromArray")
    println("intArrayOf -> MutableList：$mutableListFromArray")
    println("intArrayOf -> Set：$setFromArray")

    // 集合转数组
    val list = listOf(1, 2, 3, 4, 5)
    val intArrayFromList = list.toIntArray()        // 转IntArray
    val typedArray = list.toTypedArray()            // 转Array<Int>
    println("List -> IntArray：${intArrayFromList.contentToString()}")
    println("List -> Array<Int>：${typedArray.contentToString()}")

    // 不同类型数组的转换
    val strList = listOf("a", "b", "c")
    val strArray = strList.toTypedArray()
    println("String List -> Array：${strArray.contentToString()}")

    // 集合之间的转换
    val set = list.toSet()
    val listFromSet = set.toList()
    println("List -> Set -> List：$listFromSet")

    // Map与List的转换
    val map = mapOf("a" to 1, "b" to 2, "c" to 3)
    val entryList = map.toList()                    // Map转List<Pair>
    println("Map -> List：$entryList")

    val pairList = listOf("x" to 10, "y" to 20)
    val mapFromPairs = pairList.toMap()             // List<Pair>转Map
    println("List<Pair> -> Map：$mapFromPairs")
}

// ============================================================
// 主演示函数
// ============================================================
fun runCollectionsDemo() {
    println("============================================================")
    println("                  Kotlin 集合与序列演示")
    println("============================================================")

    collectionTypesDemo()          // 集合类型
    factoryFunctionsDemo()         // 工厂函数
    collectionOperationsDemo()     // 集合操作
    sequenceDemo()                 // 序列
    performanceComparisonDemo()    // 性能对比
    rangeDemo()                    // 区间
    arrayCollectionConversionDemo() // 数组与集合转换

    println("\n============================================================")
    println("                  集合与序列演示结束")
    println("============================================================")
}
