#include <math.h>
#include "xcgi.c"

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

#include "simplerand2_results.c"

void printresults1() {
    int seed = 0;

    int nrand = 0, propx = 0, nranda = 0, nrandb = 0,
    fact1_1a = 0, fact1_1b = 0, fact1_2a = 0, fact1_2b = 0, fact2_1a = 0, fact2_1b = 0,
    fact2_2a = 0, fact2_2b = 0, fact2_3a = 0, fact2_3b = 0, fact3_1a = 0, fact3_1b = 0,
    fact3_2a = 0, fact3_2b = 0, fact3_3a = 0, fact3_3b = 0, fact3_4a = 0, fact3_4b = 0,
    factx_1a = 0, factx_1b = 0, factx_2a = 0, factx_2b = 0,
    fact1_1an = 0, fact1_1bn = 0, fact1_2an = 0, fact1_2bn = 0, fact2_1an = 0, fact2_1bn = 0,
    fact2_2an = 0, fact2_2bn = 0, fact2_3an = 0, fact2_3bn = 0, fact3_1an = 0, fact3_1bn = 0,
    fact3_2an = 0, fact3_2bn = 0, fact3_3an = 0, fact3_3bn = 0, fact3_4an = 0, fact3_4bn = 0,
    factx_1an = 0, factx_1bn = 0, factx_2an = 0, factx_2bn = 0;

    if (FORM_REC == iFormSubmitted)
        nrand = iNrandRec;
    else
        nrand = iNrandUnrec;

    getseed_(&seed);
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

    printf(
"<html>\n"
"<head>\n"
"<title>Randomisation results</title>\n"
"<meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\">\n"
"\n\n"

"<link rel=\"stylesheet\" href=\"cochrane.css\" type=\"text/css\">\n"
"\n\n"
"</head>\n"
"\n"
"<body bgcolor=\"#FFFFFF\">\n"

//"<table width=\"80%%\" align=\"center\">\n"
//"<tr><td>"

"<table border=1 cellspacing=0 cellpadding=10 width=750 bordercolor=\"#003366\" align=\"center\">\n"
"<tr><td>\n"

"  <table width=\"100%%\">\n"
"    <tr><td>\n"
"      <table width=\"200\" border=\"0\" align=\"left\" cellpadding=\"3\">\n"
"        <tr>\n"
"          <td bgcolor=\"#EAEEF7\" width=\"100\" height=\"2\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/index.html\" class=\"menubar\">Home</a></div></td>\n"
"          <td bgcolor=\"#EAEEF7\" height=\"2\" width=\"100\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/contents.php\" class=\"menubar\">Contents</a></div></td>\n"
"        </tr>\n"
"        <tr bgcolor=\"#EAEEF7\">\n"
"          <td colspan=\"2\"> <div align=\"center\"><b><strong style=\"color: #003366\">jameslindlibrary.org</strong></b></div></td>\n"
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
"        <tr>\n"
"          <td>&nbsp;</td>\n"
"          <td>&nbsp;</td>\n"
"          <td align=\"right\">&nbsp;</td>\n"
"          <td align=\"right\">&nbsp;</td>\n"
"        </tr>\n"
"        <tr> \n"
"          <td>&nbsp;</td>\n"
"          <td colspan=\"3\">\n"
"            <form name=\"form1\" >\n"
"              <input type=\"hidden\" name=\"seed\" value=\"%d\">\n"
"              <input type=\"hidden\" name=\"ftype\" value=\"%d\">\n"
"              <input type=\"hidden\" name=\"cat\" value=\"%s\">\n"
"              <input type=\"hidden\" name=\"nrandrec\" value=\"%d\">\n"
"              <input type=\"hidden\" name=\"perc\" value=\"%d\">\n"
"              <input type=\"hidden\" name=\"power\" value=\"0\">\n"
"              <input type=\"submit\" name=\"Submit\" value=\"Make 10 times bigger\">\n"
"            </form>\n"
"          </td>\n"
"        </tr>\n",
            seed,
            iFormSubmitted + 2,
            (FORM_UNREC == iFormSubmitted) ? szCategory : "Very anxious?",
            nrand, iPercent);
       
    printf(
"</table>\n"
"<p><p><p><a href=\"javascript:history.go(-1)\"  onMouseOver=\"self.status=document.referrer;return true\"><<< Go Back</a>"
"</td></tr></table>\n"
"</body>\n"
"</html>\n");

} // printresults1()

int main(int argc, char** argv, char* envp[])
{
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

    if (USE_CGI) {
        xcgi_init(argc, argv);
        xcgi_header("html");
    }

    fprintf(stderr, "test\n");
    
    if (0 == strlen(xcgi_param_value_named("ftype")))
        iFormSubmitted = FORM_NONE;
    else
        iFormSubmitted = atoi(xcgi_param_value_named("ftype"));

    if (FORM_NONE != iFormSubmitted) {

        strcpy(szPercent,    xcgi_param_value_named("perc"));
        strcpy(szNrandRec,   xcgi_param_value_named("nrandrec"));
        strcpy(szNrandUnrec, xcgi_param_value_named("nrandunrec"));
        strcpy(szCategory,   xcgi_param_value_named("cat"));
        strcpy(szPower,      xcgi_param_value_named("power"));
    
        iPercent    = atoi(szPercent);
        iNrandRec   = atoi(szNrandRec);
        iNrandUnrec = atoi(szNrandUnrec);
        iPower      = atoi(szPower);

        seed = atoi(xcgi_param_value_named("seed"));

        if (FORM_REC == iFormSubmitted) {
            iNrand = iNrandRec;

            if (0 == strlen(szNrandRec)
                || iNrand < 0 || iNrand > 100000) {
                strcpy(szErrorNrandRec, _szErrorNrandRec);
                bErr = 1;
            }
        }

        if (FORM_UNREC == iFormSubmitted) {
            iNrand = iNrandUnrec;

            if (0 == strlen(szNrandUnrec)
                || iNrand < 0 || iNrand > 100000) {
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
    
    if (0 == bErr  && FORM_NONE != iFormSubmitted) {

        printresults();

    } else {

        // introduction

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
"          <td bgcolor=\"#EAEEF7\" width=\"100\" height=\"2\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/index.html\" class=\"menubar\">Home</a></div></td>\n"
"          <td bgcolor=\"#EAEEF7\" height=\"2\" width=\"100\"> <div align=\"center\"><a href=\"http://www.jameslindlibrary.org/contents.php\" class=\"menubar\">Contents</a></div></td>\n"
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

/* 20th september 2006
"<p>It helps to create comparison \n"
"  groups made up of patients without any concious or unconcious difference in the\n"
"  allocation of patients to the treatment groups. Unless potential bias is reduced \n"
"  in this way, any difference in the progress of patients in the comparison groups \n"
"  may simply reflect the fact that they were composed of patients who were different \n"
"  to begin with, and that the differences have nothing to do with relative effects \n"
"  of the different treatments.</p>\n"
"<p>Sometimes it is said by way of shorthand that randomisation creates \"comparable\n"
"  groups\". Actually, the argument is subtler: chance mechanisms can never assure that\n"
"  two groups are comparable in all respects. However, when using a chance mechanism,\n"
"  you can be certain that the composition of the groups has not been influenced by patient\n"
"  or physician preference. In addition, randomisation guarantees that any difference\n"
"  between groups is due to chance. The latter aspect is important since it permits to use\n"
"  statistics: to calculate whether the actual outcome in a treated group is sufficiently\n"
"  different from what might be due to the play of chance.</p>\n"
"<p>To verify whether the groups that will be compared are sufficiently equal in important\n"
"  prognostic characteristics, all reports from randomised trials include a table of baseline\n"
"  characteristics. In that table, you will find all important  factors that might affect the\n"
"  outcome of the patient (like being older or younger), and that are recorded. By\n"
"  definition, unknown characteristics - which medical science does not yet know - cannot\n"
"  be recorded. The aim of this table is to assure the reader that like is compared with like,\n"
"  as far as the present state of medical science goes. If important characteristics differ\n"
"  between the groups, this can be remedied by adjusting the comparison (often by\n"
"  multivariate models). Unknown important characteristics cannot be verified, but we can\n"
"  rely on randomisation that any difference will be due to chance, which is taken into\n"
"  account in the statistics of the trial (usually by confidence intervals).</p>\n"
"<p>When the effect you are looking for is small, trials need to be large, because otherwise it\n"
"  can happen all too easily that important baseline characteristics are largely different\n"
"  between the groups. You can see this or yourself in the little game below.</p>\n"
"<h4>Achieving balance in important RECORDED patient characteristics</h4>\n"
"<p>The larger the number of patients assigned at random to treatment comparison \n"
"  groups, the more likely it is that the comparison groups will be made up of \n"
"  patients with comparable characteristics, and the less likely that\n"
"  large imbalances will remain. You can see this effect of increasing the \n"
"  numbers of patients using this randomisation program. Ask the program to randomise \n"
"  successively larger numbers of patients (up to 100,000) and watch the way that \n"
"  the characteristics of the patients in the treatment comparison groups become \n"
"  increasingly balanced.</p>\n");*/

    printf(
"%s\n\n"
"<form method=\"post\" action=\"\">\n"
"  <input type=\"text\" name=\"nrandrec\" value=\"%s\" size=\"6\" maxlength=\"6\">\n"
"  <span>Enter the number of patients to be randomised, and hit the Go button....</span>\n"
"  <input type=\"hidden\" name=\"ftype\" value=\"%d\">\n"
"  <input type=\"hidden\" name=\"power\" value=\"0\">\n"
"  <input type=\"submit\" name=\"Submit\" value=\"Go\">\n"
"</form>\n"

/*"<h4><br>\n"
"  Achieving balance in important UNRECORDED patient characteristics</h4>\n"
"<p>The program uses duration and severity of patients' health problems, and their \n"
"  ages, because these characteristics of patients are quite likely to influence \n"
"  the outcome of their illnesses, and possibly the effects of treatments as well. \n"
"  But what about characteristics that are important but haven't been recorded? \n"
"  For example, some patients may be very anxious about their illness, and this \n"
"  may have an impact on their progress and the effects of treatment.</p>\n"
"<p>Allocating sufficiently large numbers of patients at random to treatment comparison \n"
"  groups achieves balance in unrecorded as well as recorded characteristics of \n"
"  importance. To show this, replace 'very anxious' with an unrecorded characteristic \n"
"  of your own choice, indicate its likely frequency, and run the program by hitting \n"
"  Go.</p>"*/,
    szErrorNrandRec,
    szNrandRec,
    FORM_REC);
    
/*    printf(
"<p style=\"font-weight: bold\">"
"  You should understand first of all that even small differences might wreck a trial that\n"
"  is looking into small effects of treatment. Hence, the importance of having large\n"
"  numbers when you want to study a small effect. Mind that this remains a game of\n"
"  chance: if you were to run the randomiser 30,000 times, for 30,000 characteristics,\n"
"  you will certainly encounter one that is very different between the groups. Actually,\n"
"  we have 30,000 genes, so this means that it is always possible that there are\n"
"  important, but as yet unknown factor differs between the groups.</p>");*/
      
    fflush(stdout);

/*    printf(
"<form method=\"post\" action=\"\">\n"
"  <table border=\"0\" width=\"80%%\" align=\"center\">\n"
"%s\n\n"
"    <tr> \n"
"      <td><span>Name the unrecorded characteristic here:</span></td>\n"
"      <td> \n"
"        <input type=\"text\" name=\"cat\" value=\"%s\">\n"
"      </td>\n"
"    </tr>\n"
"%s\n\n"
"    <tr> \n"
"      <td><span>How many people out of 100 have this characteristic?</span></td>\n"
"      <td>\n"
"        <input type=\"text\" name=\"perc\" value=\"%s\" size=\"3\" maxlength=\"3\">\n"
"      </td>\n"
"    </tr>\n"
"%s\n\n"
"    <tr>\n"
"      <td><span>Enter the number of patients to be randomised...</span></td>\n"
"      <td><input type=\"text\" name=\"nrandunrec\" value=\"%s\" size=\"6\" maxlength=\"6\"></td>\n"
"    </tr>\n"
"    <tr>\n"
"      <td><span>And hit Go:</span></td>\n"
"      <td>\n"
"        <input type=\"hidden\" name=\"ftype\" value=\"%d\">\n"
"        <input type=\"hidden\" name=\"power\" value=\"0\">\n"
"        <input type=\"submit\" name=\"Submit2\" value=\"Go\">\n"
"      </td>\n"
"    </tr>\n"
"  </table>\n"
"</form>\n"
"<p>&nbsp;</p>\n"
"<p>&nbsp; </p>\n"
"</td></tr></table>\n"
"</body>\n"
"</html>",
        szErrorCategory,
        szCategory,
        szErrorPercent,
        szPercent,
        szErrorNrandUnrec,
        szNrandUnrec,
        FORM_UNREC);*/
        
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
