extern void getseed_(const int* seed);

extern void simplerand2_(
    const int* seed,
    const int* bUnrec, const int* nrand, const int* propx, const int* nranda, const int* nrandb,
    const int* fact1_1a, const int* fact1_1b, const int* fact1_2a, const int* fact1_2b, const int* fact2_1a, const int* fact2_1b,
    const int* fact2_2a, const int* fact2_2b, const int* fact2_3a, const int* fact2_3b, const int* fact3_1a, const int* fact3_1b,
    const int* fact3_2a, const int* fact3_2b, const int* fact3_3a, const int* fact3_3b, const int* fact3_4a, const int* fact3_4b,
    const int* factx_1a, const int* factx_1b, const int* factx_2a, const int* factx_2b,
    const int* fact1_1an, const int* fact1_1bn, const int* fact1_2an, const int* fact1_2bn, const int* fact2_1an, const int* fact2_1bn,
    const int* fact2_2an, const int* fact2_2bn, const int* fact2_3an, const int* fact2_3bn, const int* fact3_1an, const int* fact3_1bn,
    const int* fact3_2an, const int* fact3_2bn, const int* fact3_3an, const int* fact3_3bn, const int* fact3_4an, const int* fact3_4bn,
    const int* factx_1an, const int* factx_1bn, const int* factx_2an, const int* factx_2bn);

int nrand = 0, nrand2 = 0, propx = 0, nranda = 0, nrandb = 0, nranda2 = 0, nrandb2 = 0,
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

void getresults1() {
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
}

void getresults2() {
    fact1_1a2 = 0; fact1_1b2 = 0; fact1_2a2 = 0; fact1_2b2 = 0; fact2_1a2 = 0; fact2_1b2 = 0;
    fact2_2a2 = 0; fact2_2b2 = 0; fact2_3a2 = 0; fact2_3b2 = 0; fact3_1a2 = 0; fact3_1b2 = 0;
    fact3_2a2 = 0; fact3_2b2 = 0; fact3_3a2 = 0; fact3_3b2 = 0; fact3_4a2 = 0; fact3_4b2 = 0;
    factx_1a2 = 0; factx_1b2 = 0; factx_2a2 = 0; factx_2b2 = 0;
    fact1_1an2 = 0; fact1_1bn2 = 0; fact1_2an2 = 0; fact1_2bn2 = 0; fact2_1an2 = 0; fact2_1bn2 = 0;
    fact2_2an2 = 0; fact2_2bn2 = 0; fact2_3an2 = 0; fact2_3bn2 = 0; fact3_1an2 = 0; fact3_1bn2 = 0;
    fact3_2an2 = 0; fact3_2bn2 = 0; fact3_3an2 = 0; fact3_3bn2 = 0; fact3_4an2 = 0; fact3_4bn2 = 0;
    factx_1an2 = 0; factx_1bn2 = 0; factx_2an2 = 0; factx_2bn2 = 0;
    //printf("<p>nranda2 %d, nrandb2%d</p>", nranda2, nrandb2);
    simplerand2_(
        &seed,
        &iFormSubmitted, &nrand2, &iPercent, &nranda2, &nrandb2,
        &fact1_1a2, &fact1_1b2, &fact1_2a2, &fact1_2b2, &fact2_1a2, &fact2_1b2,
        &fact2_2a2, &fact2_2b2, &fact2_3a2, &fact2_3b2, &fact3_1a2, &fact3_1b2,
        &fact3_2a2, &fact3_2b2, &fact3_3a2, &fact3_3b2, &fact3_4a2, &fact3_4b2,
        &factx_1a2, &factx_1b2, &factx_2a2, &factx_2b2,
        &fact1_1an2, &fact1_1bn2, &fact1_2an2, &fact1_2bn2, &fact2_1an2, &fact2_1bn2,
        &fact2_2an2, &fact2_2bn2, &fact2_3an2, &fact2_3bn2, &fact3_1an2, &fact3_1bn2,
        &fact3_2an2, &fact3_2bn2, &fact3_3an2, &fact3_3bn2, &fact3_4an2, &fact3_4bn2,
        &factx_1an2, &factx_1bn2, &factx_2an2, &factx_2bn2); 
}

void printresults() {
    char szSubtitle[1024];
    char *szLimitReached = "";
    if (FORM_REC == iFormSubmitted) {
        nrand = iNrandRec;
    } else {
        nrand = iNrandUnrec;
    }
    if (0 == iPower) {
        getseed_(&seed);
    }
    nrand2 = nrand * pow(10, iPower); //nrand = iNrandRec;
    if (nrand2 > 100000) {
        nrand2 = nrand * pow(10, --iPower);
        szLimitReached = "<p align=\"center\"><font color=\"#FF0000\">Cannot randomize more than 100,000 patients</font></p>";
    }
    xcgi_param_showall(); //printf("<!-- nrand: %d, nrand2: %d -->\n\n", nrand, nrand2);
    getresults1();
    getresults2();
    sprintf(szSubtitle,
        "<h4 align=\"center\"><font size=\"-1\" color=\"#0000FF\">%d patients were "
        " randomised,<br>\n%d to treatment A, %d to treatment B:</font></h4>",
        nrand2, nranda2, nrandb2);
    printf(
        "<html>\n"
        "<head>\n"
        "<title>Randomisation results</title>\n"
        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
        "\n"
        "<link rel=\"stylesheet\" href=\"cochrane.css\" type=\"text/css\">\n"
        "<script language=\"Javascript\">\n"
        "\n"
        "function Show(ename) {	\n"
        "	if(document.all) {\n"
        "		for(var i = 0; i < document.all.length; ++i) {\n"
        "			if(document.all[i].name == ename) {\n"
        "				document.all[i].style.visibility ='visible';\n"
        "			}\n"
        "		}\n"
        "	} else {\n"
        "		var objs = document.getElementsByName(ename);\n"
        "	   	for(var i = 0; i < objs.length; ++i) {  \n"
        "			objs[i].style.visibility ='visible';\n"
        "		}\n"
        "	}\n"
        "}\n"
        "\n"
        "function Show6() {\n"
        "	Show(\"cond\");\n"
        "}\n"
        "\n"
        "function Show5() {\n"
        "	Show(\"age\");\n"
        "	setTimeout(\"Show6()\", 1000);\n"
        "}\n"
        "\n"
        "function Show4() {\n"
        "	Show(\"sev\");\n"
        "	setTimeout(\"Show5()\", 1000);\n"
        "}\n"
        "\n"
        "function Show3() {\n"
        "	Show(\"duration\");\n"
        "	setTimeout(\"Show4()\", 1000);\n"
        "}\n"
        "\n"
        "function Show2() {\n"
        "	Show(\"heading\");\n"
        "	setTimeout(\"Show3()\", 1000);\n"
        "}\n"
        "\n"
        "function Show1() {\n"
        "	setTimeout(\"Show2()\", 1000);\n"
        "}\n"
        "\n"
        "</script>\n"
        "</head>\n"
        "\n"
        "<body bgcolor=\"#FFFFFF\" %s>\n"
        "<table border=1 cellspacing=0 cellpadding=10 width=750 bordercolor=\"#003366\" align=\"center\">\n"
        "<tr><td>\n"
        "  <table width=\"100%\">\n"
        "    <tr><td>\n"
        "      <table width=\"200\" border=\"0\" align=\"left\" cellpadding=\"3\">\n"
        "        <tr>\n"
        "          <td bgcolor=\"#EAEEF7\" width=\"100\" height=\"2\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/\" class=\"menubar\">Home</a></div></td>\n"
        "          <td bgcolor=\"#EAEEF7\" height=\"2\" width=\"100\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/search\" class=\"menubar\">Search</a></div></td>\n"
        "        </tr>\n"
        "        <tr bgcolor=\"#EAEEF7\">\n"
        "          <td colspan=\"2\" class=\"nav\"> <div align=\"center\"><b><font color=\"#003366\">jameslindlibrary.org</font></b></div></td>\n"
        "        </tr>\n"
        "      </table><br>\n"
        "    </td></tr>\n"
        "    <tr><td>\n"
        "            <h4 align=\"center\">%d patients were randomised,<br>\n"
        "              %d to treatment A, %d to treatment B:</h4>\n"
        "%s"
        "    </td></tr>\n"
        "  </table>\n"
        "      <table width=\"80%\" align=\"center\" border=\"0\">\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td align=\"right\">Treatment A</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"n_trt_a1\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">Treatment B</td>\n"
        "          <td name=\"n_trt_b1\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td align=\"right\">(total = %d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" id=\"hideable\" name=\"heading\" style=\"visibility:hidden\"><font color=\"#0000FF\">(total=%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">(total = %d)</td>\n"
        "          <td name=\"n_trt_b2\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"heading\" style=\"visibility:hidden\">(total=%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td align=\"right\">number (\%)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"n_trt_a3\"><font color=\"#0000FF\"></font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">number (\%)</td>\n"
        "          <td name=\"n_trt_b3\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td colspan=\"2\"><b>Duration of health problem</b></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Long term</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" id=\"hideable\" name=\"duration\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_dur_b1\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"duration\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>More recent</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"duration\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_dur_b2\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"duration\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td colspan=\"2\"><b>Severity of health problem</b></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Mild</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"sev\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_sev_b1\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"sev\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Moderate</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"sev\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_sev_b2\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"sev\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Severe</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"sev\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_sev_b3\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"sev\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td colspan=\"2\"><b>Age, in years</b></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Under 15</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"age\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_age_b1\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"age\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>15-34</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"age\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_age_b2\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"age\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>35-64</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"age\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td name=\"n_age_b3\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"age\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>65 &amp; older</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"age\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;%d (%d)</td>\n"
        "          <td name=\"n_age_b4\">&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"age\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td colspan=\"2\"><b>Condition: %s</b></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td><font color=\"#0000FF\"></font></td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td> \n"
        "            <div align=\"right\"><font color=\"#0000FF\"></font></div>\n"
        "          </td>\n"
        "          <td>&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>Yes</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"cond\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\" name=\"n_cnd_b1\">&nbsp;</td>\n"
        "          <td align=\"right\"> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"cond\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>No</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\" name=\"cond\" style=\"visibility:hidden\"><font color=\"#0000FF\">%d (%d)</font></td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">%d (%d)</td>\n"
        "          <td align=\"right\" name=\"n_cnd_b2\">&nbsp;</td>\n"
        "          <td align=\"right\"> \n"
        "            <div align=\"right\"><font color=\"#0000FF\" name=\"cond\" style=\"visibility:hidden\">%d (%d)</font></div>\n"
        "          </td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td>&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "          <td align=\"right\"> \n"
        "            <div align=\"right\"></div>\n"
        "          </td>\n"
        "          <td align=\"right\">&nbsp;</td>\n"
        "        </tr>\n"
        "        <tr> \n"
        "          <td>&nbsp;</td>\n"
        "          <td colspan=\"9\" align=\"center\"> \n"
        "            <div align=\"center\">%s</div>\n"
        "            <form name=\"form1\" >\n"
        "              <div align=\"center\">\n"
        "              <input type=\"hidden\" name=\"seed\" value=\"%d\">\n"
        "              <input type=\"hidden\" name=\"ftype\" value=\"%d\">\n"
        "              <input type=\"hidden\" name=\"cat\" value=\"%s\">\n"
        "			  <input type=\"hidden\" name=\"power\" value=\"%d\">\n"
        "              <input type=\"hidden\" name=\"nrandrec\" value=\"%d\">\n"
        "              <input type=\"hidden\" name=\"nrandunrec\" value=\"%d\">\n"
        "			  <input type=\"hidden\" name=\"perc\" value=\"%d\">\n"
        "              <input type=\"submit\" name=\"Submit\" value=\"Make 10 times bigger\">\n"
        "              </div>\n"
        "            </form>\n"
        "          </td>\n"
        "        </tr>\n"
        "      </table>\n"
        "<p><p><p><a href=\"%s\"><<< Back to beginning</a></td></tr></table>\n"
        "</body>\n"
        "</html>\n",
        (0 == iPower) ? "" : "onLoad=\"Show1()\"",
        nrand,  nranda,  nrandb,
        (0 == iPower) ? "" : szSubtitle,
        nranda, nranda2, nrandb, nrandb2,

        fact1_1an, fact1_1a, fact1_1an2, fact1_1a2, fact1_1bn, fact1_1b, fact1_1bn2, fact1_1b2,
        fact1_2an, fact1_2a, fact1_2an2, fact1_2a2, fact1_2bn, fact1_2b, fact1_2bn2, fact1_2b2,

        fact2_1an, fact2_1a, fact2_1an2, fact2_1a2, fact2_1bn, fact2_1b, fact2_1bn2, fact2_1b2,
        fact2_2an, fact2_2a, fact2_2an2, fact2_2a2, fact2_2bn, fact2_2b, fact2_2bn2, fact2_2b2,
        fact2_3an, fact2_3a, fact2_3an2, fact2_3a2, fact2_3bn, fact2_3b, fact2_3bn2, fact2_3b2,

        fact3_1an, fact3_1a, fact3_1an2, fact3_1a2, fact3_1bn, fact3_1b, fact3_1bn2, fact3_1b2,
        fact3_2an, fact3_2a, fact3_2an2, fact3_2a2, fact3_2bn, fact3_2b, fact3_2bn2, fact3_2b2,
        fact3_3an, fact3_3a, fact3_3an2, fact3_3a2, fact3_3bn, fact3_3b, fact3_3bn2, fact3_3b2,
        fact3_4an, fact3_4a, fact3_4an2, fact3_4a2, fact3_4bn, fact3_4b, fact3_4bn2, fact3_4b2,

        (FORM_UNREC == iFormSubmitted) ? szCategory : "Very anxious?",
        factx_1an, factx_1a, factx_1an2, factx_1a2, factx_1bn, factx_1b, factx_1bn2, factx_1b2,
        factx_2an, factx_2a, factx_2an2, factx_2a2, factx_2bn, factx_2b, factx_2bn2, factx_2b2,

        szLimitReached,
        seed, iFormSubmitted, szCategory, iPower+1, nrand, nrand, iPercent,
        CGI_TARGET
    );

} //printresults()



// cruft
    //getseed_(&seed);

    //printf("10 times: %d", iFormSubmitted); return;
    //printf("Time: %s<BR>", __TIME__);
/*    printf("1: before blah blah blah:"
"                seed %d,"
"                iFormSubmitted %d, nrand %d, nrand2 %d, iPercent %d, nranda %d, nrandb %d,"
"                fact1_1a %d, fact1_2a %d<p>",
                seed,
                iFormSubmitted, nrand, nrand2, iPercent, nranda, nrandb,
                fact1_1a, fact1_2a);*/

/*    simplerand2_(
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
*/
/*    printf("2: after 1:"
"                seed %d,"
"                iFormSubmitted %d, nrand %d, nrand2 %d, iPercent %d, nranda %d, nrandb %d,"
"                fact1_1a %d, fact1_2a %d<p>",
                seed,
                iFormSubmitted, nrand, nrand2, iPercent, nranda, nrandb,
                fact1_1a, fact1_2a);
*/

/*
    simplerand2_(
                &seed,
                &iFormSubmitted, &nrand2, &iPercent, &nranda2, &nrandb2,
                &fact1_1a2, &fact1_1b2, &fact1_2a2, &fact1_2b2, &fact2_1a2, &fact2_1b2,
                &fact2_2a2, &fact2_2b2, &fact2_3a2, &fact2_3b2, &fact3_1a2, &fact3_1b2,
                &fact3_2a2, &fact3_2b2, &fact3_3a2, &fact3_3b2, &fact3_4a2, &fact3_4b2,
                &factx_1a2, &factx_1b2, &factx_2a2, &factx_2b2,
                &fact1_1an2, &fact1_1bn2, &fact1_2an2, &fact1_2bn2, &fact2_1an2, &fact2_1bn2,
                &fact2_2an2, &fact2_2bn2, &fact2_3an2, &fact2_3bn2, &fact3_1an2, &fact3_1bn2,
                &fact3_2an2, &fact3_2bn2, &fact3_3an2, &fact3_3bn2, &fact3_4an2, &fact3_4bn2,
                &factx_1an2, &factx_1bn2, &factx_2an2, &factx_2bn2);    
*/
  /*  printf("3: after 2:"
"                seed %d,"
"                iFormSubmitted %d, nrand %d, nrand2 %d, iPercent %d, nranda %d, nrandb %d,"
"                fact1_1a2 %d, fact1_2a2 %d;;; fact1_1a2 + fact1_2a2: %d<<p>",
                seed,
                iFormSubmitted, nrand, nrand2, iPercent, nranda, nrandb,
                fact
1_1a2, fact1_2a2, fact1_1a2 + fact1_2a2); */