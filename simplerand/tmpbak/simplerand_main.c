#include "xcgi.c"
#include "simplerand.h"

#define USE_CGI 1

//extern int errno, sys_nerr;
extern void simplerand_(const int* iPercent, const int* iNoPatients);

char* CGI_TARGET = "simplerand.cgi";

int main(int argc, char** argv, char* envp[])
{
    const char* _szErrorPercent = "<p><font color=\"red\">&gt;&gt;&gt; Percentage must be between 0 and 100</font></p>";
    const char* _szErrorNoPatients = "<p><font color=\"red\">&gt;&gt;&gt; Number of patients must be between 0 and 10000</font></p>";
    char* szCategory, szErrorPercent, szErrorNoPatients;
    int iPercent, iNoPatients;
    int bErr = 0;

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }
    
    fprintf(stderr, "test\n");
    
    iPercent    = atoi(xcgi_param_value_named("perc"));
    iNoPatients = atoi(xcgi_param_value_named("pats"));
    
    if (0 == strlen(xcgi_param_value_named("perc"))
        || iPercent < 0 || iPercent > 100) {
        szErrorPercent = _szErrorPercent;
        bErr = 1;
    } else {
        szErrorPercent = NULL;
    }
        
    if (0 == strlen(xcgi_param_value_named("pats"))
        || iNoPatients < 0 || iNoPatients > 10000) {
        szErrorNoPatients = _szErrorNoPatients;
        bErr = 1;
    } else {
        szErrorNoPatients = NULL;
    }
    
    if (0 == bErr) {
        simplerand_(&iPercent, &iNoPatients);
        printf(
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
            iPercent, &iNoPatients);
            
        return 0;
    }

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
"<table width=\"75%\" border=\"0\">\n"
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
"    <td>: Good</td>\n"
"  </tr>\n"
"  <tr> \n"
"    <td>&nbsp;</td>\n"
"    <td>: V. Good</td>\n"
"  </tr>\n"
"</table>\n"
"<p align=\"left\">A fourth category will be defined by the user. Choose a category, \n"
"  and the number of people out of 100 who have this characteristic (e.g. for Diabetic, \n"
"  5 out of 100).</p>\n"
"<form method=\"post\" action=\"%s\">\n"
"%s\n"
"  <p>Category \n"
"    <input type=\"text\" name=\"textfield\" value=\"%s\">\n"
"  </p>\n"
"  <p>No. people out of 100 with this characteristic \n"
"    <input type=\"text\" name=\"textfield2\" value=\"%d\">\n"
"  </p>\n"
"  <p>Choose a number of patients to be randomised, up to 100,000, and they will \n"
"    be divided by simple randomisation, and a breakdown of the proportions of \n"
"    the above characteristics will be displayed.</p>\n"
"  <p> \n"
"%s\n"
"    <input type=\"text\" name=\"textfield3\"  value=\"%d\">\n"
"    Enter the number of patients to be randomised, and hit the 'Go' button...</p>\n"
"  <p>\n"
"    <input type=\"submit\" name=\"Submit\" value=\"Go\">\n"
"  </p>\n"
"</form>\n"
"</body>\n"
"</html>",
    CGI_TARGET,    
    szCategory,
    szErrorPercent,
    iPercent,
    szErrorNoPatients,
    iNoPatients);

    return 0;
}
