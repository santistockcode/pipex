/* tests/unit/test_cmdpath.c */
#include <criterion/criterion.h>
#include "../../include/utils.h"
#include <unistd.h>

Test(cmd_resolve_path, finds_ls_on_normal_path)
{
    char *fake_env[] = { "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", NULL };
    char *p = cmd_resolve_path("ls", fake_env);
    cr_expect_not_null(p);
    free(p);
}

// Test(sample, test) {
//     cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
//     cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
//     cr_assert(strlen("") == 0);
// }