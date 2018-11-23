#include <iostream>
#include "scan.h"
#include "util.h"
#include "parser.h"
#include "cgen.h"

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
FILE *source;
FILE *listing;
FILE *code;
bool is_optimized;

/* allocate and set tracing flags */
int EchoSource = FALSE;
int TraceScan = FALSE;
int TraceParse = FALSE;
int TraceAnalyze = FALSE;
int TraceCode = FALSE;

int Error = FALSE;


int main(int argc, char *argv[]) {

    //std::cout << "输入TINY++源程序文件名: ";
    //char *srcfile_name = new char[50];
    //std::cin >> srcfile_name;

    char *srcfile_name = const_cast<char *>("../test.tny");



    if (argc >= 2) {
        srcfile_name = argv[1];
    }

    if (!strchr(srcfile_name, '.'))
        strcat(srcfile_name, ".tny");

    FILE *src_file = fopen(srcfile_name, "r");

    if (src_file == nullptr) {
        std::cerr << "噢噢，文件打不开!" << std::endl;
        exit(1);
    }


    source = src_file;

    if (argc == 3 && !strcmp(argv[2], "tokens")) { //仅输出token
        listing = stdout;

        do {
            int line = 0;
            auto tmp = getToken(line);
            if (tmp.first == ENDFILE)
                break;
            printToken(tmp.first, tmp.second.c_str(), line);
        } while (true);
        return 0;
    }


    //输出文件路径，默认输出到屏幕
    if (argc >= 3) {
        code = fopen(argv[2], "w");
    } else {
        code = stdout;
    }
    //报告文件路径，默认输出到屏幕
    if (argc >= 4)
        listing = fopen(argv[3], "w");
    else
        listing = stdout;

    is_optimized = false;
    if (argc >= 5)
        if (strcmp(argv[4], "-O") == 0 || strcmp(argv[4], "-o") == 0)
            is_optimized = true;

    fprintf(listing, "\nTINY COMPILATION: %s\n", srcfile_name);
    cgen();

}
