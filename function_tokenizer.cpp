#include <string>
#include "function_tokenizer.h"
#include <iostream>

bool isseperator (char letter)
{
  return letter == '(' || letter ==')' || letter=='=' ||letter=='<'||letter=='>'
  ||letter=='x'||letter=='*' ||letter=='|'|| letter == '/' || letter=='[' || letter==']' ;
}

void token(tokent token)
{
  switch(token.kind){
  case TIMES: std::cout<<" x ";break; 
  default:;
  }
}

void emptybuffer(std::string &buffer, std::vector<tokent> &tokenseq)
{
  tokent t;
  if(buffer=="U")
  {
    t.kind = UNTIL; tokenseq.push_back(t);
  }
  else if (buffer=="P"){
    t.kind = PROB; tokenseq.push_back(t);
  }
  else if (buffer=="F"){
   t.kind = FI; tokenseq.push_back(t);
  }
  else if (buffer=="G"){
  t.kind = GL; tokenseq.push_back(t); 
  }
  else if (buffer=="X"){
  t.kind = X; tokenseq.push_back(t); 
  }
  else if(buffer=="min"){
    t.kind=MIN; tokenseq.push_back(t);
  }
  else if (buffer=="max"){
    t.kind=MAX; tokenseq.push_back(t);
  }
  else{
    t.kind=IDENTIFIER;
    t.label=buffer;
    tokenseq.push_back(t);
  }

  buffer.clear();
}

std::vector<tokent>  pctl_tokenizer(std::string input)
{
  std::vector<tokent> tokenseq;
  std::string buffer;
  tokent t;
  if(input.size()==0)
    {std::cout<<"Empty string \n";
    throw std::exception();}
  std::string::iterator pos =input.begin();
  while(pos!=input.end())
  {
    if(isseperator(*pos)) {
      if(buffer.empty()){
        
        switch(*pos)
        {
          case 'x': t.kind = TIMES; tokenseq.push_back(t);break;
          case '*': t.kind=TIMES; tokenseq.push_back(t); break;
          case'-': t.kind=SUBTRACT; tokenseq.push_back(t);break;
          case'+': t.kind=PLUS; tokenseq.push_back(t); break;
          case '/': t.kind=DIVIDE; tokenseq.push_back(t); break;
          case'|': t.kind=DIVIDE; tokenseq.push_back(t); break;
          case'(': t.kind=LP; tokenseq.push_back(t); break;
          case')': t.kind=RP; tokenseq.push_back(t); break;
          case'[': t.kind=SLP; tokenseq.push_back(t); break;
          case']': t.kind=SRP; tokenseq.push_back(t); break;
          default:;
        }
        pos++;
      }
      else{
             emptybuffer(buffer, tokenseq) ;
      }
      
    }
    else{
    buffer+= *pos;
    pos++;
    }
    
    
  }
  if(!buffer.empty()){
          emptybuffer(buffer,tokenseq);
          }
return tokenseq;      
}