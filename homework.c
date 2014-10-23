/* 
 * file:        homework.c
 * description: Skeleton code for CS 5600 Homework 2
 *
 * Peter Desnoyers, Northeastern CCIS, 2011
 * $Id: homework.c 530 2012-01-31 19:55:02Z pjd $
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hw2.h"

/********** YOUR CODE STARTS HERE ******************/

/*
 * Here's how you can initialize global mutex and cond variables
 */
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t C_Barber = PTHREAD_COND_INITIALIZER;
pthread_cond_t C_Customer = PTHREAD_COND_INITIALIZER;
pthread_cond_t C_Haircut = PTHREAD_COND_INITIALIZER;
int number_of_customers = 0;
int max_customers = 5;
int time_delay = 3; //average time between customer() calls 

int total_customers = 0;
int total_turned_away = 0;

/* the barber method
 */
void barber(void)
{
    pthread_mutex_lock(&m);
    while (1) {
        /* your code here */
      if (number_of_customers == 0) {
	printf("DEBUG: %f barber goes to sleep\n", timestamp());
	pthread_cond_wait(&C_Barber, &m);
	printf("DEBUG: %f barber wakes up\n", timestamp());
      }
      // add a sleep
      double T = 1.2;
      sleep_exp(T, &m);
      pthread_cond_signal(&C_Haircut);
    }
    pthread_mutex_unlock(&m);
}

/* the customer method
 */
void customer(int customer_num)
{
    pthread_mutex_lock(&m);
    /* your code here */
    printf("DEBUG: %f customer %d enters shop\n", timestamp(), customer_num);
    ++total_customers;
    if (number_of_customers >= max_customers) {
      //printf("DEBUG: %f customer %d exits: shop is full\n", timestamp(), customer_num);
      ++total_turned_away;
    } 
    else {
      ++number_of_customers;
      if (number_of_customers > 1) {
	pthread_cond_wait(&C_Customer, &m);
      }
      pthread_cond_signal(&C_Barber);
      printf("DEBUG: %f customer %d starts haircut\n", timestamp(), customer_num);
      pthread_cond_wait(&C_Haircut, &m);
      --number_of_customers;

      // indicate to other customers (if any) that there are no customers

      pthread_cond_signal(&C_Customer);
    }
    printf("DEBUG: %f customer %d leaves shop\n", timestamp(), customer_num);
    pthread_mutex_unlock(&m);
}

/* Threads which call these methods. Note that the pthread create
 * function allows you to pass a single void* pointer value to each
 * thread you create; we actually pass an integer (the customer number)
 * as that argument instead, using a "cast" to pretend it's a pointer.
 */

/* the customer thread function - create 10 threads, each of which calls
 * this function with its customer number 0..9
 */
void *customer_thread(void *context) 
{
    int customer_num = (int)context; 

    /* your code goes here */
    while(1) {
      //double sleep_time = rand() % time_delay;
      sleep_exp(10.0, NULL);
      customer(customer_num);
    }
    return 0;
}

/*  barber thread
 */
void *barber_thread(void *context)
{
    barber(); /* never returns */
    return 0;
}

void q2(void)
{
    /* to create a thread:
        pthread_t t; 
        pthread_create(&t, NULL, function, argument);
       note that the value of 't' won't be used in this homework
    */

    /* your code goes here */
  pthread_t t_barber;
  pthread_create(&t_barber, NULL, barber_thread, 0);
  
  pthread_t t_customer[10];
  int i;
  
  for(i = 0; i < 10; i++) {
    pthread_create(&t_customer[i], NULL, customer_thread, (void*)i);
  }
  wait_until_done();
}

/* For question 3 you need to measure the following statistics:
 *
 * 1. fraction of  customer visits result in turning away due to a full shop 
 *    (calculate this one yourself - count total customers, those turned away)
 * 2. average time spent in the shop (including haircut) by a customer 
 *     *** who does not find a full shop ***. (timer)
 * 3. average number of customers in the shop (counter)
 * 4. fraction of time someone is sitting in the barber's chair (counter)
 *
 * The stat_* functions (counter, timer) are described in the PDF. 
 */

void q3(void)
{
    /* your code goes here */
  q2();
  printf("Fraction of customers turned away: %f\n", 1.0*total_turned_away/total_customers);
}
