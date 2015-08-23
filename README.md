# ranger_bhvr_tree

[![Join the chat at https://gitter.im/ufownl/ranger_bhvr_tree](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/ufownl/ranger_bhvr_tree?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

This is a C++11 behavior tree implementation.

## 简单说明
ranger_bhvr_tree仅需包含相应头文件即可使用，然而为了方便测试异步环境下的逻辑，所有示例程序都使用[CAF](http://github.com/actor-framework/actor-framework)(develop分支)作为底层异步API。若使用默认的`std::function<void(bool, Agent*)>`作为`Handler Policy`，结果处理逻辑将为同步逻辑，但在这种模式下使用循环语义的节点容易出现调用堆栈过深爆栈崩溃的情况。

### Handler Policy Concept
接下来是一个合格的`Handler Policy`所需要具备的最少特性：
* 支持从任意Signature为`void(bool, Agent*)`的`Callable Object`到`Handler Policy`的隐式转换
* 支持复制语义和移动语义
* 提供`void operator () (bool, Agent*)`

示例见[sample_util.hpp](https://github.com/ufownl/ranger_bhvr_tree/blob/master/sample/sample_util.hpp)中`class sample_handler`定义。

