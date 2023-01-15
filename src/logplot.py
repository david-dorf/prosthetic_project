import matplotlib.pyplot as plt

# open the putty log file
# with open('putty.log') as f:
with open('putty2.log') as f:
    # read the file into a list
    lines = f.readlines()

signalarray = []
for line in lines:
    # remove the newline character
    line = line.rstrip()
    # split the line into a list of strings
    line = line.split()
    # convert the list of strings into a list of floats
    line = float(line[0])
    # add the line to the signal array
    signalarray.append(line)
    # plot the line
    print(line)
    
plt.xlabel('Time Frame (9600 Baud)')
# EWMA is the Exponentially Weighted Moving Average
# plt.ylabel('MMG Signal Strength Squared (EWMA)')
plt.ylabel('EMG Signal Strength Squared (EWMA)')
plt.plot(signalarray)
plt.show()