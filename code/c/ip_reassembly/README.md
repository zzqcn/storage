# ip分片重组试验

以 VMware 虚拟机为准说明.

## 基本用法

1. 编译好 DPDK, 绑定好网卡, 配置好大页内存. 确保至少有一个虚拟网卡
2. 使用 ``gen_frags.py`` 生成测试用的IP分片报文, 如 frags.pcap. IP分片报文的生成必须符合 IP 协议标准, 负责重组会失败. 这个脚本依赖 scapy 库
3. 编译 ip_reassembly:
   ```
   export RTE_SDK=<DPDK>
   make clean && make
   ```
4. 运行 ip_reassembly:
   ```
   cd build/
   sudo  ./ip_reassembly -l 1 -n 4 -- -p 1 --maxflows=32
   ```
   注意:  
   (1) 这里使用了一个 lcore, 如果使用多个lcore, 由于 **某种原因** 同一条流的分片可能被2个 lcore 处理, 造成重组失败  
   (2) maxflows 需要设置的小一点, 不然会消耗大量内存, 虚拟机配置的大页内存可能会不够
5. 在宿主机上使用科来发包工具向虚拟机网卡发包, 发送的报文为frags.pcap
6. 在虚拟机上观查 ip_reassembly 程序打印的log

## 原理

此代码是从 DPDK 同名官方示例修改而来, 增加了一些打印语句, 并且过滤了 ipv4 多播报文以防止虚拟机杂包干扰.

限制条件:
1. 同一条 IP 流的报文必须被一个 lcore 处理, 因为 IP 分片重组表是每一个 rx queue 创建了一个
2. 最大分片数为4, 这是 DPDK 配置配置值

假设我们造的分片报文共有3个分片, 那么收包的时候是3个 mbuf, 重组后会按顺序重组成一个 mbuf, 它由之前的3个 mbuf 通过链表链接而成(``rte_mbuf->next``).

## 调试

为了方便调试研究, 需要注意一些事项:
1. 以调试选项编译 DPDK, 方法是在编译前执行
   ```
   export EXTRA_CFLAGS=`-g -O0`
   ```
2. 修改 <DPDK>/config/common_base, 将 librte_ip_frag 附近的配置修改如下:
   ```
   CONFIG_RTE_LIBRTE_IP_FRAG=y
   CONFIG_RTE_LIBRTE_IP_FRAG_DEBUG=y
   CONFIG_RTE_LIBRTE_IP_FRAG_MAX_FRAG=4
   CONFIG_RTE_LIBRTE_IP_FRAG_TBL_STAT=y
   ```
3. DPDK IP分片重组库默认使用 ``RTE_LOGTYPE_USER1`` 做为调试日志, 所以用到它的程序, 如ip_reassembly 最好使用另一个 log type, 如 ``RTE_LOGTYPE_USER2``, 方法是修改 main.c 中的宏 ``#define RTE_LOGTYPE_IP_RSMBL RTE_LOGTYPE_USER2``. 

   还需要把日志级别修改为 DEBUG, 方法是在 main 函数里 EAL 初始化后调用:
   ```
   rte_log_set_level(RTE_LOGTYPE_USER1, RTE_LOG_DEBUG);
   rte_log_set_level(RTE_LOGTYPE_USER2, RTE_LOG_DEBUG);
   ```