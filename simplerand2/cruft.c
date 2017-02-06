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
"  Go.</p>"*/

/*    printf(
"<p style=\"font-weight: bold\">"
"  You should understand first of all that even small differences might wreck a trial that\n"
"  is looking into small effects of treatment. Hence, the importance of having large\n"
"  numbers when you want to study a small effect. Mind that this remains a game of\n"
"  chance: if you were to run the randomiser 30,000 times, for 30,000 characteristics,\n"
"  you will certainly encounter one that is very different between the groups. Actually,\n"
"  we have 30,000 genes, so this means that it is always possible that there are\n"
"  important, but as yet unknown factor differs between the groups.</p>");*/

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
