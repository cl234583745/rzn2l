loader app fsp2.0
	Created on: 2024年12月17日
	Author: jerryfllpll 292812832@qq.com

该例程基于官网r01an6737xx0200-rzn2l-separating-loader-and-application修改优化的，也参考了其他相关例程，
主要是增加了通常升级固件可能用到的资源，构建基础工程，细节需要用户实现。

// README.txt
// 1: 正确完整的编译loader+app:必须首先clean loader，构建前增加make -r -j8 clean
// 2: 使能startup的初始化mpu代码 #if 1 // Original program
    /* Invalid these settings for loader project.
    * These settings are done in the application program.
    * Settings can also be made in the loader program if necessary. */
// 3: BSP_CFG_LDR_SIZE_NML 要根据loader而增大
// 4: 这里动态计算app bin大小IMAGE_APP_FLASH_section，进行复制app的，实际升级固件app肯定会变化，需要修改预留app大小
// 5: loader除了引导app外，loader升级app需要用到xspi、crc可参考例程使用；OTP可读写UID可参考例程
// 6: 其他可能用到的功能如AES、签名等需要自己实现