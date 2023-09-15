#include <stdio.h>
#include <stdlib.h>

/* 事件队列 */
typedef struct daos_eq {
	int			use_lock;
	int			is_polling;
	int			eq_fd;
} daos_eq_t;



int daos_eq_create(void **eq_out){
  daos_eq_t *eq = NULL;
  eq = (struct daos_eq *)malloc(sizeof(daos_eq_t));
  eq->eq_fd = 100;
  printf("eq:%p, fd:%d\n", eq, eq->eq_fd);
  *eq_out = (void*)eq;
  return 0;
}

int main(){
  int    rc = 0;
  void  *eq = NULL;

  
  printf("eq1:%p\n", eq);
  daos_eq_create(&eq);
  
  printf("eq2:%p\n", eq);
  daos_eq_t *eq1 = (daos_eq_t *)eq;
  printf("eq3:%p, fd:%d\n", eq1, eq1->eq_fd);
  eq1->eq_fd = 200;
  printf("eq4:%p, fd:%d\n", eq1, eq1->eq_fd);
  return 0;
}

// gcc void.c -o main;./main