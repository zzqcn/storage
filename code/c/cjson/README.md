# cJSON的基本使用与出错信息的改进

详细文章见 yuque.com/zzqcn/ftvl2p/wu0kb5

## 目录结构

```
.
    ├── cjson       // cJSON 1.7.12原版
    ├── cjson2      // cJSON 1.7.12出错信息改进版
    ├── demo.json   // JSON样本
    ├── builder.c   // 从C数据结构构造JSON
    ├── parser.c    // 使用原版cJSON解析JSON
    └── parser2.c   // 使用改进版cJSON解析JSON, 给出更多错误信息
```

## 编译运行

```bash
$make
$./parser <input JSON file path>
$./parser2 <input JSON file path>
$./builder <output JSON file path>
```

