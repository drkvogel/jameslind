      subroutine test()
      
      character temp*256
      
      call GETENV('LD_LIBRARY_PATH',temp)
      print*,'LD_LIBRARY_PATH: ', temp

      end subroutine test
