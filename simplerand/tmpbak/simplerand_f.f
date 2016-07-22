      subroutine simplerand(iPercent, iNoPatients,
     x                      dur_lt_a,   dur_lt_b,
     x                      dur_rec_a,  dur_rec_b,
     x                      age_u35_a,  age_u35_b,
     x                      age_btw_a,  age_btw_b,
     x                      age_65p_a,  age_65p_b,
     x                      hlt_poor_a, hlt_poor_b,
     x                      hlt_ok_a,   hlt_ok_b,
     x                      hlt_good_a, hlt_good_b,
     x                      hlt_vgd_a,  hlt_vgd_b,
     x                      rand_a,     rand_b,
     x                      result_a,   result_b)
     
      implicit none
    
      integer  iPercent, iNoPatients
      integer  dur_lt_a,   dur_lt_b
      integer  dur_rec_a,  dur_rec_b
      integer  age_u35_a,  age_u35_b
      integer  age_btw_a,  age_btw_b
      integer  age_65p_a,  age_65p_b
      integer  hlt_poor_a, hlt_poor_b
      integer  hlt_ok_a,   hlt_ok_b
      integer  hlt_good_a, hlt_good_b
      integer  hlt_vgd_a,  hlt_vgd_b
      integer  rand_a,     rand_b
      integer  result_a,   result_b
    
      iPercent = iPercent + 1
      iNoPatients = 1234
    
      iPercent = 1
      iNoPatients = 2
      dur_lt_a = 3
      dur_lt_b = 4
      dur_rec_a = 5
      dur_rec_b = 6
      age_u35_a = 7
      age_u35_b = 8
      age_btw_a = 9
      age_btw_b = 10
      age_65p_a = 11
      age_65p_b = 12
      hlt_poor_a = 13
      hlt_poor_b = 14
      hlt_ok_a = 15
      hlt_ok_b = 16
      hlt_good_a = 17
      hlt_good_b = 18
      hlt_vgd_a = 19
      hlt_vgd_b = 20
      rand_a = 21
      rand_b = 22
      result_a = 23
      result_b = 24
    
      return
    
      end subroutine simplerand
