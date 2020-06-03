#pragma gcc diagnostic error "-std=c++11"

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <time.h>
#include <string.h>
#include <limits>
#include <algorithm>
#include <memory>

using namespace std;

#define POPSIZE 400
#define MAXGENS 500
#define PXOVER 0.4
#define PMUTATION 0.0001
#define MUTI 2
#define MUTINUM 2
#define FITNESS_C 100

#define B 30                //数据块数

#define W 28                //资源域中心数量

#define D 2000              //用户要求的截止时间

//数据块大小
int S[B] ={238 ,368 ,233 ,165 ,228 ,74 ,122 ,170 ,2 ,106 ,
           35 ,58 ,380 ,283 ,304 ,95 ,193 ,204 ,194 ,341 ,
           220 ,125 ,311 ,226 ,98 ,150 ,41 ,227 ,172 ,187
};

//每个资源域中心的VM数量
int R[W] ={79 ,55 ,50 ,51 ,71 ,55 ,70 ,59 ,71 ,59 ,
           59 ,54 ,49 ,58 ,63 ,45 ,62 ,59 ,78 ,52 ,
           57 ,66 ,63 ,50 ,58 ,47 ,77 ,64
};

//每个资源域中心的VM的存储容量
int M[W] = {89 ,497 ,327 ,518 ,229 ,79 ,382 ,219 ,397 ,440 ,
            409 ,80 ,257 ,349 ,551 ,497 ,556 ,477 ,107 ,516 ,
            428 ,372 ,509 ,559 ,406 ,359 ,253 ,228
};

//每个资源域中心的VM的处理速度
int V[W]={40 ,4 ,2 ,8 ,16 ,1 ,1 ,48 ,32 ,2 ,
          8 ,8 ,64 ,96 ,48 ,1 ,40 ,64 ,2 ,4 ,
          4 ,32 ,40 ,4 ,16 ,2 ,1 ,96
};

//每个资源域中心对应的网络带宽
double N[W]={0.300 ,1.750 ,0.800 ,0.800 ,0.525 ,2.085 ,2.085 ,2.085 ,2.780 ,2.780 ,
             0.800 ,1750.000 ,2.780 ,3.500 ,3.500 ,3.500 ,4.000 ,9.500 ,13.600 ,10.000 ,
             2.800 ,2.800 ,2.880 ,2.880 ,2.880 ,6.800 ,13.600 ,19.000
};

//每个资源域中心的租用单价
double P[W]={0.178 ,0.041 ,0.081 ,0.269 ,0.357 ,0.033 ,0.066 ,0.132 ,0.526 ,1.053 ,
             0.714 ,1.418 ,2.330 ,2.856 ,4.312 ,8.624 ,14.000 ,18.984 ,25.312 ,22.400 ,
             0.721 ,1.442 ,2.884 ,5.768 ,19.012 ,12.656 ,25.200 ,45.696
};

//每个资源域中心的上行带宽费用
double U[W]={3.000 ,0.800 ,0.600 ,0.590 ,1.000 ,0.800 ,0.529 ,0.660 ,0.720 ,0.500 ,
             0.800 ,0.800 ,0.590 ,0.720 ,0.800 ,0.800 ,0.800 ,0.500 ,0.530 ,0.500 ,
             0.720 ,0.500 ,0.800 ,0.400 ,0.500 ,0.530 ,0.720 ,0.600
};

//两点之间的网络延迟
double L[B][W]={
        0.202 ,0.403 ,0.134 ,0.020 ,0.292 ,0.239 ,0.167 ,0.155 ,0.291 ,0.117 ,
        0.099 ,0.368 ,0.163 ,0.029 ,0.201 ,0.228 ,0.079 ,0.353 ,0.294 ,0.142 ,
        0.174 ,0.400 ,0.336 ,0.371 ,0.242 ,0.338 ,0.163 ,0.352 ,0.197 ,0.241 ,
        0.185 ,0.023 ,0.224 ,0.343 ,0.023 ,0.096 ,0.205 ,0.213 ,0.274 ,0.076 ,
        0.354 ,0.396 ,0.024 ,0.140 ,0.405 ,0.249 ,0.392 ,0.064 ,0.182 ,0.265 ,
        0.229 ,0.378 ,0.245 ,0.188 ,0.372 ,0.067 ,0.106 ,0.158 ,0.043 ,0.327 ,
        0.379 ,0.251 ,0.329 ,0.183 ,0.217 ,0.375 ,0.303 ,0.402 ,0.168 ,0.200 ,
        0.058 ,0.145 ,0.176 ,0.105 ,0.265 ,0.161 ,0.334 ,0.237 ,0.248 ,0.139 ,
        0.082 ,0.057 ,0.097 ,0.351 ,0.268 ,0.049 ,0.041 ,0.355 ,0.230 ,0.063 ,
        0.261 ,0.190 ,0.338 ,0.170 ,0.396 ,0.135 ,0.126 ,0.279 ,0.117 ,0.317 ,
        0.059 ,0.198 ,0.085 ,0.258 ,0.283 ,0.330 ,0.042 ,0.197 ,0.190 ,0.271 ,
        0.316 ,0.296 ,0.351 ,0.036 ,0.226 ,0.199 ,0.108 ,0.247 ,0.177 ,0.318 ,
        0.291 ,0.061 ,0.131 ,0.208 ,0.255 ,0.107 ,0.367 ,0.361 ,0.410 ,0.063 ,
        0.301 ,0.091 ,0.284 ,0.366 ,0.373 ,0.148 ,0.320 ,0.395 ,0.368 ,0.090 ,
        0.246 ,0.307 ,0.366 ,0.176 ,0.323 ,0.215 ,0.399 ,0.054 ,0.042 ,0.156 ,
        0.352 ,0.356 ,0.198 ,0.106 ,0.188 ,0.033 ,0.237 ,0.134 ,0.416 ,0.269 ,
        0.221 ,0.340 ,0.341 ,0.129 ,0.330 ,0.293 ,0.300 ,0.229 ,0.268 ,0.248 ,
        0.343 ,0.137 ,0.178 ,0.332 ,0.294 ,0.124 ,0.170 ,0.273 ,0.157 ,0.193 ,
        0.052 ,0.132 ,0.172 ,0.230 ,0.262 ,0.383 ,0.285 ,0.078 ,0.141 ,0.282 ,
        0.327 ,0.385 ,0.202 ,0.291 ,0.093 ,0.112 ,0.208 ,0.373 ,0.365 ,0.056 ,
        0.244 ,0.330 ,0.217 ,0.402 ,0.285 ,0.090 ,0.148 ,0.035 ,0.386 ,0.329 ,
        0.251 ,0.061 ,0.084 ,0.046 ,0.271 ,0.326 ,0.409 ,0.180 ,0.384 ,0.130 ,
        0.042 ,0.334 ,0.095 ,0.267 ,0.206 ,0.212 ,0.402 ,0.394 ,0.165 ,0.347 ,
        0.073 ,0.032 ,0.300 ,0.270 ,0.056 ,0.165 ,0.383 ,0.184 ,0.224 ,0.392 ,
        0.136 ,0.054 ,0.033 ,0.200 ,0.124 ,0.328 ,0.106 ,0.113 ,0.087 ,0.070 ,
        0.266 ,0.109 ,0.027 ,0.341 ,0.399 ,0.213 ,0.176 ,0.382 ,0.187 ,0.364 ,
        0.352 ,0.240 ,0.376 ,0.232 ,0.132 ,0.412 ,0.020 ,0.095 ,0.220 ,0.224 ,
        0.067 ,0.336 ,0.302 ,0.124 ,0.159 ,0.405 ,0.032 ,0.245 ,0.142 ,0.099 ,
        0.339 ,0.388 ,0.231 ,0.346 ,0.353 ,0.254 ,0.139 ,0.109 ,0.216 ,0.349 ,
        0.053 ,0.147 ,0.212 ,0.051 ,0.402 ,0.325 ,0.087 ,0.403 ,0.400 ,0.286 ,
        0.250 ,0.091 ,0.245 ,0.175 ,0.195 ,0.385 ,0.203 ,0.207 ,0.253 ,0.325 ,
        0.329 ,0.172 ,0.293 ,0.184 ,0.141 ,0.226 ,0.417 ,0.303 ,0.358 ,0.213 ,
        0.232 ,0.390 ,0.383 ,0.024 ,0.022 ,0.366 ,0.372 ,0.132 ,0.392 ,0.395 ,
        0.398 ,0.265 ,0.065 ,0.266 ,0.020 ,0.283 ,0.274 ,0.247 ,0.113 ,0.107 ,
        0.152 ,0.022 ,0.302 ,0.025 ,0.229 ,0.023 ,0.274 ,0.227 ,0.306 ,0.211 ,
        0.063 ,0.161 ,0.225 ,0.069 ,0.166 ,0.270 ,0.058 ,0.161 ,0.381 ,0.030 ,
        0.136 ,0.402 ,0.319 ,0.224 ,0.292 ,0.362 ,0.088 ,0.146 ,0.189 ,0.181 ,
        0.276 ,0.321 ,0.227 ,0.158 ,0.369 ,0.036 ,0.204 ,0.266 ,0.286 ,0.133 ,
        0.100 ,0.371 ,0.274 ,0.348 ,0.021 ,0.063 ,0.197 ,0.059 ,0.203 ,0.202 ,
        0.069 ,0.362 ,0.184 ,0.411 ,0.167 ,0.099 ,0.353 ,0.235 ,0.224 ,0.165 ,
        0.039 ,0.123 ,0.109 ,0.246 ,0.261 ,0.058 ,0.305 ,0.088 ,0.347 ,0.214 ,
        0.243 ,0.027 ,0.165 ,0.097 ,0.355 ,0.166 ,0.183 ,0.175 ,0.204 ,0.410 ,
        0.357 ,0.297 ,0.352 ,0.164 ,0.288 ,0.099 ,0.243 ,0.264 ,0.314 ,0.090 ,
        0.409 ,0.333 ,0.193 ,0.098 ,0.202 ,0.078 ,0.179 ,0.087 ,0.188 ,0.149 ,
        0.280 ,0.412 ,0.156 ,0.068 ,0.089 ,0.134 ,0.214 ,0.296 ,0.332 ,0.398 ,
        0.285 ,0.312 ,0.275 ,0.218 ,0.056 ,0.186 ,0.297 ,0.322 ,0.030 ,0.233 ,
        0.035 ,0.062 ,0.189 ,0.209 ,0.183 ,0.371 ,0.310 ,0.342 ,0.081 ,0.078 ,
        0.071 ,0.384 ,0.070 ,0.250 ,0.075 ,0.182 ,0.407 ,0.269 ,0.058 ,0.319 ,
        0.248 ,0.324 ,0.211 ,0.146 ,0.164 ,0.290 ,0.355 ,0.041 ,0.235 ,0.408 ,
        0.297 ,0.250 ,0.050 ,0.067 ,0.082 ,0.213 ,0.061 ,0.372 ,0.178 ,0.165 ,
        0.030 ,0.229 ,0.172 ,0.080 ,0.102 ,0.227 ,0.286 ,0.088 ,0.120 ,0.324 ,
        0.030 ,0.347 ,0.271 ,0.221 ,0.116 ,0.415 ,0.133 ,0.051 ,0.079 ,0.348 ,
        0.039 ,0.357 ,0.178 ,0.112 ,0.047 ,0.241 ,0.305 ,0.131 ,0.236 ,0.063 ,
        0.318 ,0.246 ,0.315 ,0.113 ,0.349 ,0.040 ,0.364 ,0.215 ,0.151 ,0.063 ,
        0.162 ,0.161 ,0.033 ,0.413 ,0.405 ,0.130 ,0.409 ,0.118 ,0.204 ,0.068 ,
        0.046 ,0.223 ,0.048 ,0.248 ,0.316 ,0.075 ,0.112 ,0.244 ,0.229 ,0.328 ,
        0.330 ,0.170 ,0.197 ,0.268 ,0.264 ,0.127 ,0.287 ,0.207 ,0.365 ,0.418 ,
        0.250 ,0.108 ,0.203 ,0.264 ,0.101 ,0.187 ,0.417 ,0.133 ,0.286 ,0.201 ,
        0.181 ,0.355 ,0.047 ,0.209 ,0.183 ,0.386 ,0.307 ,0.319 ,0.210 ,0.159 ,
        0.270 ,0.162 ,0.309 ,0.047 ,0.410 ,0.153 ,0.197 ,0.320 ,0.383 ,0.143 ,
        0.319 ,0.256 ,0.274 ,0.101 ,0.100 ,0.398 ,0.269 ,0.140 ,0.110 ,0.177 ,
        0.321 ,0.271 ,0.113 ,0.391 ,0.103 ,0.319 ,0.357 ,0.033 ,0.261 ,0.190 ,
        0.215 ,0.111 ,0.333 ,0.104 ,0.139 ,0.366 ,0.280 ,0.359 ,0.266 ,0.287 ,
        0.125 ,0.208 ,0.123 ,0.379 ,0.289 ,0.246 ,0.356 ,0.181 ,0.366 ,0.090 ,
        0.382 ,0.310 ,0.384 ,0.117 ,0.281 ,0.067 ,0.060 ,0.261 ,0.123 ,0.301 ,
        0.031 ,0.318 ,0.392 ,0.387 ,0.046 ,0.154 ,0.376 ,0.349 ,0.094 ,0.222 ,
        0.216 ,0.199 ,0.410 ,0.319 ,0.201 ,0.323 ,0.188 ,0.180 ,0.127 ,0.134 ,
        0.250 ,0.132 ,0.067 ,0.214 ,0.229 ,0.371 ,0.304 ,0.269 ,0.213 ,0.050 ,
        0.194 ,0.267 ,0.392 ,0.166 ,0.278 ,0.061 ,0.344 ,0.234 ,0.390 ,0.060 ,
        0.036 ,0.229 ,0.239 ,0.070 ,0.171 ,0.063 ,0.416 ,0.339 ,0.223 ,0.123 ,
        0.096 ,0.097 ,0.235 ,0.143 ,0.334 ,0.088 ,0.095 ,0.261 ,0.337 ,0.331 ,
        0.292 ,0.154 ,0.178 ,0.306 ,0.300 ,0.079 ,0.347 ,0.266 ,0.293 ,0.360 ,
        0.307 ,0.353 ,0.169 ,0.169 ,0.046 ,0.320 ,0.212 ,0.042 ,0.239 ,0.059 ,
        0.188 ,0.315 ,0.179 ,0.046 ,0.038 ,0.093 ,0.114 ,0.156 ,0.377 ,0.074 ,
        0.067 ,0.249 ,0.208 ,0.269 ,0.178 ,0.131 ,0.329 ,0.106 ,0.378 ,0.245 ,
        0.046 ,0.265 ,0.178 ,0.238 ,0.057 ,0.204 ,0.138 ,0.249 ,0.269 ,0.400 ,
        0.331 ,0.037 ,0.294 ,0.090 ,0.064 ,0.356 ,0.206 ,0.201 ,0.093 ,0.163 ,
        0.299 ,0.183 ,0.035 ,0.087 ,0.032 ,0.193 ,0.241 ,0.384 ,0.322 ,0.199 ,
        0.209 ,0.391 ,0.087 ,0.410 ,0.209 ,0.124 ,0.194 ,0.370 ,0.396 ,0.087 ,
        0.350 ,0.307 ,0.104 ,0.224 ,0.020 ,0.191 ,0.203 ,0.206 ,0.372 ,0.276 ,
        0.349 ,0.294 ,0.082 ,0.364 ,0.361 ,0.138 ,0.180 ,0.226 ,0.102 ,0.083 ,
        0.405 ,0.334 ,0.097 ,0.115 ,0.325 ,0.286 ,0.262 ,0.142 ,0.279 ,0.238 ,
        0.209 ,0.209 ,0.168 ,0.336 ,0.413 ,0.168 ,0.107 ,0.239 ,0.354 ,0.103 ,
        0.138 ,0.326 ,0.377 ,0.200 ,0.313 ,0.362 ,0.362 ,0.073 ,0.167 ,0.043
};

struct Gene                           //染色体中基因变量
{
    int loc;                         //数据块所在位置
    int num;                         //VM的数量
};

struct Chromosome                   //种群个体
{
    struct Gene Gen[B];
    double fitness;
    double rfitness;
    double cfitness;
};

struct Chromosome popcurrent[POPSIZE+1];           //当前种群

struct Chromosome popnext[POPSIZE+1];              //下一代种群

void init();

double aim();

void eval();

void elite();

void select();

void crossover();

void mutate();

bool restrain();

int iter = 0;

bool restrain(struct Gene G[])
{
    //数据处理总时间小于截止时间(数据之间有依赖)
    double time = 0;
    //cout<<"0000"<<endl;
    for(int j=0; j<B; j++)
    {
        if(G[j].num==0)
            return  false;

        //cout<<"tmp time:"<<S[j]/N[G[j].loc]+L[j][G[j].loc]+S[j]/(G[j].num*V[G[j].loc])<<endl;
        time = time + S[j]/N[G[j].loc]+L[j][G[j].loc]+S[j]/(G[j].num*V[G[j].loc]);
    }
    //cout<<"time:"<<time<<endl;
    if(time > D)
        return false;
    //cout<<"11111111111"<<endl;
    //分配的VM的个数小于或等于第i个资源域中心的VM的数量
    for(int i=0; i<W; i++)
    {
        int count = 0;

        for(int j=0; j<B; j++)
        {
            if(G[j].loc == i)
                count = count + G[j].num;
        }

        if(count > R[i])
            return false;
    }
    //cout<<"2222222222"<<endl;
    //所分配的VM的容量需大于或等于对应数据块的大小
    for(int j=0; j<B; j++)
    {
        if(S[j] > G[j].num*M[G[j].loc])
            return false;
    }
    //cout<<"3333333333333"<<endl;
    return true;
}

void init()
{
    for(int p=0; p<=POPSIZE; p++)
    {
        popcurrent[p].fitness = 0;
        popcurrent[p].rfitness = 0;
        popcurrent[p].cfitness = 0;

        do{
            for(int j=0; j<B; j++)
            {
                int addr = rand()%W;

                popcurrent[p].Gen[j].loc = addr;

                int temp = rand()%(int)(R[addr]/MUTINUM);

                while(temp == 0)
                    temp = rand()%(int)(R[addr]/MUTINUM);

                popcurrent[p].Gen[j].num = temp;

                if(popcurrent[p].Gen[j].num==0)
                    cout<<"error 0!!!!"<<endl;
            }
        }while(restrain(popcurrent[p].Gen) == false);
    }

    for(int p=0; p<=POPSIZE; p++)
    {
        for(int j=0; j<B; j++)
        {
            cout<<p<<"::"<<popcurrent[p].Gen[j].loc<<"::"<<popcurrent[p].Gen[j].num<<endl;
        }
    }

    cout<<endl<<endl;
}

double aim(struct Gene G[])
{
    double obj = 0;

    for(int j=0; j<B; j++)
    {
        int loc = G[j].loc;

        int num = G[j].num;

        obj = obj+(U[loc] + P[loc]/(num*V[loc]))*S[j];
    }

    return obj;
}

void eval()
{
    double fitness_sum = 0;

    double obj = 0;

    for(int p=0; p<POPSIZE; p++)
    {
        //计算每一个个体的目标值
        obj = aim(popcurrent[p].Gen);

        popcurrent[p].fitness = 1/(1+FITNESS_C+obj);
    }
}

void elite()
{
    int elite_individual = 0;
    int restrain_false_count=0;
    int replace_count=0;
    int replace_total;

    for(int p=0; p<POPSIZE; p++)
    {
        if((restrain(popcurrent[p].Gen)==true) && (popcurrent[p].fitness < popcurrent[elite_individual].fitness))
            elite_individual = p;
    }

    //计算restrain 为false的个数
    for(int p=0; p<POPSIZE; p++)
    {
        if(restrain(popcurrent[p].Gen)==false)
            restrain_false_count++;
    }
    //cout<<"false count: "<<restrain_false_count<<endl;

    replace_total=restrain_false_count/MUTI;


    //保留最优的种群
    if((popcurrent[elite_individual].fitness > popcurrent[POPSIZE].fitness) && (elite_individual !=0))
    {
        memcpy(&popcurrent[POPSIZE].Gen, &popcurrent[elite_individual].Gen, B*sizeof(struct Gene));

        popcurrent[POPSIZE].fitness = popcurrent[elite_individual].fitness;

        popcurrent[POPSIZE].rfitness = popcurrent[elite_individual].rfitness;

        popcurrent[POPSIZE].cfitness = popcurrent[elite_individual].cfitness;
    }

    //替换部分restrain为false
    for(int p=0;p<POPSIZE && replace_count<replace_total;p++)
    {
        if(restrain(popcurrent[p].Gen)==false)
        {
            replace_count++;

            memcpy(&popcurrent[p].Gen, &popcurrent[elite_individual].Gen, B*sizeof(struct Gene));

            popcurrent[p].fitness = popcurrent[elite_individual].fitness;

            popcurrent[p].rfitness = popcurrent[elite_individual].rfitness;

            popcurrent[p].cfitness = popcurrent[elite_individual].cfitness;
        }

    }
}

void select()
{
    double sum_fitness = 0;

    double PS;

    for(int p=0; p<POPSIZE; p++)
        sum_fitness = sum_fitness + popcurrent[p].fitness;

    for(int p=0; p<POPSIZE; p++)
        popcurrent[p].rfitness = popcurrent[p].fitness/sum_fitness;

    for(int p=0; p<POPSIZE; p++)
    {
        if(p == 0)
            popcurrent[p].cfitness = popcurrent[p].rfitness;
        else
            popcurrent[p].cfitness = popcurrent[p-1].cfitness + popcurrent[p].rfitness;
    }

    //轮盘赌选择
    for(int p=0; p<POPSIZE; p++)
    {
        PS = ((double)rand())/RAND_MAX;

        if(PS < popcurrent[0].cfitness)
        {
            memcpy(&popnext[p].Gen, &popcurrent[0].Gen, B*sizeof(struct Gene));

            popnext[p].fitness = popcurrent[0].fitness;

            popnext[p].rfitness = popcurrent[0].rfitness;

            popnext[p].cfitness = popcurrent[0].cfitness;
        }
        else
        {
            for(int p1=0; p1<(POPSIZE-1); p1++)
            {
                if((PS>=popcurrent[p1].cfitness) && (PS<popcurrent[p1+1].cfitness))
                {
                    memcpy(&popnext[p].Gen, &popcurrent[p1+1].Gen, B*sizeof(struct Gene));

                    popnext[p].fitness = popcurrent[p1+1].fitness;

                    popnext[p].rfitness = popcurrent[p1+1].rfitness;

                    popnext[p].cfitness = popcurrent[p1+1].cfitness;
                }
            }
        }
    }

    memcpy(&popnext[POPSIZE].Gen, &popcurrent[POPSIZE].Gen, B*sizeof(struct Gene));

    popnext[POPSIZE].fitness = popcurrent[POPSIZE].fitness;

    popnext[POPSIZE].rfitness = popcurrent[POPSIZE].rfitness;

    popnext[POPSIZE].cfitness = popcurrent[POPSIZE].cfitness;

    for(int p=0; p<=POPSIZE; p++)                                                             //复制，完成所有下一代种群个体的更新
    {
        memcpy(&popcurrent[p].Gen, &popnext[p].Gen, B*sizeof(struct Gene));

        popcurrent[p].fitness = popnext[p].fitness;

        popcurrent[p].rfitness = popnext[p].rfitness;

        popcurrent[p].cfitness = popnext[p].cfitness;
    }
}

void crossover()
{
    double PC = 0;                                                                 //交叉概率

    int point = 0;                                                                 //个体进行单点交叉时的交叉点

    //double random_par = 0;                                                         //个体中进行浮点数算数交叉时的权系数

    for(int p=0; p<(POPSIZE-1); p=p+2)                                                 //单点交叉
    {
        PC = ((double)rand())/RAND_MAX;

        struct Gene Gene_swap = {0, 0};

        if(PC < PXOVER)
        {
            point = rand()%B;

            for(int p1=point+1; p1<B; p1++)
            {
                memcpy(&Gene_swap, &popcurrent[p].Gen[p1], sizeof(struct Gene));

                memcpy(&popcurrent[p].Gen[p1], &popcurrent[p+1].Gen[p1], sizeof(struct Gene));

                memcpy(&popcurrent[p+1].Gen[p1], &Gene_swap, sizeof(struct Gene));
            }
        }
    }
}

void mutate()                                                            //单点变异
{
    double PM;

    for(int p=0; p<POPSIZE; p++)
    {
        for(int j=0; j<B; j++)
        {
            PM = ((double)rand())/RAND_MAX;

            if(PM < PMUTATION)
            {

                int addr = rand()% W;

                popcurrent[p].Gen[j].loc = addr;

                int temp = rand()%(int)(R[addr]/MUTINUM);

                while(temp == 0)
                    temp = rand()%(int)(R[addr]/MUTINUM);

                popcurrent[p].Gen[j].num = temp;

                if(popcurrent[p].Gen[j].num==0)
                    cout<<"error 0!!!!"<<endl;
            }
        }
    }
}

int main()
{

    //printf("%f\n",L[0][0]);
    clock_t start = clock();

    srand((int)time(NULL));

    iter = 1;

    init();

    do{

        //计算适应度
        eval();

        //精英保留
        elite();

        select();

        crossover();

        mutate();

        iter++;
    }while(iter < MAXGENS);


    cout<<"数据块id::资源中心id::VM个数\n";
    for(int j=0; j<B; j++)
        cout<<j<<"::"<<popcurrent[POPSIZE].Gen[j].loc<<""<<":"<<popcurrent[POPSIZE].Gen[j].num<<endl;

    //输出总成本
    double cost = 0.0;

    for(int j=0; j<B; j++)
    {
        int loc = popcurrent[POPSIZE].Gen[j].loc;

        int num = popcurrent[POPSIZE].Gen[j].num;

        double cost1=0.0;

        cost1 = (U[loc] + P[loc]/(num*V[loc]))*S[j];

        cout<<j<<":!!!"<<cost1<<endl;

        cost = cost+(U[loc] + P[loc]/(num*V[loc]))*S[j];
    }

    cout<<"total cost:"<< cost<<endl;
}
