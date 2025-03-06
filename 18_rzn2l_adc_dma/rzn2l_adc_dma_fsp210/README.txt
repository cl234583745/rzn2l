rzn2l_adc_dma_fsp210
	Created on: 2024/12/18
	Author: jerryfllpll 292812832@qq.com
	
基于瑞萨rsk开发板做连续adc读取，通过dma联系传输
1、adc AN103
2、	//To perform transfers in series, write 1 to the REN bit in the CHCFG_n register immediately!!!
    //So you can't resume debugging from the pause!!!
    //p_instance_ctrl->p_reg->GRP[group].CH[channel].CHCFG_b.REN = 1;
    g_transfer0_ctrl.p_reg->GRP[0].CH[0].CHCFG_b.REN = 1;
3、//todo:data parse