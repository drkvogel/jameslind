	program simple_rand_main

	implicit none

	integer nrand, nranda, nrandb
	integer fact1_1a,fact1_1b,fact1_2a,fact1_2b,fact2_1a,
     *  fact2_1b,fact2_2a,fact2_2b,fact2_3a,fact2_3b,fact3_1a,
     *  fact3_1b,fact3_2a,fact3_2b,fact3_3a,fact3_3b,fact3_4a,
     *  fact3_4b,factx_1a,factx_1b,factx_2a,factx_2b
	real propx
	character clabel*15


c set the argunments to be passed into program

	nrand=200
	propx=6
	clabel='Very Anxious'
c	clabel=' '

c call prog

	call simplerand(nrand,clabel,propx,nranda,nrandb, 
     *fact1_1a,fact1_1b,fact1_2a,fact1_2b,fact2_1a,fact2_1b,
     *fact2_2a,fact2_2b,fact2_3a,fact2_3b,fact3_1a,fact3_1b,
     *fact3_2a,fact3_2b,fact3_3a,fact3_3b,fact3_4a,
     *fact3_4b,factx_1a,factx_1b,factx_2a,factx_2b)

c write the data out to screen

	write(6,1000)nranda,nrandb
	write(6,1001)fact1_1a,fact1_1b,fact1_2a,fact1_2b
	write(6,1002)fact2_1a,fact2_1b,fact2_2a,fact2_2b,
     c             fact2_3a,fact2_3b
	write(6,1003)fact3_1a,fact3_1b,fact3_2a,fact3_2b,
     c             fact3_3a,fact3_3b,fact3_4a,fact3_4b
	if(clabel.ne.' ')then
	  write(6,1004)clabel,factx_1a,factx_1b,factx_2a,factx_2b
	endif

1000  format(/
     c'       Treatments :               A     B'/
     c'                              ',i5,1x,i5)
1001  format(
     c' Duration health    Long-term',i5,'%',i5,'%'/,
     c' problem            Recent   ',i5,'%',i5,'%'/)
1002  format(
     c' Severity of        Mild     ',i5,'%',i5,'%'/
     c' Health Problem     Moderate ',i5,'%',i5,'%'/,
     c'                    Severe   ',i5,'%',i5,'%'/)
1003  format(
     c' Age (yrs)          Under 15 ',i5,'%',i5,'%'/,
     c'                    25-34 yrs',i5,'%',i5,'%'/,
     c'                    35-64 yrs',i5,'%',i5,'%'/,
     c'                    65+ yrs  ',i5,'%',i5,'%'/)
1004  format(a15,
     c               '     Y        ',i5,'%',i5,'%'/,
     c'                    N        ',i5,'%',i5,'%')

	stop
	end
