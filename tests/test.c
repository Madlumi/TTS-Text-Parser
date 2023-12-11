#include <stdio.h>
#include <string.h>
#define UNIT_TESTING
#include "../src/ttsp.c"
int M_TestReport(int fail, int succ){
   printf("\nRan %d Tests: %d failed, %d succeeded.\n", fail+succ , fail, succ);
   return fail;
}
int M_AssertString(char * a, char * b, int* fail, int * succ){
   if(!strcmp(a, b)){
      *succ+=1;
      return 1;
   }
   *fail+=1;
   printf ("Assert fail: [%s]==[%s]\n",a,b);
   return 0; 
}

int testStringin(){
   int  f=0;int  s=0;
   system("echo \"uwu. owo\n AWO\" aaa\". Can't, cannot\">TESTTXT");
   char ** t = splitTxt("TESTTXT");
   M_AssertString(  t[0]  , "uwu"  , &f, &s);
   M_AssertString(  t[1]  , "owo"  , &f, &s );
   M_AssertString(  t[2]  , "AWO aaa"   , &f, &s);
   M_AssertString(  t[3]  , "Can't"  , &f, &s);
   M_AssertString(  t[4]  , "cannot"  , &f, &s );
   system("rm TESTTXT");
   return M_TestReport(f, s);
}
int testparse(){
   int  f=0;int  s=0;
   M_AssertString(  ParseTtsText("uwu")  , "uwu"  , &f, &s);
   return M_TestReport(f, s);
}
int main(){
	int fail=0,succ=0;
	testStringin() ? fail++ : succ ++;
	testparse() ? fail++ : succ ++;
}
