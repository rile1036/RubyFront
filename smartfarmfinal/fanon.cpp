  1 #include "fanon.h"
  2 #include "rgbtest.h"
  3 void sig_handler(int signo);
  4
  5
  6 void sig_handler(int signo)
  7 {
  8     printf("process stop\n");
  9   digitalWrite (FAN, 0) ; // Off
 10   digitalWrite (RED, 0) ;
 11   digitalWrite (GREEN, 0);
 12   digitalWrite (BLUE, 0);
 13   digitalWrite (RGBLEDPOWER, 0);
 14   exit(0);
 15 }
