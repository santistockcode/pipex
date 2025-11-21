import os

class CasesSuite():
    # register here every use case

class CaseScenario():
    
    def __init__(self, workspace, pipex_bin, pipex_args, function_assert):
        self.workspace = workspace
        self.pipex_bin = pipex_bin
        self.pipex_args = pipex_args
        self.function_assert = function_assert
    
    def create_workspace():
        # create a usable infile in the new workspace

    def better_call_pipex():
        # use pipex to generate outfile_pipex


def main():
    # check if pipex binary exists
    
    # check if mirror_bash_script exists (it accepts parameters)

    # init CasesSuite and CaseScenario

    # create workspaces for each CaseScenario

    # cp bash script into each workspace 

    # source bash script (to outfile_bash)

    # better_call_pipex (to outfile_pipex)

    # function assert outfile, or fd, or whatever 

if __name__ == "__main__":
    main()