/*
     dtrace -s helloWorld.d
*/
dtrace:::BEGIN {
   trace("Hello World!");
   exit(0);
}
