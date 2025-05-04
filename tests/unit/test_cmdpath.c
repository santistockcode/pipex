/* tests/unit/test_cmdpath.c */
#include <criterion/criterion.h>
#include "util.h"
#include <unistd.h>

Test(cmd_resolve_path, finds_ls_on_normal_path)
{
    char *fake_env[] = { "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", NULL };
    char *p = cmd_resolve_path("ls", fake_env);
    cr_expect_not_null(p);
    cr_expect(eq(int, access(p, X_OK), 0));   // really executable
    free(p);
}
