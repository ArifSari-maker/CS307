#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
///Arif Kemal Sarı 28999
pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barr;
pthread_mutex_t mutex_sem_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_sem_B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_inB = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_inA = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condA;
pthread_cond_t condB;


int keynumber_A , keynumber_B,numberA,numberB ,sleepnum;



void sem_waitA(char *team) {
    if(*team=='A')
    {
       pthread_mutex_lock(&mutex_sem_A);
     
       pthread_mutex_unlock(&thread_lock);    
       while(!(keynumber_A>0)){
            pthread_cond_wait(&condA,&mutex_sem_A); 
       }
       pthread_mutex_lock(&mutex_inA);
       keynumber_A--;
       pthread_mutex_unlock(&mutex_inA); 
       pthread_mutex_unlock(&mutex_sem_A);
    }
    
    
}

void sem_waitB(char *team) {
    if(*team=='B')
    {
       pthread_mutex_lock(&mutex_sem_B);
     
       pthread_mutex_unlock(&thread_lock);    
       while(!(keynumber_B>0)){
            pthread_cond_wait(&condB,&mutex_sem_B);
            
       }    
       pthread_mutex_lock(&mutex_inB);
       keynumber_B--;
       pthread_mutex_unlock(&mutex_inB);  
      
       pthread_mutex_unlock(&mutex_sem_B);
    }
    
    
}


void* thread_func(void* t) {
    sleep(sleepnum);
    pthread_mutex_lock(&thread_lock);
    sleepnum = -(1/1000);
    bool driver = false;
    char* team  = (char *) t;
    
    printf("Thread ID: %ld, Team: %s, I am looking for a car\n", pthread_self(), team);

	if (*team == 'A') {
	        numberA += 1;
	        if(!(numberA<4)){
	           keynumber_A = 3;
	           for(int i=0;i<3;i++)
	           {
	           pthread_cond_signal(&condA);
	           }
	            numberA-=4;
	            driver = true;
	        }
	        else if (!(numberA<2)&&!(numberB<2)){
	           keynumber_A=2;
	           keynumber_B=2;
	            for(int i=0;i<2;i++)
	           {
	           pthread_cond_signal(&condA);
	           }
	            for(int i=0;i<2;i++)
	           {
	           pthread_cond_signal(&condB);
	           }
	           numberA-=2;
	           numberB-=2;
	           driver = true;
	        }
	        else{
	            
	          sem_waitA(team);
	        }
		
	}
	else {
	     numberB += 1;
	     if(!(numberB<4)){
	           keynumber_B = 3;
	           for(int i=0;i<3;i++)
	           {
	           pthread_cond_signal(&condB);
	           }
	           numberB-=4;
	           driver = true;
	        }
	        else if (!(numberB<2)&&!(numberA<2)){
	           keynumber_A=2;
	           keynumber_B=2;
	            for(int i=0;i<2;i++)
	           {
	           pthread_cond_signal(&condA);
	           }
	            for(int i=0;i<2;i++)
	           {
	           pthread_cond_signal(&condB);
	           }
	           numberA-=2;
	           numberB-=2;
	           driver = true;
	        }
	        else{
	           sem_waitB(team);
		}
	}    


    printf("Thread ID: %ld, Team: %s, I have found a spot in a car\n", pthread_self(), team);
    pthread_barrier_wait(&barr);
    if(driver) {
        printf("Thread ID: %ld, Team: %s, I am the captain and driving the car\n", pthread_self(), team);
	    pthread_barrier_destroy(&barr); 
	    pthread_barrier_init(&barr, NULL, 4); 
	    pthread_mutex_unlock(&thread_lock);
    }
}

int main(int argc, char* argv[]){
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int total_num = num1 + num2;

    if ((total_num % 4 != 0 ) || ( num1 % 2 != 0 ) || (num2 % 2 != 0 )){
        printf("The main terminates\n");
        return 1;
    }
    keynumber_A= 0;
    keynumber_B= 0;
    numberA = 0;
    numberB = 0;
    sleepnum = (total_num/2)/1000;
    pthread_cond_t* condition_variable_A = &condA;
    pthread_cond_t* condition_variable_B = &condB;
    pthread_mutex_init(&thread_lock, NULL);
    pthread_mutex_init(&mutex_sem_A,NULL);
    pthread_mutex_init(&mutex_sem_B,NULL);
    pthread_mutex_init(&mutex_inB,NULL);
    pthread_mutex_init(&mutex_inA,NULL);
    pthread_cond_init(condition_variable_A, NULL);
    pthread_cond_init(condition_variable_B, NULL);
    pthread_barrier_init(&barr, NULL, 4);
    

    pthread_t th[total_num];
   

    
    char* a= "A";
    char* b= "B";
    for (int i = 0; i < num2; i++) {
        if (pthread_create(&th[i], NULL, thread_func,b) != 0) {
            fprintf(stderr, "Failed to create thread\n");
            return -1;
        }
    }
    
    for (int j = total_num-num1; j < total_num; j++){
        if (pthread_create(&th[j], NULL, thread_func,a) != 0) {
            fprintf(stderr, "Failed to create thread\n");
            return -1;
        }
    }
    for (int i = 0; i < total_num; i++){
        if (pthread_join(th[i],NULL) != 0) {
            fprintf(stderr, "Failed to join thread\n");
            return -1;
        }
    }

    pthread_barrier_destroy(&barr); 
    printf("The main terminates\n");
    return 0;
}
