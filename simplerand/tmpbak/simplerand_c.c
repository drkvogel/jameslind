#include "xcgi.c"
#include "simplerand.h"

#define USE_CGI 1

//extern int errno, sys_nerr;
extern void simplerand_(const int* iPercent,   const int* iNoPatients,
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

char* CGI_TARGET = "simplerand.cgi";

int main(int argc, char** argv, char* envp[])
{
    const char _szErrorPercent[]    = "<p><font color=\"red\">&gt;&gt;&gt; Percentage must be between 0 and 100</font></p>";
    const char _szErrorNoPatients[] = "<p><font color=\"red\">&gt;&gt;&gt; Number of patients must be between 0 and 10000</font></p>";
    const char _szErrorCategory[]   = "<p><font color=\"red\">&gt;&gt;&gt; Please enter a category to be randomised</font></p>";
    int dur_lt_a,   dur_lt_b;
    int dur_rec_a,  dur_rec_b;
    int age_u35_a,  age_u35_b;
    int age_btw_a,  age_btw_b;
    int age_65p_a,  age_65p_b;
    int hlt_poor_a, hlt_poor_b;
    int hlt_ok_a,   hlt_ok_b;
    int hlt_good_a, hlt_good_b;
    int hlt_vgd_a,  hlt_vgd_b;
    
    int rand_a,     rand_b;
    int result_a,   result_b;

    char szCategory[100]        = "\0";
    char szPercent[4]           = "\0";
    char szNoPatients[6]        = "\0";

    char szErrorCategory[100] = "\0";
    char szErrorPercent[100]    = "\0";
    char szErrorNoPatients[100] = "\0";
    
    int iPercent, iNoPatients;
    int bErr = 0;

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
    
    fprintf(stderr, "test\n");
    
    if (0 != strlen(xcgi_param_value_named("Submit"))) {

        strcpy(szPercent,    xcgi_param_value_named("perc"));
        strcpy(szNoPatients, xcgi_param_value_named("nump"));
        strcpy(szCategory,   xcgi_param_value_named("cat"));
        
        iPercent    = atoi(szPercent);
        iNoPatients = atoi(szNoPatients);
        
        if (0 == strlen(szPercent)
            || iPercent < 0 || iPercent > 100) {
            strcpy(szErrorPercent, _szErrorPercent);
            bErr = 1;
        }
        
        if (0 == strlen(szNoPatients)
            || iNoPatients < 0 || iNoPatients > 10000) {
            strcpy(szErrorNoPatients, _szErrorNoPatients);
            bErr = 1;
        }
        
        if (0 == strlen(szCategory)) {
            strcpy(szErrorCategory, _szErrorCategory);
            bErr = 1;
        }
    }
    
    if (0 == bErr  && 0 != strlen(xcgi_param_value_named("Submit"))) {
        simplerand_(&iPercent, &iNoPatients,
                        &dur_lt_a,   &dur_lt_b,
                        &dur_rec_a,  &dur_rec_b,
                        &age_u35_a,  &age_u35_b,
                        &age_btw_a,  &age_btw_b,
                        &age_65p_a,  &age_65p_b,
                        &hlt_poor_a, &hlt_poor_b,
                        &hlt_ok_a,   &hlt_ok_b,
                        &hlt_good_a, &hlt_good_b,
                        &hlt_vgd_a,  &hlt_vgd_b,
                        &rand_a,     &rand_b,
                        &result_a,   &result_b);
                        
/*        printf(
"<html>\n"
"<head>\n"
"<title>Untitled Document</title>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
"</head>\n"
"\n"
"<body bgcolor=\"#FFFFFF\">\n"
"<h2 align=\"center\">Simple Randomisation into 2 Groups</h2>\n"
"<p>Percent: %d</p>"
"<p>No. patients: %d</p>"
"</body></html>",
            iPercent, iNoPatients); */
            
        printf(
"<html>"
"<head>"
"<title>Untitled Document</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"
"</head>"
""
"<body bgcolor=\"#FFFFFF\">"
"<p>%d patients (%d\%with the condition: %s) were randomised into:</p>"
"<p>%d Treatment A</p>"
"<p>%d Treatment B</p>"
"<table width=\"75\%\" border=\"0\">"
"  <tr>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>Treatment A</td>"
"    <td>Treatment B</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>Duration of problem</td>"
"    <td>: Long term</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>: More recent</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>Age</td>"
"    <td>: Under 35 yrs old</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>: Between 35 &amp; 65 years old</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>: 65 Years &amp; older</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>Current health status</td>"
"    <td>: Poor</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td> "
"      <p>: OK</p>"
"    </td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>: Good</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>: V. Good</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"  <tr> "
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"    <td>&nbsp;</td>"
"  </tr>"
"  <tr> "
"    <td>%s</td>"
"    <td>Yes</td>"
"    <td>%d</td>"
"    <td>%d</td>"
"  </tr>"
"</table>"
"</body>"
"</html>",
            iNoPatients, iPercent,
            szCategory,
            rand_a,     rand_b,
            dur_lt_a,   dur_lt_b,
            dur_rec_a,  dur_rec_b,
            age_u35_a,  age_u35_b,
            age_btw_a,  age_btw_b,
            age_65p_a,  age_65p_b,
            hlt_poor_a, hlt_poor_b,
            hlt_ok_a,   hlt_ok_b,
            hlt_good_a, hlt_good_b,
            hlt_vgd_a,  hlt_vgd_b,
            szCategory,
            result_a,   result_b
        );
            
    } else {
// introduction
        printf(
"<html>"
"<head>"
"<title>Simple Randomisation</title>"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">"
"</head>"
""
"<body bgcolor=\"#FFFFFF\">"
"<h3 align=\"center\">Allocating sufficiently large numbers of patients at random "
"  to create similar treatment comparison groups</h3>"
"<p>Random allocation of patients to treatment comparison groups is one of the "
"  ways to ensure that tests of treatments are fair. It helps too create comparison "
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
"  increasingly balanced.</p>"
"<form method=\"post\" action=\"\">"
"  <input type=\"text\" name=\"iPatients\" size=\"6\" maxlength=\"6\">"
"  Enter the number of patients to be randomised, and hit the Go button.... "
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
"  Go.</p>");
    printf(
"<form method=\"post\" action=\"\">"
"  <table border=\"0\" width=\"75%\">"
"    <tr> "
"      <td>Name the unrecorded characteristic here:</td>"
"      <td> "
"        <input type=\"text\" name=\"strChar\" value=\"Very anxious\">"
"      </td>"
"    </tr>"
"    <tr> "
"      <td>How many people out of 100 have this characteristic?</td>"
"      <td>"
"        <input type=\"text\" name=\"iFreq\" value=\"6\" size=\"3\" maxlength=\"3\">"
"      </td>"
"    </tr>"
"  </table>"
"  <p> "
"    <input type=\"text\" name=\"textfield\" size=\"6\" maxlength=\"6\">"
"    Now enter the number of patients to be randomised and hit the Go button... "
"    <input type=\"submit\" name=\"Submit2\" value=\"Go\">"
"  </p>"
"</form>"
"<p>&nbsp;</p>"
"<p>&nbsp; </p>"
"</body>"
"</html>"

// old introduction:

/*
        printf(    
"<html>\n"
"<head>\n"
"<title>Untitled Document</title>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
"</head>\n"
"\n"
"<body bgcolor=\"#FFFFFF\">\n"
"<h2 align=\"center\">Simple Randomisation into 2 Groups</h2>\n"
"<p align=\"left\">When you divide people randomly into two equal-sized groups, for \n"
"  example by tossing a coin to determine the group, the proportions of their characteristics \n"
"  are likely to be unbalanced due to the play of chance. Dividing people in this \n"
"  random way, a process called <i>simple randomisation</i>, can thus be a problem, \n"
"  since certain characteristics may relate to an outcome of interest, such as \n"
"  long term health, or recovery from illness, and therefore bias comparisons between \n"
"  the treatment groups.</p>\n"
"<p align=\"left\">The larger the group of patients randomised this way, the less \n"
"  likely these imbalances occur, since characteristics are more likely to even \n"
"  out in their respective proportions.</p>\n"
"<p align=\"left\">To demonstrate this, conside a group of patients with the following \n"
"  characteristics:</p>\n"
"<table width=\"75\%\" border=\"0\">\n"
"  <tr> \n"
"    <td>Duration of problem</td>\n"
"    <td>: Long term</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>: More recent</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>Age</td>\n"
"    <td>: Under 35 yrs old</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>: Between 35 &amp; 65 years old</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>: 65 Years &amp; older</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>&nbsp;</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>Current health status</td>\n"
"    <td>: Poor</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td> \n"
"      <p>: OK</p>\n"
"    </td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>Good</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>V. Good</td>\n"
"  </tr>\n"
"</table>\n"
"<p align=\"left\">A fourth category will be defined by the user. Choose a category, \n"
"  and the number of people out of 100 who have this characteristic (e.g. for Diabetic, \n"
"  5 out of 100).</p>\n"
        );
        */
/*        
        printf(
"<form method=\"post\" action=\"%s\">\n"
"%s\n"
"  <p>Category \n"
"    <input type=\"text\" name=\"cat\" value=\"%s\">\n"
"  </p>\n"
"%s\n"
"  <p>No. people out of 100 with this characteristic \n"
"    <input type=\"text\" name=\"perc\" value=\"%s\">\n"
"  </p>\n"
"  <p>Choose a number of patients to be randomised, up to 100,000, and they will \n"
"    be divided by simple randomisation, and a breakdown of the proportions of \n"
"    the above characteristics will be displayed.</p>\n"
"  <p> \n"
"%s\n"
"    <input type=\"text\" name=\"nump\"  value=\"%s\">\n"
"    Enter the number of patients to be randomised, and hit the 'Go' button...</p>\n"
"  <p>\n"
"    <input type=\"submit\" name=\"Submit\" value=\"Go\">\n"
"  </p>\n"
"</form>\n"
"</body>\n"
"</html>",
            CGI_TARGET,
            szErrorCategory,
            xcgi_param_value_named("cat"),
            szErrorPercent,
            xcgi_param_value_named("perc"),
            szErrorNoPatients,
            xcgi_param_value_named("nump")            
            ); */
    }

    if (USE_CGI) xcgi_exit();

    return 0;
}
