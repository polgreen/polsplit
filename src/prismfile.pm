mdp 

const double a;


module test 

 // local state 
 s: [0..3] init 0;
 
[act0] s=0 -> 1*a : (s'=1) + (1-1*a):(s'=3);
[act1] s=0 -> 1*a : (s'=1) + (1-1*a):(s'=3);
[act2] s=1 -> 1*a : (s'=2) + (1-1*a):(s'=3);
[act3] s=2 -> 1 : (s'=2) ;
[act4] s=3 -> 1 : (s'=3) ;

endmodule 
 
label "complete"=(s=2);
