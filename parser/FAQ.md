while compiling grammar:
  Error: A JNI error has occurred, please check your installation and try again
Exception in thread "main" java.lang.UnsupportedClassVersionError: org/antlr/v4/Tool has been compiled by a more recent version of the Java Runtime (class file version 55.0), this version of the Java Runtime only recognizes class file versions up to 52.0
  see https://javaalmanac.io/bytecode/versions/
  this antlr-4.12.0-complete.jar was compiled by Java 11, and you have Java 8, (maybe together with Java 11 an above)
try java -version: for example: 'openjdk version "1.8.0_362"'
  Instead of remove , one can switching between versions 'sudo update-alternatives --config java'
 

