@echo off

setlocal ENABLEDELAYEDEXPANSION

set arg=%1

if NOT DEFINED arg (
    echo No option specified.
    exit /b 2
)

rem Use "compile 0" to compile all source files.
rem Use "compile 1" to compile all files related to the project.
rem Use "compile 2" to generate the executable for the project.

set compile_src_id=0
set compile_proj_src_id=1
set gen_proj_exec_id=2

if %1 EQU %compile_src_id% (
    goto compile_all_src_files
) else if %1 EQU %compile_proj_src_id% (
    goto compile_project_only
) else if %1 EQU %gen_proj_exec_id% (
    goto generate_exec
) else (
    echo Unknown option "%1".
    exit /b 1
)

:compile_all_src_files
clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -Wno-unused-function -D_FORTIFY_SOURCE=2 *.c "C-MyBasics\*.c" "Miscellaneous\Analysis\.c" -c
set RETURN_STATUS=!errorlevel!

:compile_project_only
clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -D_FORTIFY_SOURCE=2 Translator.c "C-MyBasics\MyBasics.c" -c
clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -Wno-unused-function -D_FORTIFY_SOURCE=2 "Miscellaneous\Analysis\wdanalysis.c" -c
set RETURN_STATUS=!errorlevel!
goto cleanup

:generate_exec
clang -fstack-protector-strong -Og -pedantic -Wall -Werror -Wextra -Wfloat-equal -Wformat=2 -Wimplicit-atomic-properties -Wmissing-declarations -Wmissing-prototypes -Woverlength-strings -Wshadow -D_FORTIFY_SOURCE=2 Translator.c "C-MyBasics\MyBasics.c" -o Translator
exit /b !errorlevel!

:cleanup
del *.o /f /s /q
exit /b !RETURN_STATUS!
