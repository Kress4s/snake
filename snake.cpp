#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

// 按键头文件
#include <conio.h>

// 播放背景音乐
// #include <mmsystem.h>
// #pragma comment(lib, "winmm.lib")

// 定义蛇的最大长度
#define SNAKE_NUM 500

// 定义窗口宽高
const int height = 480;
const int width = 640;

/*
    需要循环，函数，easyx，数组
*/

// 做界面 图形窗口

// 蛇的结构
struct snake
{
    int size;  // 蛇的节数
    int dir;   // 方向
    int speed; // 速度
    // 坐标 POINT结构体 是源码里面定义好的，直接拿来用
    POINT coor[SNAKE_NUM];
} snake;

// 食物
struct food
{
    int x;
    int y;
    int r;
    bool isEated;   // 食物是否被吃
    DWORD color; // 食物颜色
} food;

// 舍得方向
enum DIR
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

void foodInit();

// 数据初始化
void GameInit()
{
    // 初始化图形窗口; SHOWCONSOLE：显示控制台
    initgraph(width, height, SHOWCONSOLE);
    // 初始化蛇 蛇有3节
    snake.size = 3;
    snake.speed = 10;
    snake.dir = RIGHT;
    // 循环画蛇
    for (int i = snake.size - 1; i >= 0; i--)
    {
        // +20 距离更近点
        /*
        snake.size - 1 -i  因为蛇头坐标为0
        */
        // +5 是 一节蛇的半径，坐标是蛇节的圆心
        snake.coor[snake.size - 1 - i].x = 10 * i + 5;
        snake.coor[snake.size - 1 - i].y = 10;
    }

    // 初始化食物
    foodInit();

    // 播放背景音乐
    // mciSendString("open ./bgm.mp3 alias bgm", 0, 0, 0);
    // mciSendString("play bgm repeat", 0, 0, 0);
};

// 初始化 画布
void GameDraw()
{
    // 双缓冲绘图
    BeginBatchDraw();
    // 设置背景颜色（两步骤）
    setbkcolor(RGB(28, 115, 119));
    // 清除绘图设备
    cleardevice();

    // 绘制蛇
    setfillcolor(WHITE); // 设置蛇的颜色
    for (int i = 0; i < snake.size; i++)
    {
        solidcircle(snake.coor[i].x, snake.coor[i].y, 5);
    }

    // 绘制食物
    if (!food.isEated){
        solidcircle(food.x, food.y, 5);
    }
    EndBatchDraw();
};

// 移动蛇
void Move()
{
    // 当前函数，并没有贪吃蛇移动的样子，因为所有的坐标都是跟着一起移动，是平行移动的
    // for (int i = 0; i < snake.size; i++)
    // {
    //     switch (snake.dir)
    //     {
    //     case RIGHT:
    //         snake.coor[i].x++;
    //         break;
    //     case LEFT:
    //         snake.coor[i].x--;
    //         break;
    //     case UP:
    //         snake.coor[i].y--;
    //         break;
    //     case DOWN:
    //         snake.coor[i].y++;
    //         break;
    //     }
    // }

    // 让身体跟着头移动
    // for (int i = 1; i < snake.size; i++)
    // {
    //     snake.coor[i] = snake.coor[i - 1];
    // };
    // 从尾部开始变换坐标，不然就会变成两个点了，赋值顺序错了
    for (int i = snake.size - 1; i > 0; i--)
    {
        snake.coor[i] = snake.coor[i - 1];
    };

    switch (snake.dir)
    {
    // 这里的为什么加上speed？因为我们的蛇的直径是10，保证每个节点一定的像素点是一样的，不然都挤在蛇头了
    // 还有一个问题，蛇不能原路返回，往下就不能往上，往左就不能望右
    case UP:
        // 过界
        if (snake.coor[0].y < 0)
        {
            snake.coor[0].y = height - 10;
        }
        snake.coor[0].y -= snake.speed;
        break;

    case DOWN:
        // 过界
        if (snake.coor[0].y > height)
        {
            snake.coor[0].y = 10;
        }
        snake.coor[0].y += snake.speed;
        break;

    case LEFT:
        // 过界
        if (snake.coor[0].x < 0)
        {
            snake.coor[0].x = width - 10;
        }
        snake.coor[0].x -= snake.speed;
        break;
    case RIGHT:
        // 过界
        if (snake.coor[0].x > width)
        {
            snake.coor[0].x = 10;
        }
        snake.coor[0].x += snake.speed;
        break;
    }
};

// 通过按键改变蛇的方向
void keyControl()
{
    // 当前函数直接加到主函数中， 只有按键了，蛇才会动，这时候需要判断是否有按键( _kbhit() 函数)
    if (!_kbhit())
    {
        return;
    }
    // 72 80 75 77 上下左右键值码
    switch (_getch())
    {
    case 'w':
    case 'W':
    case 72:
        if (snake.dir != DOWN)
        {
            snake.dir = UP;
        }
        break;
    case 's':
    case 'S':
    case 80:
        if (snake.dir != UP)
        {
            snake.dir = DOWN;
        }
        break;
    case 'a':
    case 'A':
    case 75:
        if (snake.dir != RIGHT)
        {
            snake.dir = LEFT;
        }
        break;
    case 'D':
    case 'd':
    case 77:
        if (snake.dir != LEFT)
        {
            snake.dir = RIGHT;
        }
        break;
    }
};

//初始化食物
void foodInit(){
    // 初始化食物 rand()随机产生一个整数，如果没用设置随机种子，每次产生的都是一样的
    // 设置随机数种子: GetTickCount() 获取系统开机到现在经历的毫秒数，一直变化的
    srand(GetTickCount());
    food.x = rand() % width;
    food.y = rand() % height;
    food.r = 5;
    food.color = RGB(rand() % 256, rand() % 256, rand() % 256);
    food.isEated = false;
}; 

// 吃食物：只要头到食物的距离小于食物的半径
void eatFood(){
    if (abs(food.x - snake.coor[0].x) <= food.r && abs(food.y - snake.coor[0].y) <= food.r && !food.isEated){
        food.isEated = true;
        snake.size++;
    }
    if (food.isEated) {
        foodInit();
    }
};


// 游戏暂停
void stop(){
    if(kbhit()){
        if(_getch() == ' '){

        }
    }
};

int main()
{
    GameInit();
    // GameDraw();
    while (1)
    {
        GameDraw();
        Move();
        keyControl();
        eatFood();
        Sleep(50);
    }

    return 0;
};