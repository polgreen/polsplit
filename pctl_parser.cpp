#include "pctl_parser.h"
#include <vector>
#include <iostream>
#include <exception>
pctlformula parse (std::vector<tokent> &tokenseq);
pctlformula parseprimary(std::vector<tokent> &tokenseq);


void outputproperty(pctlformula f, std::ostream &out)
{
	switch(f.t.kind)
 	{
 	case IDENTIFIER: out<< '\"'<<f.t.label<<'\"'; break;
 	case UNTIL:out<<"("; outputproperty(f.children[0], out); out<< " U "; outputproperty(f.children[1], out);out<<")"; break;
 	case BUNTIL:out<<"("; outputproperty(f.children[0], out); out<< " U"; outputproperty(f.children[2], out);
 	outputproperty(f.children[3], out);
 	outputproperty(f.children[1], out);out<<")"; break;
	case AND:  out<<"("; outputproperty(f.children[0], out); out<< " & "; outputproperty(f.children[1], out); out<<")";break;
 	case OR:    out << "("; outputproperty(f.children[0], out); out<< " | "; outputproperty(f.children[1], out); out << ")"; break;
 	case PROB: out<<"P"; outputproperty(f.children[0],out); outputproperty(f.children[1], out); 
 	outputproperty(f.children[2], out); out<<"[";outputproperty(f.children[3], out); out<<"]";break;
	case NOT: out<<"!"; outputproperty(f.children[0], out); break;
	case GT: out<<">"; outputproperty(f.children[0], out);break;
 	case LT: out<<"<";outputproperty(f.children[0], out);break;
 	case EQ:out<<"=";outputproperty(f.children[0], out);break;
 	case LP: out<<"("; break;
 	case RP: out<<")"; break;
 	case SP: out<<" "; break;	
 	case FI: out<<"F "; outputproperty(f.children[0], out); break;
 	case GL: out<<"G "; outputproperty(f.children[0], out); break;
 	case X: out<<"X"; outputproperty(f.children[0], out); break;
 	case MIN: out<<"min"; break;
 	case MAX: out<<"max"; break;
 	case QUE: out<<"?"; break;
 	case IMPLIES: out<<"implies"; break;
 	case GE: out<<">="; break;
 	case LE: out<<"<="; break;

 default:;
 }
}

void output(pctlformula f)
{
 switch(f.t.kind)
 {
 case IDENTIFIER: std::cout<< ' '<<f.t.label<<' '; break;
 case UNTIL:std::cout<<"("; output(f.children[0]); std::cout<< " U "; output(f.children[1]);std::cout<<")"; break;
 case BUNTIL:std::cout<<"("; output(f.children[0]); std::cout<< " U"; output(f.children[2]); output(f.children[3]);
 	output(f.children[1]);std::cout<<")"; break;
case AND:  std::cout<<"("; output(f.children[0]); std::cout<< " & "; output(f.children[1]); std::cout<<")";break;
 case OR:    std::cout << "("; output(f.children[0]); std::cout<< " | "; output(f.children[1]); std::cout << ")"; break;
 case PROB: std::cout<<"P"; output(f.children[0]); output(f.children[1]); 
 	output(f.children[2]); std::cout<<"[";output(f.children[3]); std::cout<<"]";break;
 case NOT: std::cout<<"!"; output(f.children[0]); break;
 case GT: std::cout<<">"; output(f.children[0]);break;
 case LT: std::cout<<"<";output(f.children[0]);break;
 case EQ:std::cout<<"=";output(f.children[0]);break;
 case LP: std::cout<<"("; break;
 case RP: std::cout<<")"; break;
 case SP: std::cout<<" "; break;	
 case FI: std::cout<<"F "; output(f.children[0]); break;
 case GL: std::cout<<"G "; output(f.children[0]); break;
 case X: std::cout<<"X"; output(f.children[0]); break;
 case MIN: std::cout<<"min"; break;
 case MAX: std::cout<<"max"; break;
 case QUE: std::cout<<"?"; break;
 case IMPLIES: std::cout<<"implies"; break;
 case GE: std::cout<<">="; break;
 case LE: std::cout<<"<="; break;

 default:;
 }
}

void outputtoken(std::vector<tokent> &tokenseq)
{
	      for (unsigned i=0; i<tokenseq.size(); i++)
        {
            token(tokenseq.at(i));
        }
        std::cout<<"\n";
}

pctlformula parseprob(std::vector<tokent> &tokenseq)
{
	std::cout<<"parseprob \n";
	outputtoken(tokenseq);
	pctlformula f, f0, f1, f2, f3;
	f.t = tokenseq.front();
	tokenseq.erase(tokenseq.begin());
	if (tokenseq.empty())
	{
		std::cout<<"expected bounds after P \n";
		throw std::exception();
	}
		else if (tokenseq.front().kind==GE || tokenseq.front().kind==LE ||
		tokenseq.front().kind==GT || tokenseq.front().kind==LT)
	{
		std::cout<<"Got to line 98 \n";
		f0.t.kind=tokenseq.front().kind;
		std::cout<<"Got to line 100 \n";
		tokenseq.erase(tokenseq.begin());
		std::cout<<"Got to line 102 \n";
		if(tokenseq.empty())
			{std::cout<<"expected identifier after P>=, P>, P< or P<=\n";
			throw std::exception();}
		else{
			f1 = parseprimary(tokenseq);
			std::cout<<"Got to line 108 \n";
			f.children.resize(4);
			f.children[0] = f0;
			f.children[1] = f1;
			f.children[2].t.kind=SP; 
			std::cout<<"Got to line 113 \n";
			if(!tokenseq.empty() && tokenseq.front().kind==SLP)
			{
				tokenseq.erase(tokenseq.begin());
				f3 = parse(tokenseq);
				f.children[3]=f3;

				if(!tokenseq.empty() && tokenseq.front().kind==SRP)
				{
					tokenseq.erase(tokenseq.begin());
					return f;
				}
				else{std::cout<<"missing SRP\n";
					throw std::exception();}
			}
			else{std::cout<<"missing SLP\n";
				throw std::exception();}

		}
	}
	else if (tokenseq.front().kind==MIN || tokenseq.front().kind==MAX)
	{
		f0.t.kind=tokenseq.front().kind;
		tokenseq.erase(tokenseq.begin());
		if(tokenseq.empty() || tokenseq.front().kind!=EQ)
			{std::cout<<"expected = after P min or P max\n";
			throw std::exception();}
		else{
			f1.t.kind=EQ;
			tokenseq.erase(tokenseq.begin());
			if(tokenseq.empty() || (tokenseq.front().kind!=QUE && tokenseq.front().kind!=IDENTIFIER))
			{std::cout<<"expected ? or identifier after P min or P max\n";
			throw std::exception();}
			else{
			 f2.t=tokenseq.front();	
			 tokenseq.erase(tokenseq.begin());

			 f.children.resize(4);
			 f.children[0] = f0;
			 f.children[1] = f1;
			 f.children[2] = f2;
			 if(!tokenseq.empty() && tokenseq.front().kind==SLP)
			   {
				tokenseq.erase(tokenseq.begin());
				f3 = parse(tokenseq);
				f.children[3]=f3;

				if(!tokenseq.empty() && tokenseq.front().kind==SRP)
				 {
					tokenseq.erase(tokenseq.begin());
					return f;
				 }
				 else{std::cout<<"missing SRP\n";
					throw std::exception();}
			   }
			  else{std::cout<<"missing SLP\n";
				throw std::exception();}

		     } 

	       }
	 } 
	 else{std::cout<<"expecting MIN or MAX or > or < after PROB\n";
		throw std::exception();}     

}

pctlformula parseprimary(std::vector<tokent> &tokenseq)
{
	std::cout<<"parseprimary \n";
	outputtoken(tokenseq);
	pctlformula f;
	if(tokenseq.front().kind==IDENTIFIER)
	{
		f.t = tokenseq.front();
		tokenseq.erase(tokenseq.begin());
		return f;
	}
	else if(tokenseq.front().kind == NOT)
	{
		f.t =tokenseq.front();
		tokenseq.erase(tokenseq.begin());
		pctlformula f0 = parseprimary(tokenseq);
		f.t.kind = NOT;
		f.children.resize(1);
	 	f.children[0] = f0;
	 	return f;
	}

	else if (tokenseq.front().kind==LP)
	{
		tokenseq.erase(tokenseq.begin());
		f = parse(tokenseq);
		if (tokenseq.front().kind == RP)
		{
			tokenseq.erase(tokenseq.begin());
			return f;
		}
		else{
			std::cout<< "missing right parenthesis\n";
			throw std::exception();
		}
	}

	else if (tokenseq.front().kind==PROB)
	{
		return parseprob(tokenseq);
	}

	else if (!tokenseq.empty()){
		switch(tokenseq.front().kind){
				case UNTIL: std::cout<<"UNTIL must be preceeded by an identifier\n"; break;
				case AND: std::cout<<"AND must be preceeded by an identifier\n"; break;
				case OR: std::cout<<"OR must be preceeded by an identifier\n"; break;
				default: ;
			}
			throw std::exception();
			
	}	
	else
		{std::cout<<"empty tokenseq\n";
		throw std::exception();}

}

pctlformula parseconjunction(std::vector<tokent> &tokenseq)
{
	std::cout<<"parseconjunction \n";
	outputtoken(tokenseq);
	pctlformula f0;
	f0 = parseprimary(tokenseq);
	if(tokenseq.front().kind==AND)
	{
	 pctlformula f1;
	 tokenseq.erase(tokenseq.begin());
	 if(tokenseq.empty())
	 {
		std::cout<<"error. Expected identifier after AND\n";
		throw std::exception();
		return f0;
	 }
	 else{
	  f1 = parseconjunction(tokenseq);
	  pctlformula f;
	  f.t.kind = AND;
	  f.children.resize(2);
	  f.children[0] = f0;
	  f.children[1] = f1;
	  return f;
	 }
	}
	else
	{
		return f0;
	}


}

pctlformula parsedisjunction(std::vector<tokent> &tokenseq)
{
	std::cout<<"parsedisjunction \n";
	outputtoken(tokenseq);

	pctlformula f0;
	f0 = parseconjunction(tokenseq);
	if(tokenseq.front().kind==OR)
	{
	pctlformula f1;
	tokenseq.erase(tokenseq.begin());
	if(tokenseq.empty())
	{
		std::cout<<"error. Expected identifier after OR\n";
		throw std::exception();
	}
	else{
	 f1 = parsedisjunction(tokenseq);
     pctlformula f;
	 f.t.kind = OR;
	 f.children.resize(2);
	 f.children[0] = f0;
	 f.children[1] = f1;
	 return f;
		}
	} 
	else
	{
		return f0;
	}
}





pctlformula parseFG(std::vector<tokent> &tokenseq)
{
	std::cout<<"parseFG \n";
	outputtoken(tokenseq);

	pctlformula f0;
	pctlformula f1;
	pctlformula f;

	if(tokenseq.front().kind==FI)
	{
	 tokenseq.erase(tokenseq.begin());
	  if(tokenseq.empty())
	   {
		std::cout<<"error. Expected identifier after FINALLY \n";
		throw std::exception();
	   }
	  else{

	   
		f1 = parseFG(tokenseq);
		f.t.kind = FI;
		f.children.resize(1);
	 	f.children[0] = f1;	

	   return f;
	   }
	}
	else if(tokenseq.front().kind==GL)
	{
	  tokenseq.erase(tokenseq.begin());
	  if(tokenseq.empty())
	   {
		std::cout<<"error. Expected identifier after GLOBALLY \n";
		throw std::exception();
	   }
	  else
	  {
	   f1 = parseFG(tokenseq);
	   f.t.kind = GL;
	   f.children.resize(1);
	   f.children[0] = f1;	
	   return f;
	   }
	 } 
	else if(tokenseq.front().kind==X)
	{
	  tokenseq.erase(tokenseq.begin());
	  if(tokenseq.empty())
	   {
		std::cout<<"error. Expected identifier after X \n";
		throw std::exception();
	   }
	  else
	  {
	   f1 = parseFG(tokenseq);
	   f.t.kind = X;
	   f.children.resize(1);
	   f.children[0] = f1;	
	   return f;
	   }
	 } 	
	else
	{
		f0 = parsedisjunction(tokenseq);
		return f0;
	}
}

pctlformula parseuntil(std::vector<tokent> &tokenseq)
{
	std::cout<<"parseuntil \n";
	 outputtoken(tokenseq);
	pctlformula f,f0,f1,f2,f3;
	f0 = parseFG(tokenseq);
	if(tokenseq.front().kind==UNTIL)
	{
	 tokenseq.erase(tokenseq.begin());
	 if(tokenseq.empty())
	  {
		std::cout<<"error. Expected identifier after UNTIL \n";
		throw std::exception();
	  }
	 else if(tokenseq.front().kind==GT || tokenseq.front().kind==GE)
	 {
	 	std::cout<<"error: GT and GE not allowed after UNTIL \n";
	 	throw std::exception();
	 }
	 else if (tokenseq.front().kind==LT || tokenseq.front().kind==LE)
	 {
	 	f2.t.kind=tokenseq.front().kind;
		tokenseq.erase(tokenseq.begin());
		if(!tokenseq.empty() && tokenseq.front().kind==IDENTIFIER)
			{
			//outputtoken(tokenseq);
			f3 = parseprimary(tokenseq);
			//outputtoken(tokenseq);

			if(tokenseq.empty())
			{std::cout<<"Error: expected something after UNTIL \n";
			throw std::exception();}
			else
			{
		//	outputtoken(tokenseq);
			f1 = parseFG(tokenseq);
			f.t.kind=BUNTIL;
			f.children.resize(4);
			f.children[0] = f0;
			f.children[1] = f1;
			f.children[2] = f2;
			f.children[3] = f3; 
			return f;
			}
		}
		else{std::cout<<"expected identifier after U>=, U>, U<, or U<= \n";
			throw std::exception();}
	 }
	 else
	  {
	  	f1=parseFG(tokenseq);
	  	f.t.kind=UNTIL;
	  	f.children.resize(2);
	  	f.children[0]=f0;
	  	f.children[1]=f1;
	  	return f;
	  }
	 }

	 else
	 {
	 	return f0;
	 }
	
}



pctlformula parse (std::vector<tokent> &tokenseq)
{
	
pctlformula f;
std::cout<<"parse\n";
outputtoken(tokenseq);
if(tokenseq.empty())
{
	std::cout<<"parse error: empty tokenseq\n";
	throw std::exception();
}
else{
 f =  parseuntil(tokenseq);

 return f;
}
}

pctlformula parse_top (std::vector<tokent> &tokenseq)
{
pctlformula f;
std::cout<<"visited parse_top \n";
 outputtoken(tokenseq);
 f =  parse(tokenseq);

  if(!tokenseq.empty())
  {
	std::cout<<"parse error: tokens after end of expression \n";
	throw std::exception();
	outputtoken(tokenseq);
	  }

 return f;

}