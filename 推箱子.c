//通关条件：所有目标点都被箱子覆盖（地图上不再有 '.'）
#include <stdio.h>

#define ROW 7  
#define COL 7   

#define WALL          '#'   // 墙 
#define SPACE         ' '   // 空地 
#define PLAYER        '@'   // 玩家 
#define BOX           '$'   // 箱子 
#define TARGET        '.'   // 目标点 
#define BOX_TARGET    '*'   // 箱子在目标上 
#define PLAYER_TARGET '+'   //玩家在目标上

int steps = 0;   // 步数计数器

char map[ROW][COL] = {
    {'#','#','#','#','#','#','#'},
    {'#',' ',' ',' ',' ','.','#'},
    {'#',' ',' ','$',' ',' ','#'},
    {'#',' ','@',' ',' ',' ','#'},
    {'#',' ','$',' ','$',' ','#'},
    {'#',' ','.',' ',' ','.','#'},
    {'#','#','#','#','#','#','#'}
};
//打印地图
void printmap(void) {
    for (int i = 0; i < ROW; i++) {        
        for (int j = 0; j < COL; j++) {    
            printf("%c", map[i][j]);       
        }
        printf("\n");                     
    }
    printf("\n");                          
}
//检查通关
int checkwin(void) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (map[i][j] == TARGET) {      //还有空的目标点就不算赢'.'  
                return 0;   //不通关返回0              
            }
        }
    }
    return 1;                              
}
//移动玩家
int moveplayer(int dx, int dy) {
    int px = 0, py = 0;        //初始化玩家位置

    //找到玩家当前坐标
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (map[i][j] == PLAYER || map[i][j] == PLAYER_TARGET) {
                px = i;                    
                py = j;
                //记录位置                    
            }
        }
    }
    //玩家下一步位置
    int nx = px + dx;                      
    int ny = py + dy;                     

    //检查玩家是否越界，防止数组越界访问
    if (nx < 0 || nx >= ROW || ny < 0 || ny >= COL) {
        return 0;                          
    }

    //检查撞墙
    if (map[nx][ny] == WALL) {
        return 0;                          
    }

    //下一步遇到的是箱子就推箱子
    if (map[nx][ny] == BOX || map[nx][ny] == BOX_TARGET) {
        //箱子下一步位置
        int bx = nx + dx;                  
        int by = ny + dy;                  

        //检查箱子是否越界
        if (bx < 0 || bx >= ROW || by < 0 || by >= COL) {
            return 0;
        }

        //箱子撞墙或者撞到箱子或者撞到目标点的箱子，都不动
        if (map[bx][by] == WALL || map[bx][by] == BOX || map[bx][by] == BOX_TARGET) {
            return 0;
        }

        //箱子移动到新位置，新位置是目标点，变成'*'；否则还是'$'
        map[bx][by] = (map[bx][by] == TARGET) ? BOX_TARGET : BOX;

        //箱子原来位置恢复：如果原来是'*'，恢复成'.'；否则恢复成' '
        map[nx][ny] = (map[nx][ny] == BOX_TARGET) ? TARGET : SPACE;
    }

    //玩家离开原位置，恢复原来位置；如果玩家原来站在目标点上，离开后恢复成'.'；否则恢复成' '
    map[px][py] = (map[px][py] == PLAYER_TARGET) ? TARGET : SPACE;

    //玩家移动到新位置；如果新位置是目标点，玩家变为'+'；否则还是'@'
    map[nx][ny] = (map[nx][ny] == TARGET) ? PLAYER_TARGET : PLAYER;

    return 1;        //移动成功返回1
}

int main(void) {
    char input;                            
    printf("推箱子:w上 s下 a左 d右 q退出\n");
    printmap();   
    while (1) {                   
        printf("输入 w/a/s/d 移动,q 退出：");
        scanf(" %c", &input);             //空格接受换行符
        if (input == 'q') {
            printf("游戏结束。\n");
            break;
        }
        int moved = 0;             //标志变量，是否移动成功
        //根据输入移动玩家
        switch (input) {
            case 'w': moved = moveplayer(-1,  0); break;  
            case 's': moved = moveplayer( 1,  0); break;  
            case 'a': moved = moveplayer( 0, -1); break;  
            case 'd': moved = moveplayer( 0,  1); break;  
            default:
                printf("无效输入\n");    
                continue;                  //跳过本次循环
        }
        if (moved != 0) {
            steps++;
            printmap();                    
            if (checkwin() != 0) {              
                printf("恭喜通关,共用了%d步\n",steps);
                break;                     
            }
        }
    }
    return 0;
}