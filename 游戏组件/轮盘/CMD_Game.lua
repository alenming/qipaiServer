local cmd = {}

--[[
******
* 结构体描述
* {k = "key", t = "type", s = len, l = {}}
* k 表示字段名,对应C++结构体变量名
* t 表示字段类型,对应C++结构体变量类型
* s 针对string变量特有,描述长度
* l 针对数组特有,描述数组长度,以table形式,一维数组表示为{N},N表示数组长度,多维数组表示为{N,N},N表示数组长度
* d 针对table类型,即该字段为一个table类型
* ptr 针对数组,此时s必须为实际长度

** egg
* 取数据的时候,针对一维数组,假如有字段描述为 {k = "a", t = "byte", l = {3}}
* 则表示为 变量a为一个byte型数组,长度为3
* 取第一个值的方式为 a[1][1],第二个值a[1][2],依此类推

* 取数据的时候,针对二维数组,假如有字段描述为 {k = "a", t = "byte", l = {3,3}}
* 则表示为 变量a为一个byte型二维数组,长度都为3
* 则取第一个数组的第一个数据的方式为 a[1][1], 取第二个数组的第一个数据的方式为 a[2][1]
******
]]

--[[--游戏版本
cmd.VERSION 					= appdf.VersionValue(6,0,3)
--游戏标识
cmd.KIND_ID						= 110
	
--游戏人数
cmd.GAME_PLAYER					= 100

--房间名长度
cmd.SERVER_LEN					= 32

--游戏记录长度
cmd.RECORD_LEN					= 5
--]]
--视图位置
cmd.MY_VIEWID					= 2
cmd.AREA_COUNT                  = 8
cmd.TAG_COUNT                  = 32
--动物索引
cmd.HIT_INDEX_S_DZ						=1							--小大众
cmd.HIT_INDEX_S_AD						=2							--小奥迪
cmd.HIT_INDEX_S_BM						=3							--小宝马
cmd.HIT_INDEX_S_BC						=4							--小奔驰
cmd.HIT_INDEX_B_DZ						=5							--大大众
cmd.HIT_INDEX_B_AD						=6							--大奥迪
cmd.HIT_INDEX_B_BM						=7							--大宝马
cmd.HIT_INDEX_B_BC 						=8							--大奔驰


--//下注区域定义
cmd.BET_POSTION_S_DZ      =      0              --//小大众
cmd.BET_POSTION_S_AD      =      1            --  //小奥迪	
cmd.BET_POSTION_S_BM      =      2            --  //小宝马
cmd.BET_POSTION_S_BC      =      3            --  //小奔驰
cmd.BET_POSTION_B_DZ      =      4            --  //大大众
cmd.BET_POSTION_B_AD      =      5           --   //大奥迪
cmd.BET_POSTION_B_BM      =      6           --   //大宝马
cmd.BET_POSTION_B_BC      =      7           --   //大奔驰


--分类信息
cmd.ANIMAL_TYPE_NULL					=0							--无
cmd.ANIMAL_TYPE_BEAST					=1							--走兽
cmd.ANIMAL_TYPE_BIRD					=2							--飞禽
cmd.ANIMAL_TYPE_GOLD					=3							--黄金
cmd.ANIMAL_TYPE_SLIVER					=4							--白银

--游戏开始
--cmd.GAME_START 					= 1
--游戏进行
--cmd.GAME_PLAY					= 100
--下注状态
--cmd.GAME_JETTON					= 100
--游戏结束
--cmd.GAME_END					= 101

--游戏倒计时
cmd.kGAMEFREE_COUNTDOWN			= 1
cmd.kGAMEPLAY_COUNTDOWN			= 2
cmd.kGAMEOVER_COUNTDOWN			= 3

---------------------------------------------------------------------------------------
--服务器命令结构

--游戏空闲
cmd.GAME_SCENE_FREE				= 10
--游戏下注
cmd.GAME_SCENE_BET			    = 20

cmd.GAME_SCENE_END			    = 30
---------------------------------------------------------------------------------------
cmd.SUB_S_GAME_SCEN  = 2001
cmd.SUB_S_GAME_FREE = 2002						        --游戏空闲
cmd.SUB_S_GAME_START = 2003	    						--游戏开始
cmd.SUB_S_GAME_END = 2004								--游戏结束
cmd.SUB_S_PLAY_BET = 2005								--用户下注
cmd.SUB_S_PLAYER_BET_FAILUE = 2006                       --用户下注失败    
cmd.SUB_S_APPLY_BANKER = 2007                           --玩家申请上庄结果
cmd.SUB_S_APPLY_BANKER_FAILUE = 2008                           --玩家申请上庄失败
cmd.SUB_S_CANCEL_BANKER = 2009                           --玩家申请下庄结果
cmd.SUB_S_CHANGE_BANKER = 2010                           --通知切换庄家
cmd.SUB_S_BET_INFO = 2011                           --定时同步下注区域

cmd.SUB_S_ONLINE_PLAYER         =  2013    				--//在线用户
cmd.SUB_S_ONLINE_PLAYER_ERROR   =  2014    				-- //在线用户获取失败
--cmd.SUB_S_PLAY_BET2 = 1125								--用户下注
cmd.SUB_S_BET_CLEAR = 106							    --清除下注
------
---------------------------------------------------------------------------------------
--客户端命令结构

--用户下注
cmd.SUB_C_PLAYER_BET				= 100    --发送用户下注

cmd.SUB_C_APPLY_BANKER				= 101    --申请上庄

cmd.SUB_C_CANCEL_BANKER				= 102   --申请下庄

cmd.SUB_C_ONLINE_PLAYER       =   103   						-- //在线用户
cmd.SUB_C_BET_CLEAR             = 2               
---------------------------------------------------------------------------------------



------







cmd.RES_PATH 					= 	"bcbm/res/"
print("********************************************************load cmd");
return cmd