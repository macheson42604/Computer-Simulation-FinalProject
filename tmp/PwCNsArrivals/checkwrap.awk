# with 5 significant digits required, we can have legit difference of >1
# project requirements should specify 12 sig digs, which means an epsilon of
# 1e-6 works for reasonable Qs & t_Ks
BEGIN { 
	ENVIRON["NOCHECKWRAP"]="y"
	ENVIRON["NOCHECKGE"]="y"
	epsilon=1e-6;
}

{ 
	# make input floating point types (just in case)
	ai=$1*1.0
	aitk=$2*1.0
	q=int(ai/tK);  # both >0 so int() is same as floor
	x=ai-q*tK;     # x should be equal to aitk
	# :( no abs in my pedantic awk
	t=sqrt((x-aitk)*(x-aitk));
	if( t > epsilon && ENVIRON["NOCHECKWRAP"] !~ /.+/ ) { 
		printf( "\nerror arrival number %d: file a_i=%s and a_i-mod-t_K=%s; q=%lf, calculated a_i-q*tK=%lf ai-mod-tk=%lf difference=%lf\n", FNR, $1, $2, q, x, aitk, t ) > "/dev/stderr";
		printf( "\nerror arrival number %d: %s mod %lf = %lf =/= %s\n", FNR, $1, tK, x, $2 ) > "/dev/stderr"; 
		exit 1; 
	}
	if( aitk > tK+epsilon && ENVIRON["NOCHECKGE"] !~ /.+/ ) { 
		printf( "\nerror arrival number %d: file a_i-mod-t_K=%s > %lf\n", FNR, $2, tK ) > "/dev/stderr"; 
		exit 1; 
	} else {
		# we print only the aitk term, as this is what we want
	    # to plot and grade
		print $2;
	}
}

END { exit 0; }
