## 修罗版贪吃蛇与不靠谱爬虫
### 修罗版贪吃蛇
* 一款越玩越难的贪吃蛇。等级越高、速度越快越难以避开石块。
* 最后不停报一个Debug Error的错，找错找了好久乱改了好多处都没用，最后发现居然是调用malloc函数分配内存的时候定义的变量名与类型名相同导致的。（感觉很懵心里很没底不知道程序什么时候会不会再崩）
* 还有一个bug没解决就是它往上走的时候有时候撞到石块也不会死。（猜测是因为石块和蛇都在反方向动，所以其坐标未重合。）所以无处可走就向上走哈哈。 
### 不靠谱爬虫
* 先说它为什么不靠谱：它爬多少东西，不是我说了算。。。。按理来说for循环里我增加次数就爬得多，但仿佛不是这样。就算不改代码里任何一个字，运行两次爬下来的东西都不一样多。不过一个月应该没问题。
* 它是按赞数选出前十条。我是按日期爬下数据来然后用split函数依次分割存入列表的。