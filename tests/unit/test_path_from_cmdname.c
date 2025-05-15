/* tests/unit/test_cmdpath.c */
#include <criterion/criterion.h>
#include "../../include/utils.h"
#include <unistd.h>

Test(path_from_cmdname,1)
{
    char *fake_envp[] = { "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", NULL };
    char *p = path_from_cmdname("ls", fake_envp);
    cr_expect_not_null(p);
    free(p);
}

Test(path_from_cmdname, custom_path)
{
    char *fake_envp[] = { "PATH=/custom/bin:/usr/local/bin:/usr/bin", NULL };
    char *p = path_from_cmdname("ls", fake_envp);
    cr_expect_not_null(p);
    cr_expect_str_eq(p, "/custom/bin/ls");
    free(p);
}

Test(path_from_cmdname, other_env_variables)
{
    char *fake_envp[] = { "USER=john", "HOME=/home/john", "LANG=en_US.UTF-8", NULL };
    char *p = path_from_cmdname("ls", fake_envp);
    cr_expect_null(p);
    free(p);
}

Test(path_from_cmdname, all_env_variables)
{
    char *fake_envp[] = { "USER=john", "HOME=/home/john", "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", "LANG=en_US.UTF-8", NULL };
    char *p = path_from_cmdname("ls", fake_envp);
    cr_expect_not_null(p);
    cr_expect_str_eq(p, "/usr/bin/ls");
    free(p);
}

Test(path_from_cmdname, non_existing_command)
{
    char *fake_envp[] = { "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", NULL };
    char *p = path_from_cmdname("non_existing_command", fake_envp);
    cr_expect_null(p);
    free(p);
}


// Test(sample, test) {
//     cr_expect(strlen("Test") == 4, "Expected \"Test\" to have a length of 4.");
//     cr_expect(strlen("Hello") == 4, "This will always fail, why did I add this?");
//     cr_assert(strlen("") == 0);
// }