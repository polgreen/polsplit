mdp

const double a;

module testmdp

	// local state
	s : [0..3] init 0;
		
	
	[] s=0 -> 1 : (s'=1) ;
	[] s=1 -> a : (s'=2) + 1-a: (s'=3);
	[] s=1-> 0.7 : (s'=0) + 0.3: (s'=3);
	[] s=2 -> 1 : (s'=2) ;
	[] s=3 -> 1 : (s'=3) ;
		
	
endmodule

label "complete"=(s=3);