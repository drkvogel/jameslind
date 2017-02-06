#include "xcgi.c"

#define USE_CGI 1

#define NONE       -1
#define FORM_REC   0
#define FORM_UNREC 1

//extern int errno, sys_nerr;

int iFormSubmitted;
int iPercent, iNrandRec, iNrandUnrec;
char szCategory[100]        = "\0";

extern void simplerand_(
    const int* bUnrec, const int* nrand, const int* propx, const int* nranda, const int* nrandb,
    const int* fact1_1a, const int* fact1_1b, const int* fact1_2a, const int* fact1_2b, const int* fact2_1a, const int* fact2_1b,
    const int* fact2_2a, const int* fact2_2b, const int* fact2_3a, const int* fact2_3b, const int* fact3_1a, const int* fact3_1b,
    const int* fact3_2a, const int* fact3_2b, const int* fact3_3a, const int* fact3_3b, const int* fact3_4a, const int* fact3_4b,
    const int* factx_1a, const int* factx_1b, const int* factx_2a, const int* factx_2b);

/*                        const int* iPercent,   const int* iNoPatients,
                        const int* dur_lt_a,   const int* dur_lt_b,
                        const int* dur_rec_a,  const int* dur_rec_b,
                        const int* age_u35_a,  const int* age_u35_b,
                        const int* age_btw_a,  const int* age_btw_b,
                        const int* age_65p_a,  const int* age_65p_b,
                        const int* hlt_poor_a, const int* hlt_poor_b,
                        const int* hlt_ok_a,   const int* hlt_ok_b,
                        const int* hlt_good_a, const int* hlt_good_b,
                        const int* hlt_vgd_a,  const int* hlt_vgd_b,
                        const int* rand_a,     const int* rand_b,
                        const int* result_a,   const int* result_b);
*/

char* CGI_TARGET = "simplerand.cgi";

void printresults() {

    int nrand = 0, propx = 0, nranda = 0, nrandb = 0,
    fact1_1a = 0, fact1_1b = 0, fact1_2a = 0, fact1_2b = 0, fact2_1a = 0, fact2_1b = 0,
    fact2_2a = 0, fact2_2b = 0, fact2_3a = 0, fact2_3b = 0, fact3_1a = 0, fact3_1b = 0,
    fact3_2a = 0, fact3_2b = 0, fact3_3a = 0, fact3_3b = 0, fact3_4a = 0, fact3_4b = 0,
    factx_1a = 0, factx_1b = 0, factx_2a = 0, factx_2b = 0;
    
    if (FORM_REC == iFormSubmitted)
        nrand = iNrandRec;
    else
        nrand = iNrandUnrec;

    simplerand_(
                &iFormSubmitted, &nrand, &iPercent, &nranda, &nrandb,
                &fact1_1a, &fact1_1b, &fact1_2a, &fact1_2b, &fact2_1a, &fact2_1b,
                &fact2_2a, &fact2_2b, &fact2_3a, &fact2_3b, &fact3_1a, &fact3_1b,
                &fact3_2a, &fact3_2b, &fact3_3a, &fact3_3b, &fact3_4a, &fact3_4b,
                &factx_1a, &factx_1b, &factx_2a, &factx_2b);

    printf(
"<html>"
"<head>"
"<title>Randomisation results</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"
"<style type=\"text/css\">"
"body { font-family: verdana, sans-serif }"
"</style>"
"</head>"
""
"<body bgcolor=\"#FFFFFF\">"
"<p><b>%d patients were randomised, %d to treatment A, %d treatment B:</b></p>"
"<table width=\"75%%\" border=\"0\">"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>Treatment A</td>"
"    <td>Treatment B</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>(total number of patients = %d)</td>"
"    <td>(total number of patients = %d)</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>%%</td>"
"    <td>%%</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td colspan=\"2\"><b>Duration of health problem</b></td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Long term</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>More recent</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td colspan=\"2\"><b>Severity of health problem</b></td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Mild</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td> "
"      <p>Moderate</p>"
"    </td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Severe</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td colspan=\"2\"><b>Age, in years</b></td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Under 15</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>15-34</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>35-64</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>65 &amp; older</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>",
    nrand, nranda, nrandb,
    nranda, nrandb,
    fact1_1a, fact1_1b,
    fact1_2a, fact1_2b,


    fact2_1a, fact2_1b,
    fact2_2a, fact2_2b,
    fact2_3a, fact2_3b,

    fact3_1a, fact3_1b,
    fact3_2a, fact3_2b,
    fact3_3a, fact3_3b,
    fact3_4a, fact3_4b);

    printf(
"  <tr> "
"    <td colspan=\"2\"><b>Condition: %s</b></td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>Yes</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>No</td>"
"    <td>&nbsp;%d</td>"
"    <td>&nbsp;%d</td>"
"  </tr>",
            (FORM_UNREC == iFormSubmitted) ? szCategory : "Very anxious?",
            factx_1a, factx_1b,
            factx_2a, factx_2b);
    
    printf("</table>"
"</body>"
"</html>");
            
} // printresults()

int main(int argc, char** argv, char* envp[])
{
    const char _szErrorNrandRec[]   = "<font color=\"red\">&gt;&gt;&gt; Number of patients must be between 0 and 10000</font>";

    const char _szErrorCategory[]   = "<tr><td>&nbsp;</td><td><font color=\"red\">&gt;&gt;&gt; Please enter a category to be randomised</font></td></tr>";
    const char _szErrorPercent[]    = "<tr><td>&nbsp;</td><td><font color=\"red\">&gt;&gt;&gt; Percentage must be between 0 and 100</font></td></tr>";
    const char _szErrorNrandUnrec[] = "<tr><td>&nbsp;</td><td><font color=\"red\">&gt;&gt;&gt; Number of patients must be between 0 and 10000</font></td></tr>";

    char szPercent[4]           = "\0";
    char szNrandRec[6]          = "\0";
    char szNrandUnrec[6]        = "\0";

    char szErrorCategory[100]   = "\0";
    char szErrorPercent[100]    = "\0";
    char szErrorNrandRec[100]   = "\0";
    char szErrorNrandUnrec[100] = "\0";

    int bErr = 0;

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }

    fprintf(stderr, "test\n");
    
    if (0 != strlen(xcgi_param_value_named("rec")))
        iFormSubmitted = FORM_REC;
    else if (0 != strlen(xcgi_param_value_named("unrec"))) 
        iFormSubmitted = FORM_UNREC;
    else
        iFormSubmitted = NONE;

    if (NONE != iFormSubmitted) {
    
        strcpy(szPercent,    xcgi_param_value_named("perc"));
        strcpy(szNrandRec,   xcgi_param_value_named("nrandrec"));
        strcpy(szNrandUnrec, xcgi_param_value_named("nrandunrec"));
        strcpy(szCategory,   xcgi_param_value_named("cat"));

        iPercent    = atoi(szPercent);
        iNrandRec   = atoi(szNrandRec);
        iNrandUnrec = atoi(szNrandUnrec);

        if (FORM_REC == iFormSubmitted) {
        
            if (0 == strlen(szNrandRec)
                || iNrandRec < 0 || iNrandRec > 100000) {
                strcpy(szErrorNrandRec, _szErrorNrandRec);
                bErr = 1;
            }
        }
        
        if (FORM_UNREC == iFormSubmitted) {
        
            if (0 == strlen(szNrandUnrec)
                || iNrandUnrec < 0 || iNrandUnrec > 100000) {
                strcpy(szErrorNrandUnrec, _szErrorNrandUnrec);
                bErr = 1;
            }

            if (0 == strlen(szPercent)
                || iPercent < 0 || iPercent > 100) {
                strcpy(szErrorPercent, _szErrorPercent);
                bErr = 1;
            }

            if (0 == strlen(szCategory)) {
                strcpy(szErrorCategory, _szErrorCategory);
                bErr = 1;
            }        
        }
        
    }
    
    if (0 == bErr  && NONE != iFormSubmitted) {

        printresults();

    } else {
    
        // introduction

        printf(
"<html>"
"<head>"
"<title>Simple Randomisation</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"
""
"<style type=\"text/css\">"
"body   { font-family: verdana, sans-serif; font-size: 80% }"
"p,span { font-size: 80%% }"
"</style>"
""
"</head>"
""
"<body bgcolor=\"#FFFFFF\">"
"<table width=\"80%%\" align=\"center\"><tr><td>"
"<h3 align=\"center\">Allocating sufficiently large numbers of patients at random<br> "
"  to create similar treatment comparison groups</h3>"
"<p>Random allocation of patients to treatment comparison groups is one of the "
"  ways to ensure that tests of treatments are fair. It helps to create comparison "
"  groups made up of patients with comparable characteristics. Unless bias is reduced "
"  in this way, any difference in the progress of patients in the comparison groups "
"  may simply reflect the fact that they were composed of patients who were different "
"  to begin with, and that the differences were nothing to do with relative effects "
"  of the different treatments.</p>"
"<h4>Achieving balance in RECORDED patient characteristics of importance</h4>"
"<p>The larger the number of patients assigned at random to treatment comparison "
"  groups, the more likely it is that the comparison groups will be made up of "
"  patients with comparable characteristics, and the less likely that potentially "
"  important imbalances will remain. You can see this effect of increasing the "
"  numbers of patients using this randomisation program. Ask the program to randomise "
"  successively larger numbers of patients (up to 100,000) and watch the way that "
"  the characteristics of the patients in the treatment comparison groups become "
"  increasingly balanced.</p>");

    printf(
"%s\n"
"<form method=\"post\" action=\"\">"
"  <input type=\"text\" name=\"nrandrec\" value=\"%s\"size=\"6\" maxlength=\"6\">"
"  <span>Enter the number of patients to be randomised, and hit the Go button....</span>"
"  <input type=\"hidden\" name=\"rec\" value=\"rec\">"
"  <input type=\"submit\" name=\"Submit\" value=\"Go\">"
"</form>"
"<h4><br>"
"  Achieving balance in UNRECORDED patient characteristics of importance</h4>"
"<p>The program uses duration and severity of patients' health problems, and their "
"  ages, because these characteristics of patients are quite likely to influence "
"  the outcome of their illnesses, and possibly the effects of treatments as well. "
"  But what about characteristics that are important but haven't been recorded? "
"  For example, some patients may be very anxious about their illness, and this "
"  may have an impact on their progress and the effects of treatment.</p>"
"<p>Allocating sufficiently large numbers of patients at random to treatment comparison "
"  groups achieves balance in unrecorded as well as recorded characteristics of "
"  importance. To show this, replace 'very anxious' with an unrecorded characteristic "
"  of your own choice, indicate its likely frequency, and run the program by hitting "
"  Go.</p>",
    szErrorNrandRec,
    szNrandRec);

    printf(
"<form method=\"post\" action=\"\">"
"  <table border=\"0\" width=\"75%%\">"
"%s\n"
"    <tr> "
"      <td><span>Name the unrecorded characteristic here:</span></td>"
"      <td> "
"        <input type=\"text\" name=\"cat\" value=\"%s\">"
"      </td>"
"    </tr>"
"%s\n"
"    <tr> "
"      <td><span>How many people out of 100 have this characteristic?</span></td>"
"      <td>"
"        <input type=\"text\" name=\"perc\" value=\"%s\" size=\"3\" maxlength=\"3\">"
"      </td>"
"    </tr>"
"%s\n"
"    <tr>"
"      <td><span>Enter the number of patients to be randomised...</span></td>"
"      <td><input type=\"text\" name=\"nrandunrec\" value=\"%s\"size=\"6\" maxlength=\"6\"></td>"
"    </tr>"
"    <tr>"
"      <td><span>And hit Go:</span></td>"
"      <td>"
"        <input type=\"hidden\" name=\"unrec\" value=\"unrec\">"
"        <input type=\"submit\" name=\"Submit2\" value=\"Go\">"
"      </td>"
"    </tr>"
"  </table>"
"</form>"
"<p>&nbsp;</p>"
"<p>&nbsp; </p>"
"</td></tr></table>"
"</body>"
"</html>",
        szErrorCategory,
        szCategory,
        szErrorPercent,
        szPercent,
        szErrorNrandUnrec,
        szNrandUnrec);

    }

    if (USE_CGI) xcgi_exit();

    return 0;
}
