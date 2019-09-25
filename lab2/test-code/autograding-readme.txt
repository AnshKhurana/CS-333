A few sample testcases and their expected outputs have been provided for your reference. You can manually copy the testcases into the xv6 folder, add them to the xv6 Makefile, and run them from the xv6 shell. In this way, you can incrementally test the correctness of your new system calls and scheduler, as and when you add the necessary code.

OR

You can run all testcases in one shot using our autograding scripts.

Untar the provided tar ball of autograding scripts. Place your solution code in the student_modified_files folder. Make sure you have the original xv6 code and the patch tar balls ready.

Next run the following script:

./generateOutput.sh <folder-to-store-output>

This script installs the original xv6 code, patches it, copies your solution, the test cases, and some files modified by us to enable running all test cases during bootup within the shell. The outputs of the test cases are collected in individual text files and stored in the output folder provided as argument to the script.

Next, run the following script to compare your output with the expected output.

./autograder.sh expected_output <your-output-folder>

Caveat: Testcase 3 prints the number of context switches and this number will differ slightly across execution runs. So it is alright if your answers do not exactly match the expected output.

Caveat: Testcases 5 and 6 set priorities of processes, run some compute intensive task in the processes, and check their exit order. It is expected that higher priority processes will finish faster and hence exit earlier. However, you may not get the same desired exit order always due to some scheduler non-determinism.
