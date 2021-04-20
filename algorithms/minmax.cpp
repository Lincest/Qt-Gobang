/**
*	author:     roccoshi
*	created:    2021-04-20 23:11:31
*/
#include<bits/stdc++.h>
using namespace std;

#define C_NONE 0//棋子：黑子,白子,无子
#define C_BLACK 1
#define C_WHITE 2

//棋型代号 下标 权重
#define OTHER 0//0,其他棋型不考虑
#define WIN 1//100000,白赢
#define LOSE 2//-10000000
#define FLEX4 3//50000,白活4
#define flex4 4//-80000
#define BLOCK4 5//400
#define block4 6//-80000
#define FLEX3 7//400
#define flex3 8//-8000
#define whiteblock3 9//20
#define block3 10//-40
#define FLEX2 11//20
#define flex2 12//-40
#define BLOCK2 13//1
#define block2 14//-2
#define FLEX1 15//1
#define flex1 16//-2

int tuple6type[4][4][4][4][4][4];//棋型辨识数组,0无子,1黑子,2白子,3边界
void init_tuple6type(){
    memset(tuple6type,0,sizeof (tuple6type));//全部设为0
    int p1,p2,p3,p4,p5,p6,x,y,ix,iy;//x:左5中黑个数,y:左5中白个数,ix:右5中黑个数,iy:右5中白个数
    for(p1=0;p1<4;++p1){
        for(p2=0;p2<3;++p2){
            for(p3=0;p3<3;++p3){
                for(p4=0;p4<3;++p4){
                    for(p5=0;p5<3;++p5){
                        for(p6=0;p6<4;++p6){
                            x=y=ix=iy=0;

                            if(p1==1)x++;
                            else if(p1==2)y++;

                            if(p2==1){x++;ix++;}
                            else if(p2==2){y++;iy++;}

                            if(p3==1){x++;ix++;}
                            else if(p3==2){y++;iy++;}

                            if(p4==1){x++;ix++;}
                            else if(p4==2){y++;iy++;}

                            if(p5==1){x++;ix++;}
                            else if(p5==2){y++;iy++;}

                            if(p6==1)ix++;
                            else if(p6==2)iy++;

                            if(p1==3||p6==3){//有边界
                                if(p1==3&&p6!=3){//左边界
                                    //白冲4
                                    if(ix==0&&iy==4){//若右边有空位是活4也没关系，因为活4权重远大于冲4，再加上冲4权重变化可以不计
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }
                                    //黑冲4
                                    if(ix==4&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }
                                    //白眠3
                                    if(ix==0&&iy==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=whiteblock3;
                                    }
                                    //黑眠3
                                    if(ix==3&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }
                                    //白眠2
                                    if(ix==0&&iy==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }
                                    //黑眠2
                                    if(ix==2&&iy==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }                                  
                                }else if(p6==3&&p1!=3){//右边界
                                    //白冲4
                                    if(x==0&&y==4){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                    }
                                    //黑冲4
                                    if(x==4&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                    }      
                                    //黑眠3
                                    if(x==3&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=whiteblock3;
                                    }
                                    //白眠3
                                    if(x==0&&y==3){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                    }
                                    //黑眠2
                                    if(x==2&&y==0){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                    }
                                    //白眠2
                                    if(x==0&&y==2){
                                        if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                            tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                    }
                                }
                            }else{//无边界
                                //白冲4
                                if((x==0&&y==4)||(ix==0&&iy==4)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK4;
                                }
                                //黑冲4
                                if((x==4&&y==0)||(ix==4&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block4;
                                }
                                //白眠3
                                if((x==0&&y==3)||(ix==0&&iy==3)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=whiteblock3;
                                }
                                //黑眠3
                                if((x==3&&y==0)||(ix==3&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block3;
                                }
                                //白眠2
                                if((x==0&&y==2)||(ix==0&&iy==2)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=BLOCK2;
                                }
                                //黑眠2
                                if((x==2&&y==0)||(ix==2&&iy==0)){
                                    if(tuple6type[p1][p2][p3][p4][p5][p6]==0)
                                        tuple6type[p1][p2][p3][p4][p5][p6]=block2;
                                }                               
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    init_tuple6type();
    for(int p1=0;p1<4;++p1){
        for(int p2=0;p2<3;++p2){
            for(int p3=0;p3<3;++p3){
                for(int p4=0;p4<3;++p4){
                    for(int p5=0;p5<3;++p5){
                        for(int p6=0;p6<4;++p6){
                            if (tuple6type[p1][p2][p3][p4][p5][p6] == whiteblock3) {
                                printf("tuple_six_[%d][%d][%d][%d][%d][%d]\n", p1, p2, p3, p4, p5, p6);
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}