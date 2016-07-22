      subroutine test()
      
      call getenv ('LD_LIBRARY_PATH', temp)
      print*,'LD_LIBRARY_PATH: ', temp

      end subroutine test
