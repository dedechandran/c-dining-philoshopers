/*Nama : Dede Chandra Nugraha*/
/*Nim : 161524006*/
/*Mata Kuliah : Sistem Terdistribusi*/
/*Deskripsi : Dining Philosophers using pthread*/


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

//inisialisasi banyaknya philsup
#define NUMBER_PHILOSOPHER 5
pthread_mutex_t forks[NUMBER_PHILOSOPHER];

//inisialisasi mutex (forks yang dipakai untuk makan para philsup)
void initMutex(){
    for(int i=0;i<NUMBER_PHILOSOPHER;i++){
        pthread_mutex_init(&forks[i],NULL);
    }
}

void destroyMutex(){
    for(int i=0;i<NUMBER_PHILOSOPHER;i++){
        pthread_mutex_destroy(&forks[i]);
    }
}

//philsup makan
void eating(long i){
    printf("Philosopher %ld sedang makan !\n",i);
    sleep(rand()%5);
}

//philsup tidur
void sleeping(long i,long pos){
    printf("Philosopher %ld tidur karena tidak mendapatkan garpu ke %ld!\n",i,pos);
    sleep(rand()%5);
}

//philsup lapar
void hungry(long i){
    printf("Philosopher %ld lapar !\n",i);
    long LEFT = (i+1)%NUMBER_PHILOSOPHER;
    long RIGHT = i;
    //philsup mencoba mengambil garpu yang ada dikanan nya
    //jika gagal, philsup akan tidur
    if(pthread_mutex_trylock(&forks[RIGHT])==0){
        printf("Philosopher %ld berhasil mengambil garpu %ld \n",i,RIGHT);
        //philsup mencoba mengambil garpu yang ada dikirinya
        //jika gagal, philsup menyimpan garpu kanannya dan tidur
        if(pthread_mutex_trylock(&forks[LEFT])){
            printf("Philosopher %ld berhasil mengambil garpu %ld \n",i,LEFT);
            //philsup berhasil mengambil kedua garpu dan makan
            eating(i);
            //philsup selesai makan
            printf("Philosopher %ld selesai makan !\n",i);
            //philsup menyimpan kembali garpu yang ada dikanannya
            pthread_mutex_unlock(&forks[RIGHT]);
            //philsup menyimpan kembali garpu yang ada dikirinya
            pthread_mutex_unlock(&forks[LEFT]);
        }else{
            //philsup menyimpan kembali garpu yang ada dikanannya
            pthread_mutex_unlock(&forks[RIGHT]);
            sleeping(i,LEFT);
        }
    }else{
        sleeping(i,RIGHT);
    }
}

//philsup berpikir
void thinking(long i){
    printf("Philosopher %ld sedang berpikir !\n",i);
    sleep(rand()%5);
}

//philsup makan malam
void *dining(void *i){
    long tid = (long) i;
    while(1){
        thinking(tid);
        hungry(tid);
    }
}

int main(int argc, char const *argv[])
{
    // NUMBER_PHILOSOPHER = atoi(argv[1]);

    //inisialisasi thread yang merepresentasikan philosophers.
    pthread_t philosophers[NUMBER_PHILOSOPHER];
    int rc;

    //inisialisasi mutex (forks yang dipakai untuk makan para philsup)
    initMutex();
    for(long i=1;i<NUMBER_PHILOSOPHER;i++){
        rc = pthread_create(&philosophers[i],NULL,dining,(void *) i);
        if(rc){
            printf("ERROR , return code is %d",rc);
            exit(-1);
        }
    }

    while(1){
        thinking(0);
        hungry(0);
    }

    // //tunggu sampai semua thread selesai melakukan prosesnya
    // for(int j=0;j<NUMBER_PHILOSOPHER;j++){
    //     pthread_join(philosophers[j],NULL);
    // }

    //destroy mutex
    destroyMutex();


    return 0;
}
