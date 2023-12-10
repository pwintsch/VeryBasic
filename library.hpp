#ifndef LIBRARY_H_
#define LIBRARY_H_




int test1();
int test2();            /*  Prototypes */
int test3();


/* array with three functions */
extern int (*functptr[])();

extern int (*DirectCommandPtr[])();

#endif /* LIBRARY_H_ */