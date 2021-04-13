#ifndef CONTROL_PACKET_HEAD_FILE
#define CONTROL_PACKET_HEAD_FILE

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//事件结果
#define ER_FAILURE					0									//失败标志
#define ER_SUCCESS					1									//成功标志

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//界面数据包

#define UI_CORRESPOND_RESULT		1									//登录结果
#define UI_LOAD_DB_LIST_RESULT		2									//列表成功

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661
//控制数据包

#define CT_CONNECT_CORRESPOND		100									//连接登录
#define CT_LOAD_DB_GAME_LIST		101									//加载列表

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

//控制结果
struct CP_ControlResult
{
	BYTE							cbSuccess;							//成功标志
};

/////////////////////////////////本程序由猫昼游戏共享，本代码价值18万，QQ:361283661

#endif