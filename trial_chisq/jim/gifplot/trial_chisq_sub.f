      subroutine plot()
    
	integer dt, dc, nt, nc, total, iff, i, j
	integer dt_init, dc_init, nt_init, nc_init
	real o_e, var, or, odds, oddsred, z, se, ci1, ci2, p, p2
	real gammp, chisq2
	real*8 logodds, logci1, logci2, chisq, g01ecf, ndf, alogx
	character textstr*70/' '/, xstr*5/' '/
	character alog(-2:2)*5/'0.01','0.1 ','1.0 ','10  ','100 '/
      character temp*256/'temp'/

	integer PGBEG
	real xmin/-2.0/,xmax/5.0/,ymin/0.0/,ymax/10.0/
	real xarr(2)/2*0.0/,yarr(2)/2*0.0/

c	write(6,*)' Enter number of deaths in trt gp'
c	read(6,'(i4)')dt_init

c	write(6,*)' Enter number of deaths in control gp'
c	read(6,'(i4)')dc_init

	nt_init=10
	nc_init=10
	dt_init=4
	dc_init=6

c now the pgplot stuff....!!!!!!!!!!!
c      call PUTENV('LD_LIBRARY_PATH=/usr/local/pgplot')
      call getenv ('LD_LIBRARY_PATH', temp)
      print*,'LD_LIBRARY_PATH: ', temp

c open graphics device
c        IF (PGBEG(0,'?',1,1) .NE. 1) STOP

	  IF (PGBEG(0,'/user/cp/public_html/cjb/p.gif/GIF',1,1) .NE. 1) STOP

c set character font : 2=roman
	call pgscf(2)

c set coordinate viewport
	call pgsvp(0.0,1.0,0.0,1.0)
c set window
	call pgswin(0.0,1.0,0.0,1.0)

c set colour index : 1=white; 2=black
	call pgscr(0,0.0,0.0,0.0) ! black
	call pgscr(1,1.0,1.0,1.0) ! white
	call pgscr(20,0.5,0.0,0.0) ! dark red	
	call pgscr(17,1.0,0.9,0.1) ! yellowy

c set background colour
	call pgsci(1) ! white
	call pgrect(0.0,1.0,0.0,1.0)

c set window & adjust viewport to same aspect ratio (whatever that means!)
	call pgwnad(xmin,xmax,ymin,ymax)
c set fill area : 1=solid
	call pgsfs(1)
c set line width
	call pgslw(2)

c choose black!
	call pgsci(0)

c calculate the stats...
	do i=1,5
	  nt=nt_init*i
	  nc=nc_init*i
	  dt=dt_init*i
	  dc=dc_init*i
	  total=nt+nc
	  o_e=real(dt) - real(nt*(dt+dc))/real(total)
	  var=real((nt*nc)*(dt+dc)*(total-dt-dc))
	  var=var/real((total)*(total)*(total-1))
	  z=o_e/sqrt(var)
	  chisq=z*z
	  ndf=1

cc	p=0.666
cc	  p=sngl(g01ecf('U',chisq,ndf,iff))

	  chisq2=chisq/2.0
	  p=1.0-gammp(0.5,chisq2)

c	  or=real(dt*(nt-dt))/real(dc*(nc-dc))
	  
	  odds=exp(o_e/var)
	  logodds=log10(odds)
	  oddsred=(nint(100.0-100.0*odds))

	  ci1=exp(z/sqrt(var) - 1.96/sqrt(var))
	  ci2=exp(z/sqrt(var) + 1.96/sqrt(var))
	  logci1=log10(ci1)
	  logci2=log10(ci2)

c	print*,' '
c	print*,' logodds=',logodds,' logci1=',logci1,' logci2=',logci2
c	print*,' p=',p,' p2=',p2

c draw a line between the confidence intervals
	  xarr(1)=logci1
	  yarr(1)=real(i)
	  xarr(2)=logci2
	  yarr(2)=yarr(1)

	  call pgslw(1)
	  call pgline(2,xarr,yarr)

c draw a single point OR

	  call pgslw(10)
 	  call pgpt1(log10(odds),yarr(1),-1)

c write some text
	  textstr(1:27) ='T:    /     , C:    /     ,'
	  textstr(28:70)=' p=     , OR=    , 95% CI = {     :     }'

	  write(textstr(3:6),'(i4)')dt
	  write(textstr(8:12),'(i5)')nt
	  write(textstr(17:20),'(i4)')dc
	  write(textstr(22:26),'(i5)')nc

	  write(textstr(31:35),'(f5.4)')p
	  write(textstr(41:44),'(f4.2)')odds
	  write(textstr(57:61),'(f4.2)')ci1
	  write(textstr(64:67),'(f4.2)')ci2	  

	  call pgslw(1)
	  call pgsch(0.7)
	  call pgptxt(0.45,real(i),0.0,0.0,textstr)

	end do

c	draw some axes
c - y axis	
	xarr(1)=0.0
	xarr(2)=xarr(1)
	yarr(1)=0.5
	yarr(2)=10.0
	call pgline(2,xarr,yarr)
c - x axis
	xarr(1)=-2.5
	xarr(2)=2.5
	yarr(1)=0.5
	yarr(2)=0.5
	call pgline(2,xarr,yarr)

	do j=-2, 2
	  yarr(1)=0.5
	  yarr(2)=0.4
	  xarr(1)=real(j)
	  xarr(2)=xarr(1)
c	  alogx=alog10(real(j))
c        write(xstr,'(f5.4)')alogx
	  call pgline(2,xarr,yarr)
	  call pgsch(1.0)
	  call pgptxt(xarr(1)-0.2,0.2,0.0,0.0,alog(j))
	end do

	call pgend

	stop
    
        end subroutine plot
