#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void* print_arg(void* data) 
{ 
      char *str; 
      str = (char*)data; 
      printf("%s\n", str); 

      sleep(20);

      pthread_exit(NULL);
} 
  
int main(int argc, char *argv[]) 
{ 
      if (argc < 3) {
          printf("usage : %s arg1 arg2\n", "labthread1");
          return 1;
      }

      pthread_t t1, t2; 
  
      pthread_create(&t1, NULL, print_arg, (void*)argv[1]); 
      pthread_create(&t2, NULL, print_arg, (void*)argv[2]); 
      
      pthread_join(t1, NULL); 
      pthread_join(t2, NULL); 
 
      return 0;
}