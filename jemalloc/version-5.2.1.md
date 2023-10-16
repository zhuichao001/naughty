## 新特性:
> chunk这一概念被替换成了extent
> dirty page的decay变成了两阶段，dirty -> muzzy -> retained（基于衰变的清理）
> huge class这一概念不再存在
> 红黑树不再使用，取而代之的是pairing heap
