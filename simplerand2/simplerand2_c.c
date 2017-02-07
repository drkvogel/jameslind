#include <math.h>
#include "xcgi.c"
#include "simplerand2_results.c"
#define USE_CGI 1
#define FORM_NONE       0
#define FORM_REC        1
#define FORM_UNREC      2

//extern int errno, sys_nerr;
int iFormSubmitted;
int iPercent, iNrand, iNrandRec, iNrandUnrec;
int iPower;
char szCategory[100] = "";
int seed = 0;
char* CGI_TARGET = "simplerand3.cgi";

int main(int argc, char** argv, char* envp[]) {
    const char _szErrorNrandRec[]   = "<p><font color=\"red\">*** Number of patients must be between 0 and 10000 ***</font></p>\n";
    const char _szErrorCategory[]   = "<tr><td>&nbsp;</td><td><span><font color=\"red\">&gt;&gt;&gt; Please enter a category to be randomised</font></td></tr></span>\n";
    const char _szErrorPercent[]    = "<tr><td>&nbsp;</td><td><span><font color=\"red\">&gt;&gt;&gt; Percentage must be between 0 and 100</font></td></tr></span>\n";
    const char _szErrorNrandUnrec[] = "<tr><td>&nbsp;</td><td><span><font color=\"red\">*** Number of patients must be between 0 and 10000 ***</font></td></tr></span>\n";
    char szPercent[4]           = "\0";
    char szNrandRec[6]          = "\0";
    char szNrandUnrec[6]        = "\0";
    char szPower[2]             = "\0";
    char szErrorCategory[1024];
    char szErrorPercent[1024];
    char szErrorNrandRec[1024];
    char szErrorNrandUnrec[1024];
    int bErr = 0;

    strcpy(szErrorCategory, "\0");
    strcpy(szErrorPercent, "\0");
    strcpy(szErrorNrandRec, "\0");
    strcpy(szErrorNrandUnrec, "\0");
    if (USE_CGI) { // live
        xcgi_init(argc, argv);
        xcgi_header("html");
    } //fprintf(stderr, "test\n");
    
    if (0 == strlen(xcgi_param_value_named("ftype"))) { // nothing submitted
        iFormSubmitted = FORM_NONE;
    } else { // user has submitted form
        iFormSubmitted = atoi(xcgi_param_value_named("ftype"));
    }

    if (FORM_NONE != iFormSubmitted) {
        strcpy(szPercent,    xcgi_param_value_named("perc"));       // Percentage
        strcpy(szNrandRec,   xcgi_param_value_named("nrandrec"));   // Starting number to randomise
        strcpy(szNrandUnrec, xcgi_param_value_named("nrandunrec")); // not used?
        strcpy(szCategory,   xcgi_param_value_named("cat"));        // not used, always "Very anxious"
        strcpy(szPower,      xcgi_param_value_named("power"));      // 10, 100, 1000 times bigger
        iPercent    = atoi(szPercent);
        iNrandRec   = atoi(szNrandRec);
        iNrandUnrec = atoi(szNrandUnrec);
        iPower      = atoi(szPower);
        seed = atoi(xcgi_param_value_named("seed"));
        if (FORM_REC == iFormSubmitted) {
            iNrand = iNrandRec;
            if (0 == strlen(szNrandRec) || iNrand < 0 || iNrand > 100000) { // check inputs
                strcpy(szErrorNrandRec, _szErrorNrandRec); bErr = 1;
            }
        }
        // if (FORM_UNREC == iFormSubmitted) { // redundant? custom category
        //     iNrand = iNrandUnrec;
        //     if (0 == strlen(szNrandUnrec) || iNrand < 0 || iNrand > 100000) {
        //         strcpy(szErrorNrandUnrec, _szErrorNrandUnrec); bErr = 1;
        //     }
        //     if (0 == strlen(szPercent) || iPercent < 0 || iPercent > 100) {
        //         strcpy(szErrorPercent, _szErrorPercent); bErr = 1;
        //     }
        //     if (0 == strlen(szCategory)) {
        //         strcpy(szErrorCategory, _szErrorCategory); bErr = 1;
        //     }
        // }
    }
    if (0 == bErr && FORM_NONE != iFormSubmitted) { // form submitted and no errors
        printresults();
    } else { // show introduction
        printf(
            "<html>\n"
            "<head>\n"
            "<title>Simple Randomisation</title>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
            "\n"
            "</style>\n"
            "<link rel=\"stylesheet\" href=\"cochrane.css\" type=\"text/css\">\n"
            "\n"
            "</head>\n"
            "\n"
            "<body bgcolor=\"#FFFFFF\">\n"
            "<table border=1 cellspacing=0 cellpadding=10 width=750 bordercolor=\"#003366\" align=\"center\">\n"
            "<tr><td>\n"
            //"<table width=\"80%%\" align=\"center\"><tr><td>\n"
            "<table width=\"200\" border=\"0\" align=\"left\" cellpadding=\"3\">\n"
            "        <tr>\n"
            "          <td bgcolor=\"#EAEEF7\" width=\"100\" height=\"2\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/\" class=\"menubar\">Home</a></div></td>\n"
            "          <td bgcolor=\"#EAEEF7\" height=\"2\" width=\"100\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/search\" class=\"menubar\">Search</a></div></td>\n"
            "        </tr>\n"
            "        <tr bgcolor=\"#EAEEF7\">\n"
            "          <td colspan=\"2\" class=\"nav\"> <div align=\"center\"><b><strong style=\"color: #003366\">jameslindlibrary.org</strong></b></div></td>\n"
            "        </tr>\n"
            "      </table>\n"
            "  <table border=\"0\">"
            "    <tr>"
            "      <td width=\"80\">&nbsp;</td>"
            "      <td>"
            "        <h3 align=\"right\">Allocating sufficiently large numbers of patients at random\n"
            "  to create similar treatment comparison groups</h3>\n"
            "      </td>"
            "      <td><img src=\"jll_logo.jpg\" width=\"113\" height=\"153\"></td>"
            "    </tr>"
            "  </table>"
            "<h4>Comparing like with like</h4>\n"
            "<p>Random allocation of patients to treatment comparison groups is one of the \n"
            "  ways to ensure that tests of treatments are fair. Although random allocation can never\n"
            "  ensure that treatment comparison groups are exactly comparable in all respects,\n"
            "  it ensures that the composition of the comparison groups has not been biased by\n"
            "  clinician or patient choices. Unless allocation bias is reduced in this way, any\n"
            "  differences in the progress of patients in the comparison groups may simply\n"
            "  reflect the fact that they were composed of patients who were different to begin\n"
            "  with, and nothing to do with relative effects of different treatments.\n"
            "<h4>Recorded, known patient characteristics</h4>\n"
            "<p>Patients in the treatment comparison groups that have been generated using\n"
            "  random allocation need to be sufficiently alike in characteristics (age, for\n"
            "  example) which are known to influence their health problems. Reports of\n"
            "  treatment comparisons, therefore, need to show that 'like patients' have been\n"
            "  compared with 'other like patients'.\n"
            "<h4>Unrecorded and, as yet, undiscovered patient characteristics</h4>\n"
            " <p>Patients in treatment comparison groups who differ in respect of unrecorded and\n"
            "  as yet undiscovered characteristics of importance obviously cannot be shown to\n"
            "  be comparable. However, because random allocation ensures that any\n"
            "  unrecorded differences have been due to chance, these differences can be taken\n"
            "  into account in statistical analyses, usually by calculating confidence intervals.</p>\n"
            "<h4>Achieving balance in important patient characteristics</h4>\n"
            "<p>The larger the number of patients allocated at random to treatment comparison\n"
            "  groups, the more likely it is that the comparison groups will be made up of\n"
            "  patients with comparable characteristics, and the less likely that large imbalances\n"
            "  will remain. You can see this effect of increasing the numbers of patients using\n"
            "  the random allocation program below. Ask the program to randomise successively\n"
            "  larger numbers of patients (up to 100,000) and watch the way that the\n"
            "  characteristics of the patients in the treatment comparison groups become\n"
            "  increasingly balanced.</p>\n");

        printf(
            "%s\n\n"
            "<form method=\"post\" action=\"\">\n"
            "  <input type=\"text\" name=\"nrandrec\" value=\"%s\" size=\"6\" maxlength=\"6\">\n"
            "  <span>Enter the number of patients to be randomised, and hit the Go button....</span>\n"
            "  <input type=\"hidden\" name=\"ftype\" value=\"%d\">\n"
            "  <input type=\"hidden\" name=\"power\" value=\"0\">\n"
            "  <input type=\"submit\" name=\"Submit\" value=\"Go\">\n"
            "</form>\n",
            szErrorNrandRec,
            szNrandRec,
            FORM_REC);      
            fflush(stdout);
    
        printf(
            "<p>&nbsp;</p>\n"
            "<p>&nbsp; </p>\n"
            "</td></tr></table>\n"
            "</body>\n"
            "</html>");
        fflush(stdout);
    }

    if (USE_CGI) xcgi_exit();
    return 0;
}
