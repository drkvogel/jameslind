      subroutine getseed(seed)
c
c     gets seed from system date & time routine, & passes back

c     Author : Jim Halsey, Nov 2004
c
      implicit none

      integer tim, seed
      character ctim*30, ct1819*2
     
c     integer date_time(8)
c     character clock(3)*12
c
c     call date_and_time(clock(1),clock(2),clock(3),date_time)
c     call seed(date_time(8))

      tim=Time8()
      call CTime(tim, ctim)
      ct1819=ctim(18:19)
      read(ct1819,'(i2)')seed
      
      return
      end
