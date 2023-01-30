import matplotlib.pyplot as plt

# open the putty log file
# with open('putty.log') as f:
with open('ppg_qt_buffer_grasp.log') as f:
    # read the file into a list
    lines = f.readlines()

signalarray = []
for line in lines:
    line = line.rstrip()
    line = line.split()
    line = float(line[0])
    signalarray.append(line)
    print(line)
    
plt.xlabel('Time Frame (9600 Baud)')
plt.ylabel('PPG Signal')
plt.plot(signalarray)
plt.show()