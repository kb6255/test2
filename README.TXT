睡眠：仅内核停止，所有内核都在运行。
      __WFI//__WFE进入。(Wait for interrupt//wait for event)
      配置SLEEPONEXIT=0; 立即随眠：SLEEPONEXIT=1; 退出后睡眠：
      唤醒方式：WFI:任一中断。 WFE：任一事件（事件太多，很少使用）
      唤醒后退出中断，接着执行WFI后指令。

停止：在睡眠基础上进一步关闭了所有的时钟。保留了内核寄存器，内存的信息，所以从停止模式唤醒并重新开启时钟还可以从上次停止处继续执行，可由任一外部中断（EXTI）唤醒。并可选择电压调节器运行状态。
      Sleepdeep=1;深度睡眠（STOP模式）
      PDDS=0掉电深睡眠位（进入停止模式） PDDS=1;待机模式
      LPDS=0;电压调节器工作；LPDS=1;低功耗模式电压调节器掉电
      进入方式：PDDS+LPDS+SLEEPDEEP+WFI//WFE
唤醒方式：任一EXTI中断和事件
     唤醒方式：PVD\RTC\USB\EXTI
唤醒有延时：启动进入HSI内部高速振荡器+电压调节器掉电启动时间。
待机：1.8V电源关闭，唤醒后程序重头开始。引脚工作在高阻态。
      进入方式：PDDS+SLEEPDEEP+（需要清除WUF标志）+WFI//WFE
	  唤醒方式：WAKUP\ RTC\NRST\IWDG

1阿萨达
大萨达萨达十大权威生产者创造性山大啊