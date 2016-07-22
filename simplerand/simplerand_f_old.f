      subroutine simplerand(form, nrand,propx,nranda,nrandb,
     *  fact1_1a,fact1_1b,fact1_2a,fact1_2b,fact2_1a,fact2_1b,
     *  fact2_2a,fact2_2b,fact2_3a,fact2_3b,fact3_1a,fact3_1b,
     *  fact3_2a,fact3_2b,fact3_3a,fact3_3b,fact3_4a,fact3_4b,  
     *  factx_1a,factx_1b,factx_2a,factx_2b)
c
c   Takes in nrand, clabel & propx
c   gets NRAND randon numbers between 0.0 & 1.0 for each of the 4 factor groups
c   FACT1, FACT2, FACT3 & FACTX, to determine the distributions between 2 theoretical 
c   treatment groups, treatment A & treatment B
c
c   Factor 1 : Duration of health problem : 2 categories - long-term; recent
c   Factor 2 : Severity of Health problem : 3 categories - mild; moderate; severe
c   Factor 3 : Age                   : 4 categories - under 15, 15-34, 35-64, 65+

c   Factor X : Chosen by user - if CLABEL blank then there is no factor - passes back zeros
c
c   For eg, FACT3(2,1) is the number of patients for factor 2 (age), category 2 (15-34),
c                        on treatment A
c
c   Web version : Passes back 22 values...
c
c   Author : Jim Halsey, March 2004
c
      implicit none
  
      integer i, nrand, form
      integer nranda, nrandb, trtdim, 
     *  fact1_1a,fact1_1b,fact1_2a,fact1_2b,fact2_1a,fact2_1b,
     *  fact2_2a,fact2_2b,fact2_3a,fact2_3b,fact3_1a,fact3_1b,
     *  fact3_2a,fact3_2b,fact3_3a,fact3_3b,fact3_4a,
     *  fact3_4b,factx_1a,factx_1b,factx_2a,factx_2b
      integer  fact1(2,2)/4*0/,fact2(3,2)/6*0/,fact3(4,2)/8*0/,
     *         factx(2,2)/4*0/
      real ntoss, randno, propx, propxpc
      integer date_time(8)
      character clock(3)*12
  
      call date_and_time(clock(1),clock(2),clock(3),date_time)
      call srand(date_time(8))
  
      do i=1,int(nrand)

        randno = rand(0)
        
        if(randno.lt.0.5)then
          trtdim=1
          nranda=nranda+1
        else
          trtdim=2
          nrandb=nrandb+1
        endif

c factor 1 (duration of health problem) 0.7=long-term; 0.3=recent

        randno = rand(0)

        if(randno.lt.0.3)then
          fact1(1,trtdim)=fact1(1,trtdim)+1
        else 
          fact1(2,trtdim)=fact1(2,trtdim)+1
        endif

c   factor 2 (severity of health problem) 0.3: mild; 0.45 moderate; 0.25: severe

        randno = rand(0)

        if(randno.lt.0.3)then
          fact2(1,trtdim)=fact3(1,trtdim)+1
        else if((randno.ge.0.3).and.(randno.lt.0.75))then
          fact2(2,trtdim)=fact3(2,trtdim)+1
        else 
          fact2(3,trtdim)=fact3(3,trtdim)+1
        endif

c factor 3 (age) 0.2= under 15; 0.25= 14-34 yrs; 0.25= 35-64 yrs; 0.3= 65 & older

        randno = rand(0)
        
        if(randno.lt.0.2)then
          fact3(1,trtdim)=fact3(1,trtdim)+1
        else if((randno.ge.0.2).and.(randno.lt.0.45))then
          fact3(2,trtdim)=fact3(2,trtdim)+1
        else if((randno.ge.0.45).and.(randno.lt.0.7))then
          fact3(3,trtdim)=fact3(3,trtdim)+1
        else
          fact3(4,trtdim)=fact3(4,trtdim)+1
        endif


c   factor X (a factor chosen by the user)

        propxpc=propx/100.0
  
        randno = rand(0)
        
        if(randno.lt.propxpc)then
          factx(1,trtdim)=factx(1,trtdim)+1
        else 
          factx(2,trtdim)=factx(2,trtdim)+1
        endif
        
      end do

c convert to %s & assign array values to variables to be passed back top main program

      fact1_1a=nint(100.0*fact1(1,1)/real(nranda))
      fact1_1b=nint(100.0*fact1(1,2)/real(nrandb))
      fact1_2a=nint(100.0*fact1(2,1)/real(nranda))
      fact1_2b=nint(100.0*fact1(2,2)/real(nrandb))
  
      fact2_1a=nint(100.0*fact2(1,1)/real(nranda))
      fact2_1b=nint(100.0*fact2(1,2)/real(nrandb))
      fact2_2a=nint(100.0*fact2(2,1)/real(nranda))
      fact2_2b=nint(100.0*fact2(2,2)/real(nrandb))
      fact2_3a=nint(100.0*fact2(3,1)/real(nranda))
      fact2_3b=nint(100.0*fact2(3,2)/real(nrandb))
  
      fact3_1a=nint(100.0*fact3(1,1)/real(nranda))
      fact3_1b=nint(100.0*fact3(1,2)/real(nrandb))
      fact3_2a=nint(100.0*fact3(2,1)/real(nranda))
      fact3_2b=nint(100.0*fact3(2,2)/real(nrandb))
      fact3_3a=nint(100.0*fact3(3,1)/real(nranda))
      fact3_3b=nint(100.0*fact3(3,2)/real(nrandb))
      fact3_4a=nint(100.0*fact3(4,1)/real(nranda))
      fact3_4b=nint(100.0*fact3(4,2)/real(nrandb))
  
      factx_1a=nint(100.0*factx(1,1)/real(nranda))
      factx_1b=nint(100.0*factx(1,2)/real(nrandb))
      factx_2a=nint(100.0*factx(2,1)/real(nranda))
      factx_2b=nint(100.0*factx(2,2)/real(nrandb))
      
      nranda = 1
  
      return
      end
