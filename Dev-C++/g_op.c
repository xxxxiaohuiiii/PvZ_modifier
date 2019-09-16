/*Game Operation*/

#include "headgr.h"

extern char processName[];
extern DWORD Pid;
extern HANDLE hProcess;
extern int command;

extern BOOL isRun;
extern BOOL backStage;
extern BOOL anyPosition;
extern BOOL isLittle; 

/*打开游戏进程*/
BOOL openGameProcess(void){
	setColor(14);
	printf("正在打开进程...\n");
	
	//寻找进程名称并返回pid
	if(FindProcessPid(processName,Pid)){
		printf("[%s][Pid：%d]\n",processName,Pid);
		hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,Pid);
		
		if (hProcess!=0){
			setColor(10);
			printf("\n成功打开游戏进程[%s]！进程句柄：%d\n",processName,hProcess);
			
			if(readMemory(hProcess,0x42748E,0,4)==0xFF563DE8){
				return TRUE;
			}else{
				setColor(14);
				printf("\n不支持的游戏版本！目前仅支持英文原版和汉化一代/二代\n");
				return FALSE;
			}
		}else{
			setColor(12);
			printf("\n打开游戏进程[%s]失败\n",processName);
		}
	}else{
		setColor(14);
		printf("未找到游戏进程：%s\n",processName);
	}
	
	return FALSE;
}

/*是否已经读取游戏进程*/
BOOL openModify(void){
	if(hProcess==0){
		setColor(14);
		printf("\n请先打开修改器搜索游戏句柄！\n");
		return FALSE;
	}
	
	return TRUE;
}

/*选择流程*/
void Choice(void){
	//缓冲区
	int _tempValue;
	
	setColor(13);
	//根据字符判断应该执行什么流程
	switch(command){
		case 'O':
			isRun=openGameProcess();
			pause();
			break;
		#if DEBUG
			case 'T':
				if(!openModify()){
					printf("\n当前值为：");
				}
				break;
		#endif
		
		/*常规修改*/
		case 'a':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【常规修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify()){
						printf("\n请输入要修改的阳光数量：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,0,9990);
						cheatMsg(writeMemory(hProcess,0x006A9EC0,2,_tempValue,4,0x768,0x5560),"阳光");
					}
					break;
				case '2':
					if(openModify()){
						printf("\n请输入要修改的金钱数量：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,0,99999);
						cheatMsg(writeMemory(hProcess,0x006A9EC0,2,_tempValue,4,0x82C,0x28),"金钱");
					}
					break;
				case '3':
					if(openModify())cheatMsg(writeMemory(hProcess,0x00487296,0,0x1477,2),"卡牌无冷却");
					break;
				case '4':
					if(openModify()){
						if(backStage){
							cheatMsg(writeMemory(hProcess,0x0054EBEF,0,0xC3,1),"取消后台");
						}else{
							cheatMsg(writeMemory(hProcess,0x0054EBEF,0,0x57,1),"恢复后台");
						}
						backStage=!backStage;
					}
					break;
				case '5':
					if(openModify()){
						printf("\n请输入要修改的卡槽格数（1~10）：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,1,10);
						cheatMsg(writeMemory(hProcess,0x006A9EC0,3,_tempValue,4,0x768,0x144,0x24),"卡槽格数");
					}
					break;
				case '6':
					if(openModify())cheatMsg(writeMemory(hProcess,0x0043158F,0,0xEB,1),"自动收集开启");
					break;
				default :
					noOperation();
			}
			break;
		
		/*关卡修改*/
		case 'b':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【关卡修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify()){
						printf("\n请输入要修改的冒险关卡（例如我要去4-3关卡，那么应该输入43）：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,11,99);
						_tempValue-=10;
						cheatMsg(writeMemory(hProcess,0x006A9EC0,2,_tempValue,4,0x82C,0x24),"冒险模式关卡");
					}
					break;
				case '2':
					if(openModify()){
						printf("\n需要先进入任意关卡然后点击菜单后（一定要这么做，否则会崩溃或者修改无效，此外重新开始会清除该模式下的存档，请注意备份存档），修改关卡代码然后点击重新开始，如果不重新开始的话将会产生混乱关卡的效果\n");
						printf("请输入要进入的迷你游戏ID（ID可通过查询【!】菜单的关卡代码（1~70））：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,1,70);
						cheatMsg(writeMemory(hProcess,0x006A9EC0,1,_tempValue,4,0x7F8),"小游戏跳关");
					}
					break;
				default :
					noOperation();
			}
			break;
		
		/*植物修改*/
		case 'c':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【植物修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify()){
						if(!anyPosition){
							cheatMsg(writeMemory(hProcess,0x0040FE30,0,0x81,1)&&writeMemory(hProcess,0x00438E40,0,0xEB,1)&&writeMemory(hProcess,0x0042A2D9,0,0x8D,1),"开启重叠放置");
						}else{
							cheatMsg(writeMemory(hProcess,0x0040FE30,0,0x84,1)&&writeMemory(hProcess,0x00438E40,0,0x74,1)&&writeMemory(hProcess,0x0042A2D9,0,0x84,1),"取消重叠放置");
						}
						anyPosition=!anyPosition;
					}
					break;
				case '2':
					if(openModify())cheatMsg(writeMemory(hProcess,0x0041D7D0,0,0xC301B0,3)&&writeMemory(hProcess,0x0040E477,0,0xEB,1),"紫卡种植无限制");
					break;
				default :
					noOperation();
			}
			break;
		
		/*僵尸修改*/
		case 'd':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【僵尸修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify()){
						int z_i;
						BOOL kill=FALSE;
						DWORD z_status;
						/*pvz最多有1024只僵尸*/
						for(z_i=0;z_i<1024;z_i++){
							z_status=readMemory(hProcess,0x006A9EC0,3,4,0x768,0x90,z_i*0x15C+0xEC);
							//printf("僵尸状态：%d",z_status);
							if(z_status==0)kill=writeMemory(hProcess,0x006A9EC0,3,3,4,0x768,0x90,z_i*0x15C+0x28);
						}
						cheatMsg(kill,"秒杀全部僵尸");
					}
					break;
				case '2':
					if(openModify()){
						if(!isLittle){
							cheatMsg(writeMemory(hProcess,0x00523ED5,0,0xEB,1),"开启小僵尸特效");
						}else{
							cheatMsg(writeMemory(hProcess,0x00523ED5,0,0x74,1),"关闭小僵尸特效");
						}
						isLittle=!isLittle;
					}
					break;
				default :
					noOperation();
			}
			break;
		
		/*花园修改*/
		case 'e':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【花园修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify()){
						printf("\n请输入要修改的智慧树高度：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,0,2147483647);
						cheatMsg(writeMemory(hProcess,0x006A9EC0,2,_tempValue,4,0x82C,0xF4),"智慧树高度");
					}
					break;
				case '2':
					if(openModify()){
						printf("\n请输入要修改的巧克力数量：");
						scanf("%d",&_tempValue);
						_tempValue=limit(_tempValue,0,999);
						_tempValue+=1000;
						cheatMsg(writeMemory(hProcess,0x006A9EC0,2,_tempValue,4,0x82C,0x228),"巧克力数量");
					}
					break;
				default :
					noOperation();
			}
			break;
		
		/*其他修改*/
		case 'f':
			if(!openModify()){
				pause();
				break;
			}
			
			getchar();
			setColor(14);
			printf("\n已进入【其他修改】菜单，请选择子选项：");
			command=getchar();
			setColor(13);
			switch(command){
				case '1':
					if(openModify())cheatMsg(writeMemory(hProcess,0x0044E5CC,0,0x0033B866,4),"花瓶透视开启");
					break;
				case '2':
					if(openModify())cheatMsg(writeMemory(hProcess,0x0042DF5D,0,0x38,1),"显示隐藏小游戏");
					break;
				default :
					noOperation();
			}
			break;
		//~
		case '\x7E':
			upData();
			break;
		//!
		case '\x21':
			showMiniGameList();
			break;
		//@
		case '\x40':
			cheatCode();
			break;
		//#
		case '\x23':
			showOpenCheat();
			break;
		case 'A':
			about();
			break;
		case 'E':
			exit(0);
			break;
		default :
			noOperation();
	}
	
	//暂缓输入
	getchar();
}

/*规范数据范围*/
int limit(int value,int min,int max){
	if(value<min)return min;
	if(value>max)return max;
	
	return value;
}
