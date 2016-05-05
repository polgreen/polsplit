#include <string>
#include "pctl_tokenizer.h"
#include <iostream>

bool isseperator (char letter)
{
  return letter == '(' || letter ==')' || letter=='=' ||letter=='<'||letter=='>'
  ||letter=='|'||letter=='&' ||letter==' '|| letter == '"' || letter=='[' || letter==']' 
  || letter=='!' || letter=='?';
}

void token(tokent token)
{
  switch(token.kind){
  case PROB: std::cout<<"prob ";break;
  case UNTIL: std::cout<<"until ";break;
  case IDENTIFIER: std::cout<<"IDENTIFIER "<<token.label << " "; fflush(stdout); break;
  case NOT: std::cout<<"NOT ";break;
  case AND: std::cout<<"AND ";break;
  case OR: std::cout<<"OR ";break;
  case GT: std::cout<<"GT ";break;
  case LT: std::cout<<"LT ";break;
  case EQ:std::cout<<"EQ ";break;
  case LP: std::cout<<"LP "; break;
  case RP: std::cout<<"RP "; break;
  case SLP: std::cout<<"SLP "; break;
  case SRP: std::cout<<"SRP "; break;
  case FI: std::cout<<"F "; break;
  case GL: std::cout<<"G "; break;
  case X: std::cout<<"X "; break;
  case MIN: std::cout<<"min "; break;
 case MAX: std::cout<<"max "; break;
 case QUE: std::cout<<"? "; break;
 case GE: std::cout<<"GE "; break;
 case LE: std::cout<<"LE "; break;
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
  std::string::iterator pos =input.begin();
  while(pos!=input.end())
  {
    if(isseperator(*pos)) {
      if(buffer.empty()){
        
        switch(*pos)
        {
          case '!': t.kind = NOT; tokenseq.push_back(t);break;
          case '&': t.kind = AND; tokenseq.push_back(t);break;
          case '|': t.kind = OR; tokenseq.push_back(t);break;
          case '>': pos++;
          if(pos!= input.end() && *pos=='=')
          { t.kind=GE; 
            tokenseq.push_back(t); break;}
          else {t.kind = GT; tokenseq.push_back(t); pos--;break;}
          case '<': pos++;
          if(pos!= input.end() && *pos=='=')
          {t.kind=LE; tokenseq.push_back(t); break;}
          else {t.kind = LT; tokenseq.push_back(t); pos--;break;}
          case '=': pos++;
          if(pos!= input.end() && *pos=='>')
          {t.kind=IMPLIES; tokenseq.push_back(t); break;}
          else {t.kind = EQ; tokenseq.push_back(t); pos--;break;}

          case '(': t.kind = LP; tokenseq.push_back(t); break;
          case ')': t.kind = RP; tokenseq.push_back(t); break;
          case '[': t.kind = SLP; tokenseq.push_back(t); break;
          case ']': t.kind = SRP; tokenseq.push_back(t); break;
          case '?': t.kind=QUE; tokenseq.push_back(t); break;
          case ' ': ;break;
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