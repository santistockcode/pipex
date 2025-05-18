./quick_test_runner.sh ./../pipex 'ls'            'ls'
./quick_test_runner.sh ./../pipex 'grep hello'    'wc -l'
./quick_test_runner.sh ./../pipex 'tr a-z A-Z'    'cat -n'
./quick_test_runner.sh ./../pipex "grep 'hello'"    'wc -l'
./quick_test_runner.sh ./../pipex "grep hello" "sleep 3"
./quick_test_runner.sh ./../pipex "sleep 3" "ls"