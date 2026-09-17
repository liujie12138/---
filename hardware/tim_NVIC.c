#include "stm32f10x.h"

/* ★新增:阈值全部用“采样次数 × 中断周期”表达,以后改手感/改周期只动这三行
 *   TIM2 中断周期 = 72MHz / 200 / 7200 = 20ms   (PSC = 199, ARR = 7199)
 */
#define KEY_DEBOUNCE_N   4      /* 消抖:连续 4 次采样为低 = 4 × 20ms = 80ms 才算按下 */
#define KEY_LONG_N       50     /* 长按:连续 50 次采样为低 = 50 × 20ms = 1s  → key_num = -1 */
#define KEY_WINDOW_N     150    /* 采样窗口:150 × 20ms = 3s(到期自动关 TIM2) */

//int8_t key_num = 0;                                                                  // ★修改前:ISR 里改、main 里读,却没有 volatile
volatile int8_t key_num = 0;                                                           // ★修改后:中断与主程序共享的变量必须 volatile
uint8_t kill_counter = 0;//控制采样频率做到消抖
uint16_t tim_counter = 0;
volatile int8_t key_numLP = 0;
void tim_NVIC_init(void)
{   //配置接受按键
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_InitTypeDef GPIO_Initstructure;
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&GPIO_Initstructure);

    // 开 TIM2 时钟（TIM2 在 APB1）
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    // 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitstructure;
    TIM_TimeBaseInitstructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitstructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitstructure.TIM_Period = 7200 - 1;
    TIM_TimeBaseInitstructure.TIM_Prescaler = 200 - 1;
    TIM_TimeBaseInitstructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitstructure);

    
    //中断使能
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

    //中断分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //初始化NVIC
    NVIC_InitTypeDef NVIC_Initsructure;
    NVIC_Initsructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_Initsructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Initsructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_Initsructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_Initsructure);
}
//定时中断函数(状态机消抖)(中断周期 = 20ms)
void TIM2_IRQHandler(void){
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                                       // ★修改前:不判中断来源就清标志
	tim_counter ++;
    if(tim_counter <150){
       if(kill_counter >100){                                                       // ★原代码问题:kill_counter 到 4 就被清零,
              key_numLP = -1;                                                         //   永远到不了 100 → 长按判定是死代码
       }
       if(kill_counter == 4){
           key_num++;
           kill_counter = 0;
       }
       else{
           if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6) == 0){
               kill_counter ++;
           }
           else{
               kill_counter =0;
           }
       }
    }
    else{
       TIM_Cmd(TIM2,DISABLE);
       tim_counter = 0;
       key_numLP = 0;
    };
}

