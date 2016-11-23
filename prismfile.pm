dtmc 

const double a;


module test 

 // local state 
 s: [0..3] init 0;
 
[] s=0 -> 1*a : (s'=1) + (1-1*a):(s'=0);
[] s=1 -> 1*a : (s'=2) + (1-1*a):(s'=3);
[] s=2 -> 1 : (s'=0) ;
[] s=3 -> 1/2 : (s'=0)  + (1-1/2):(s'=2);

endmodule 
 
label "complete"=(s=2);
