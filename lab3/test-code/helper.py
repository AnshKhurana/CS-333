import os
import subprocess
import signal

NUM_TESTCASES = 7

def main():
    outputs = [[] for i in range(NUM_TESTCASES)]

    # Change to XV-6 folder and setup qemu 
    os.chdir("xv6-public")
    p = subprocess.Popen(["make", "clean"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    p.wait()
    p = subprocess.Popen(["make"], stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
    p.wait()
    p = subprocess.Popen(["make", "qemu"], stdout=subprocess.PIPE,universal_newlines=True, stderr=subprocess.STDOUT, preexec_fn=os.setsid)
    curIndex = None

    # Iterate over outputs from running qemu process
    while p.poll() is None:
        output = p.stdout.readline()
        #print(output) #uncomment for debug
        line = output.split()
        if len(line) == 3 and line[0] == 'Testcase':
            if line[2] == 'Start':
                curIndex = int(line[1])
            elif line[2] == 'End':
                curIndex = None       
        elif len(line) == 1 and line[0] == 'DONE':
            os.killpg(os.getpgid(p.pid), signal.SIGKILL)
            os.system("reset")
            for id in range(NUM_TESTCASES):
                file = open("out"+str(id)+".txt", "w")
                for i in range(len(outputs[id])):
                    file.write(outputs[id][i])
                file.close()
            break
        elif len(line) > 0 and curIndex is not None:
            outputs[curIndex].append(output)

if __name__ == '__main__':
    main()




