dtmc 

const double a;
const double b;


module test 

 // local state 
 s: [0..6] init 0;
 
[] s=0 -> 1/10*a + 1/10*b : (s'=1) + (1-1/10*a-1/10*b):(s'=6);
[] s=1 -> 1/2 : (s'=2)  + (1-1/2):(s'=0);
[] s=2 -> 1/2 : (s'=3)  + (1-1/2):(s'=1);
[] s=3 -> 1/2 : (s'=4)  + (1-1/2):(s'=2);
[] s=4 -> 1/2 : (s'=5)  + (1-1/2):(s'=3);
[] s=5 -> 1 : (s'=0) ;
[] s=6 -> 1 : (s'=0) ;

endmodule 
 
label "complete"=(s=2);
