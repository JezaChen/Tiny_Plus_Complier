#include <iostream>
#include "scan.h"
#include "util.h"

/**
                       .:'
                       `%@|%@%`
                       !#&'  '$$'                          `|@#;
                      `%&%:    :&!                     .!@&:  !%`
                      :%|%!   .'|@@$%!`              ;@$'     `%;
                      :%'!&$@&;.      '$&!;%&&&|: `$&:     `%@;!%`
                      :&#@|`            .`.     `|%`      '$%%|;$:
                  `%#$;.                                 ;&;.%|'%;
                :&|.                                   .%$` .%|`%!
               |$'  '%$|:                             .|#&: `%!`%!
              ;$' !&;  `|@|.                              ;@@&:'%;
             .%|.|%`!######;            .|##@&##%`          '!`'$;
             `%! |$!&######;           !&: `!&####%.            ;$'
            `$%`  '%#####@;           :%:'$########!            .%|
           :$;                        !%'|#########%.            ;%'
          '$;     '$%.                 |@#########@:             '%;
         .%|     '$!|#######@;           .:|$&&$|'               .%!
         :$:     !%`!########$`                                  .||
         :%:     !%` .!@##@%'                                     |%.
         .%|     `%%.  :&$`                                       |%.
          '$;      '$##$:|$'                                      ||
           :&;            `$@|'..`:%@#$'                         `%!
            `$%`              ..`.                               '%;
              '&$'                                               ;%`
                .|@$'                                           `%!
                 .|&|&#!                                         :$;
                '$|        '&#$|:'``.``''.                        ;$'
               :$;               ..```..                          `%!
               ||.                                                 !%.
              `%;
              ;%'  .|%`                         `|:
             `%!     ;&|.                      ;&:
                       '&@$!'            :%@####;
                            '|:       `|!`
 ***/

/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;


int main() {

    //std::cout << "输入TINY++源程序文件名: ";
    //char *srcfile_name = new char[50];
    //std::cin >> srcfile_name;

    char* srcfile_name = "../test.tny";

    if (!strchr(srcfile_name, '.'))
        strcat(srcfile_name, ".tny");

    FILE *src_file = fopen(srcfile_name, "r");

    if (src_file == nullptr) {
        std::cerr << "噢噢，文件打不开!" << std::endl;
        exit(1);
    }

    source = src_file;
    listing = stdout;
    fprintf(listing, "\nTINY COMPILATION: %s\n", srcfile_name);
    do {
        int line = 0;
        auto tmp = getToken(line);
        if (tmp.first == ENDFILE)
            break;
        printToken(tmp.first, tmp.second.c_str(), line);
    } while (true);


}
