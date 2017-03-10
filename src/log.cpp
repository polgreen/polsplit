// log.cpp;

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include <iostream>
bool LogCreated = false;


void Log (char *message)
{
  FILE *file;
  std::cout<<"logging\n";
  if (!LogCreated) {
    {
      file = fopen(LOGFILE, "a");
      LogCreated = true;
    }
  }
  else
    file = fopen(LOGFILE, "a");

  if (file == NULL) {
    if (LogCreated)
      LogCreated = false;
    return;
  }
  else
  {
    fputs(message, file);
    fclose(file);
  }

  if (file)
    fclose(file);
}


