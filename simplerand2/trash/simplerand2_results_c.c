
void printresults10times() {

    int nrand = 0, propx = 0, nranda = 0, nrandb = 0,
    fact1_1a = 0, fact1_1b = 0, fact1_2a = 0, fact1_2b = 0, fact2_1a = 0, fact2_1b = 0,
    fact2_2a = 0, fact2_2b = 0, fact2_3a = 0, fact2_3b = 0, fact3_1a = 0, fact3_1b = 0,
    fact3_2a = 0, fact3_2b = 0, fact3_3a = 0, fact3_3b = 0, fact3_4a = 0, fact3_4b = 0,
    factx_1a = 0, factx_1b = 0, factx_2a = 0, factx_2b = 0,
    fact1_1an = 0, fact1_1bn = 0, fact1_2an = 0, fact1_2bn = 0, fact2_1an = 0, fact2_1bn = 0,
    fact2_2an = 0, fact2_2bn = 0, fact2_3an = 0, fact2_3bn = 0, fact3_1an = 0, fact3_1bn = 0,
    fact3_2an = 0, fact3_2bn = 0, fact3_3an = 0, fact3_3bn = 0, fact3_4an = 0, fact3_4bn = 0,
    factx_1an = 0, factx_1bn = 0, factx_2an = 0, factx_2bn = 0,
    fact1_1a2 = 0, fact1_1b2 = 0, fact1_2a2 = 0, fact1_2b2 = 0, fact2_1a2 = 0, fact2_1b2 = 0,
    fact2_2a2 = 0, fact2_2b2 = 0, fact2_3a2 = 0, fact2_3b2 = 0, fact3_1a2 = 0, fact3_1b2 = 0,
    fact3_2a2 = 0, fact3_2b2 = 0, fact3_3a2 = 0, fact3_3b2 = 0, fact3_4a2 = 0, fact3_4b2 = 0,
    factx_1a2 = 0, factx_1b2 = 0, factx_2a2 = 0, factx_2b2 = 0,
    fact1_1an2 = 0, fact1_1bn2 = 0, fact1_2an2 = 0, fact1_2bn2 = 0, fact2_1an2 = 0, fact2_1bn2 = 0,
    fact2_2an2 = 0, fact2_2bn2 = 0, fact2_3an2 = 0, fact2_3bn2 = 0, fact3_1an2 = 0, fact3_1bn2 = 0,
    fact3_2an2 = 0, fact3_2bn2 = 0, fact3_3an2 = 0, fact3_3bn2 = 0, fact3_4an2 = 0, fact3_4bn2 = 0,
    factx_1an2 = 0, factx_1bn2 = 0, factx_2an2 = 0, factx_2bn2 = 0;

    if (FORM_REC10 == iFormSubmitted)
        nrand = iNrandRec;
    else
        nrand = iNrandUnrec;
        
    //printf("10 times: %d", iFormSubmitted); return;

    //getseed_(&seed);
    simplerand2_(
                &seed,
                &iFormSubmitted, &nrand, &iPercent, &nranda, &nrandb,
                &fact1_1a, &fact1_1b, &fact1_2a, &fact1_2b, &fact2_1a, &fact2_1b,
                &fact2_2a, &fact2_2b, &fact2_3a, &fact2_3b, &fact3_1a, &fact3_1b,
                &fact3_2a, &fact3_2b, &fact3_3a, &fact3_3b, &fact3_4a, &fact3_4b,
                &factx_1a, &factx_1b, &factx_2a, &factx_2b,
                &fact1_1an, &fact1_1bn, &fact1_2an, &fact1_2bn, &fact2_1an, &fact2_1bn,
                &fact2_2an, &fact2_2bn, &fact2_3an, &fact2_3bn, &fact3_1an, &fact3_1bn,
                &fact3_2an, &fact3_2bn, &fact3_3an, &fact3_3bn, &fact3_4an, &fact3_4bn,
                &factx_1an, &factx_1bn, &factx_2an, &factx_2bn);

    simplerand2_(
                &seed,
                &iFormSubmitted, &nrand * 10, &iPercent, &nranda, &nrandb,
                &fact1_1a2, &fact1_1b2, &fact1_2a2, &fact1_2b2, &fact2_1a2, &fact2_1b2,
                &fact2_2a2, &fact2_2b2, &fact2_3a2, &fact2_3b2, &fact3_1a2, &fact3_1b2,
                &fact3_2a2, &fact3_2b2, &fact3_3a2, &fact3_3b2, &fact3_4a2, &fact3_4b2,
                &factx_1a2, &factx_1b2, &factx_2a2, &factx_2b2,
                &fact1_1an2, &fact1_1bn2, &fact1_2an2, &fact1_2bn2, &fact2_1an2, &fact2_1bn2,
                &fact2_2an2, &fact2_2bn2, &fact2_3an2, &fact2_3bn2, &fact3_1an2, &fact3_1bn2,
                &fact3_2an2, &fact3_2bn2, &fact3_3an2, &fact3_3bn2, &fact3_4an2, &fact3_4bn2,
                &factx_1an2, &factx_1bn2, &factx_2an2, &factx_2bn2);    

    printf(
"<html>\n"
"<head>\n"
"<title>Randomisation results</title>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
"\n\n"
/*"<style type=\"text/css\">\n\n"
"body   { font-family: verdana, sans-serif; font-size: 80%% }\n\n"
"p,span, td { font-size: 80%% }\n\n"
"</style>\n\n" */
"<link rel=\"stylesheet\" href=\"cochrane.css\" type=\"text/css\">\n"
"\n\n"
"</head>\n"
"\n"
"<body bgcolor=\"#FFFFFF\">\n"

//"<table width=\"80%%\" align=\"center\">\n"
//"<tr><td>"

"<table border=1 cellspacing=0 cellpadding=10 width=750 bordercolor=\"#003366\" align=\"center\">\n"
"<tr><td>\n"

/*"  <table border=\"0\">"
"    <tr>"
"      <td width=\"80\">&nbsp;</td>"
"      <td>"
"        <h4 align=\"center\">%d patients were randomised,<br>%d to treatment A, %d treatment B:</h4>"
"      </td>"
"      <td><img src=\"lind_portrait_sm.gif\" width=\"80\" height=\"102\"></td>"
"    </tr>"
"  </table>"*/


"  <table width=\"100%%\">\n"
"    <tr><td>\n"
"      <table width=\"200\" border=\"0\" align=\"left\" cellpadding=\"3\">\n"
"        <tr>\n"
"          <td bgcolor=\"#EAEEF7\" width=\"100\" height=\"2\"> <div align=\"center\"><font size=\"-1\"><a href=\"http://www.jameslindlibrary.org/index2.html\" class=\"menubar\">Home</a></font></div></td>\n"
"          <td bgcolor=\"#EAEEF7\" height=\"2\" width=\"100\"> <div align=\"center\"><font size=\"-1\"><a href=\"http://www.jameslindlibrary.org/search.php\" class=\"menubar\">Search</a></font></div></td>\n"
"        </tr>\n"
"        <tr bgcolor=\"#EAEEF7\">\n"
"          <td colspan=\"2\"> <div align=\"center\"><b><font color=\"#003366\">jameslindlibrary.org</font></b></div></td>\n"
"        </tr>\n"
"      </table><br>\n"
"    </td></tr>\n"
"    <tr><td>\n"
"        <h4 align=\"center\">%d patients were randomised,<br>%d to treatment A, %d treatment B:</h4>\n"
"    </td></tr>\n"
"  </table>\n"

"<table width=\"80%%\" align=\"center\" border=\"0\">\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td align=\"right\">Treatment A</td>\n"
"    <td align=\"right\">Treatment B</td>\n"
"    <td width=\"25%%\">&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td align=\"right\">(total = %d)</td>\n"
"    <td align=\"right\">(total = %d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td align=\"right\">number (%%)</td>\n"
"    <td align=\"right\">number (%%)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n",
    nrand, nranda, nrandb,
    nranda, nrandb);
    
    /*printf("<tr><td colspan=4>"
        "fact1_1an, fact1_1a, fact1_1bn, fact1_1b, fact1_2an, fact1_2a, fact1_2bn, fact1_2b:<br>"
        "%d, %d, %d, %d, %d, %d, %d, %d.</td></tr> \n", 
        fact1_1an, fact1_1a, fact1_1bn, fact1_1b, fact1_2an, fact1_2a, fact1_2bn, fact1_2b);
*/
    printf(
"    <td colspan=\"2\"><b>Duration of health problem</b></td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Long term</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>More recent</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n",
    fact1_1an, fact1_1a, fact1_1bn, fact1_1b,
    fact1_2an, fact1_2a, fact1_2bn, fact1_2b);
    
    printf(
"  <tr> \n"
"    <td colspan=\"2\"><b>Severity of health problem</b></td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Mild</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Moderate</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Severe</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n",
    fact2_1an, fact2_1a, fact2_1bn, fact2_1b,
    fact2_2an, fact2_2a, fact2_2bn, fact2_2b,
    fact2_3an, fact2_3a, fact2_3bn, fact2_3b);
    
    printf(
"  <tr> \n"
"    <td colspan=\"2\"><b>Age, in years</b></td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Under 15</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>15-34</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>35-64</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>65 &amp; older</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td align=\"right\">&nbsp;%d (%d)</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n",

    fact3_1an, fact3_1a, fact3_1bn, fact3_1b,
    fact3_2an, fact3_2a, fact3_2bn, fact3_2b,
    fact3_3an, fact3_3a, fact3_3bn, fact3_3b,
    fact3_4an, fact3_4a, fact3_4bn, fact3_4b);

    printf(
"  <tr> "
"    <td colspan=\"2\"><b>Condition: %s</b></td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Yes</td>"
"    <td align=\"right\">&nbsp;%d (%d)</td>"
"    <td align=\"right\">&nbsp;%d (%d)</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>No</td>"
"    <td align=\"right\">&nbsp;%d (%d)</td>"
"    <td align=\"right\">&nbsp;%d (%d)</td>"
"  </tr>",
            (FORM_UNREC == iFormSubmitted) ? szCategory : "Very anxious?",
            factx_1an, factx_1a, factx_1bn, factx_1b,
            factx_2an, factx_2a, factx_2bn, factx_2b);
            
    printf(
"</table>\n"
"<p><p><p><a href=\"javascript:history.go(-1)\"  onMouseOver=\"self.status=document.referrer;return true\"><<< Go Back</a>"
"</td></tr></table>\n"
"</body>\n"
"</html>\n");

} // printresults2()


printf("
<html>
<head>
<title>Randomisation results</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">


<link rel="stylesheet" href="cochrane.css" type="text/css">


</head>

<body bgcolor="#FFFFFF">
<table border=1 cellspacing=0 cellpadding=10 width=750 bordercolor="#003366" align="center">
<tr><td>
  <table width="100%">
    <tr><td>
      <table width="200" border="0" align="left" cellpadding="3">
        <tr>
          <td bgcolor="#EAEEF7" width="100" height="2"> <div align="center"><font size="-1"><a href="http://www.jameslindlibrary.org/index2.html" class="menubar">Home</a></font></div></td>
          <td bgcolor="#EAEEF7" height="2" width="100"> <div align="center"><font size="-1"><a href="http://www.jameslindlibrary.org/search.php" class="menubar">Search</a></font></div></td>
        </tr>
        <tr bgcolor="#EAEEF7">
          <td colspan="2"> <div align="center"><b><font color="#003366">jameslindlibrary.org</font></b></div></td>
        </tr>
      </table><br>
    </td></tr>
    <tr><td>
            <h4 align="center">%d patients were randomised,<br>
              %d to treatment A, %d to treatment B:</h4>
    </td></tr>
  </table>
      <table width="80%" align="center" border="0">
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td align="right" width="26%">Treatment A</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="trt_a1">%s</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">Treatment B</td>
          <td name="trt_b1">&nbsp;</td>
          <td>%s</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td align="right" width="26%">(total = %d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="trt_a2">%s</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">(total = %d)</td>
          <td name="trt_b2">&nbsp;</td>
          <td>%s</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td align="right" width="26%">number (%)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="trt_a3">%s</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">number (%)</td>
          <td name="trt_b3">&nbsp;</td>
          <td>%s</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="2"><b>Duration of health problem</b></td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Long term</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="dur_a1">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="dur_b1">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">More recent</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="dur_a2">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="dur_b2">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="2"><b>Severity of health problem</b></td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Mild</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="sev_a1">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="sev_b1">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Moderate</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="sev_a2">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="sev_b2">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Severe</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="sev_a3">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="sev_b3">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="2"><b>Age, in years</b></td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Under 15</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="age_a1">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="age_b1">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">15-34</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="age_a2">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td name="age_b2">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">35-64</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="age_a3">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">&nbsp;%d (%d)</td>
          <td name="age_b3">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">65 &amp; older</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="age_a4">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">&nbsp;%d (%d)</td>
          <td name="age_b4">&nbsp;</td>
          <td>%d (%d)</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%" height="20">&nbsp;</td>
          <td width="25%" height="20">&nbsp;</td>
          <td width="26%" height="20">&nbsp;</td>
          <td height="20">&nbsp;</td>
          <td height="20">&nbsp;</td>
          <td height="20">&nbsp;</td>
          <td width="23%" height="20">&nbsp;</td>
          <td height="20">&nbsp;</td>
          <td height="20">&nbsp;</td>
          <td width="20%" height="20">&nbsp;</td>
        </tr>
        <tr> 
          <td colspan="2"><b>Condition: Very anxious?</b></td>
          <td width="26%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="23%">&nbsp;</td>
          <td>&nbsp;</td>
          <td>&nbsp;</td>
          <td width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">Yes</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="cnd_a1">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">%d (%d)</td>
          <td align="right" name="cnd_b1">&nbsp;</td>
          <td align="right">%d (%d)</td>
          <td align="right" width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">No</td>
          <td align="right" width="26%">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" name="cnd_a2">%d (%d)</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">&nbsp;%d (%d)</td>
          <td align="right" name="cnd_b2">&nbsp;</td>
          <td align="right">%d (%d)</td>
          <td align="right" width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td width="25%">&nbsp;</td>
          <td align="right" width="26%">&nbsp;</td>
          <td align="right">&nbsp;</td>
          <td align="right">&nbsp;</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="23%">&nbsp;</td>
          <td align="right">&nbsp;</td>
          <td align="right">&nbsp;</td>
          <td align="right" width="20%">&nbsp;</td>
        </tr>
        <tr> 
          <td width="1%">&nbsp;</td>
          <td colspan="9"> 
            <form name="form1" >
              <input type="submit" name="Submit" value="Make 10 times bigger">
            </form>
          </td>
        </tr>
      </table>
<p><p><p><a href="javascript:history.go(-1)"  onMouseOver="self.status=document.referrer;return true"><<< Go Back</a></td></tr></table>
</body>
</html>
